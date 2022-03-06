#include <avr/io.h>
#include <avr/interrupt.h>
#include "gfxLib.h"
#include "timing.h"
#include "pages/pages.h"
#include "pages/timerPage.h"
#include "pages/alarmPage.h"

//days, months, years
#define INITIAL_DATE {1, 1, 2022}
//hours, minutes, seconds
#define INITIAL_TIME {0, 0, 0}
//first of January 2022 is a Saturday
#define INITIAL_DAY "Saturday"

date_time_t g_date_time = {INITIAL_DATE, INITIAL_TIME};
date_time_t g_date_time_last = {INITIAL_DATE, INITIAL_TIME};

char weekday[] = INITIAL_DAY;

date_time_t* get_g_date_time(){
	return &g_date_time;
}
date_t* get_g_date(){
	return &g_date_time.date;
}
time_t* get_g_time(){
	return &g_date_time._time;
}

int get_weekday(date_t* date){
	int d = date->days;
	int m = date->months;
	int y = date->years;
	return (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
}

//------------------------------------------
char activeAlarms = 0;
date_time_t alarms[8] = {
	{INITIAL_DATE,INITIAL_TIME},{INITIAL_DATE,INITIAL_TIME},
	{INITIAL_DATE,INITIAL_TIME},{INITIAL_DATE,INITIAL_TIME},
	{INITIAL_DATE,INITIAL_TIME},{INITIAL_DATE,INITIAL_TIME},
	{INITIAL_DATE,INITIAL_TIME},{INITIAL_DATE,INITIAL_TIME}
};

void alarms_add(int i){
	activeAlarms |= (1<<i);
}
void alarms_remove(int i){
	activeAlarms &= ~(1<<i);
}
char alarms_active(){
	return activeAlarms;
}
date_time_t* alarm_get(int i){
	return &alarms[i];
}

//--------------------------------------------
char activeTimers = 0;
time_t timers[8] = {
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},
};
time_t timers_last_time[8] = {
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},
};

void timers_add(int i){
	activeTimers |= (1<<i);
}
void timers_remove(int i){
	activeTimers &= ~(1<<i);
	timers_last_time[i].hours = -1;
}
char timers_active(){
	return activeTimers;
}
time_t* timer_get(int i){
	return &timers[i];
}
time_t* timer_get_last(int i){
	return &timers_last_time[i];
}

//----------------------------------------------------

char g_time_changed(){
	return what_changed(&g_date_time, &g_date_time_last);
}

char what_changed(date_time_t* now, date_time_t* last){
	char returnValue;
	if(now->date.years != last->date.years){
		returnValue = 0b1111111; //if year has changed, so have everything else,
	}else if(now->date.months != last->date.months){
		returnValue = 0b111111; // if month changed, so have... and so on.
	}else if(now->date.days != last->date.days){
		returnValue = 0b11111; //NOTE extra '1' for weekday
	}else if(now->_time.hours != last->_time.hours){
		returnValue = 0b111;
	}else if(now->_time.minutes != last->_time.minutes){
		returnValue = 0b11;
	}else if(now->_time.seconds != last->_time.seconds){
		returnValue = 0b1;
	}else{
		return 0;
	}
	last->date = now->date;
	last->_time = now->_time;
	return returnValue;// from left to right bits: year, month, day, weekday, hours, minutes, seconds
}
char what_time_changed(time_t* now, time_t* last){
	char returnValue = 0;
	if(now->hours != last->hours){
		returnValue = 0b111;
	}else if(now->minutes != last->minutes){
		returnValue = 0b11;
	}else if(now->seconds != last->seconds){
		returnValue = 0b1;
	}else{
		returnValue = 0;
	}
	*last = *now;
	return returnValue;
}

int days_in_month(int month){
	switch(month){
		case 1:
			return 31;
		case 2:
			return 28;
		case 3:
			return 31;
		case 4:
			return 30;
		case 5:
			return 31;
		case 6:
			return 30;
		case 7:
			return 31;
		case 8:
			return 31;
		case 9:
			return 30;
		case 10:
			return 31;
		case 11:
			return 30;
		case 12:
			return 31;
	}
	return -1;
}

date_t diff_date(date_t* date1, date_t* date2){
	int years = date1->years-date2->years;
	int months = date1->months-date2->months;
	int days = date1->days-date2->days;
	if(days < 0){
		months--;
	}
	if(months < 0){
		years--;
		months += 12;
	}
	if(days < 0){
		days += days_in_month(months);
	}
	return (date_t){
		days,
		months,
		years,
	};
}

time_t diff_time(time_t* time1, time_t* time2){
	time_t time = (time_t) {
		time1->hours-time2->hours,
		time1->minutes-time2->minutes,
		time1->seconds-time2->seconds
	};
	if(time.seconds<0){
		time.minutes--;
		time.seconds += 60;
	}		
	if(time.minutes<0){
		time.hours--;
		time.minutes += 60;
	}
	return time;
}

date_time_t diff_date_time(date_time_t* date_time1, date_time_t* date_time2){
	date_time_t date_time = (date_time_t) {
		diff_date(&date_time1->date, &date_time2->date),
		diff_time(&date_time1->_time, &date_time2->_time)
	};
	if(date_time._time.hours < 0){
		date_time._time.hours += 24;
		if(--date_time.date.days < 0){
			if(--date_time.date.months < 0){
				date_time.date.months += 12;
				date_time.date.years--;
			}
			date_time.date.days += days_in_month(date_time.date.months);
		}
	}
	return date_time;
}

char inc_years(date_t *date){
	if(date->years++ == 9999){
		date->years = 0;
		return 1;
	}
	return 0;
}

char inc_months(date_t *date){
	if(date->months++ == 12){
		date->months = 1;
		return 1;
	}
	return 0;
}


char inc_days(date_t *date){
	if(date->days++ == days_in_month(date->months)){
		date->days = 1;
		return 1;
	}
	return 0;
}


char inc_hours(time_t *time){
	if(++time->hours == 24){
		time->hours = 0;
		return 1;
	}
	return 0;
}

char inc_minutes(time_t *time){
	if(++time->minutes == 60){
		time->minutes = 0;
		return 1;
	}
	return 0;
}

char inc_seconds(time_t *time){
	if(++time->seconds == 60){
		time->seconds = 0;
		return 1;
	}
	return 0;
}

unsigned int add_seconds(time_t *_time, unsigned int seconds){
	unsigned int totalSeconds = _time->seconds + seconds;
	if(totalSeconds >= 60){
		_time->seconds = totalSeconds % 60;
		return totalSeconds/60;
	}else{
		_time->seconds += seconds;
		return 0;
	}
}

char inc_hours_time(time_t* _time){
	return inc_hours(_time);
}

char inc_minutes_time(time_t* _time){
	if(inc_minutes(_time)){
		return inc_hours_time(_time);
	}
	return 0;
}

char inc_seconds_time(time_t* _time){
	if(inc_seconds(_time)){
		return inc_minutes_time(_time);
	}
	return 0;
}

char add_seconds_time(time_t *_time, unsigned int seconds){
	unsigned int overflow = add_seconds(_time, seconds);
	char minuteOverflow = 0;
	while (overflow--){
		minuteOverflow += inc_minutes_time(_time);
	}
	return minuteOverflow;
}

void inc_months_date(date_time_t* dateTime){
	if(inc_months(&dateTime->date)){
		inc_years(&dateTime->date);
	}
}

void inc_days_date(date_time_t* dateTime){
	if(inc_days(&dateTime->date)){
		inc_months_date(dateTime);
	}
}

void inc_hours_date(date_time_t* dateTime){
	if(inc_hours(&dateTime->_time)){
		inc_days_date(dateTime);
	}
}

void inc_minutes_date(date_time_t* dateTime){
	if(inc_minutes(&dateTime->_time)){
		inc_hours_date(dateTime);
	}
}

void inc_seconds_date(date_time_t* dateTime){
	if(inc_seconds(&dateTime->_time)){
		inc_minutes_date(dateTime);
	}
}

void add_seconds_date(date_time_t *date, unsigned int seconds){
	unsigned int overflow = add_seconds(&date->_time, seconds);
	while (overflow--){
		inc_minutes_date(date);
	}
}


char dec_years(date_t *date){
	if(date->years-- == 0){
		date->years = 9999;
		return 1;
	}
	return 0;
}

char dec_months(date_t *date){
	if(date->months-- == 1){
		date->months = 12;
		return 1;
	}
	return 0;
}

char dec_days(date_t *date){
	if(date->days-- == 1){
		date->days = days_in_month(date->months);
		return 1;
	}
	return 0;
}

char dec_hours(time_t *time){
	if(time->hours-- == 0){
		time->hours = 23;
		return 1;
	}
	return 0;
}

char dec_minutes(time_t *time){
	if(time->minutes-- == 0){
		time->minutes = 59;
		return 1;
	}
	return 0;
}

char dec_seconds(time_t *time){
	if(time->seconds-- == 0){
		time->seconds = 59;
		return 1;
	}
	return 0;
}

unsigned int sub_seconds(time_t *time, unsigned int seconds){
	if(time->seconds >= seconds){
		time->seconds -= seconds;
		return 0;
	}else{
		unsigned int overflow = 1;
		unsigned int difference = seconds - time->seconds;
		while(difference >= 60){
			difference -= 60;
			overflow++;
		}
		time->seconds = 60 - difference;
		return overflow;
	}
}


char dec_hours_time(time_t* _time){
	return dec_hours(_time);
}

char dec_minutes_time(time_t* _time){
	if(dec_minutes(_time)){
		return dec_hours_time(_time);
	}
	return 0;
}

char dec_seconds_time(time_t* _time){
	if(dec_seconds(_time)){
		return dec_minutes_time(_time);
	}
	return 0;
}

char sub_seconds_time(time_t *_time, unsigned int seconds){
	unsigned int overflow = sub_seconds(_time, seconds);
	char timeOverflow = 0;
	while (overflow--){
		timeOverflow += dec_minutes_time(_time);
	}
	return timeOverflow;
}

void dec_months_date(date_time_t* dateTime){
	if(dec_months(&dateTime->date)){
		dec_years(&dateTime->date);
	}
}

void dec_days_date(date_time_t* dateTime){
	if(dec_days(&dateTime->date)){
		dec_months_date(dateTime);
		dateTime->date.days = days_in_month(dateTime->date.months);
	}
}

void dec_hours_date(date_time_t* dateTime){
	if(dec_hours(&dateTime->_time)){
		dec_days_date(dateTime);
	}
}

void dec_minutes_date(date_time_t* dateTime){
	if(dec_minutes(&dateTime->_time)){
		dec_hours_date(dateTime);
	}
}

void dec_seconds_date(date_time_t* dateTime){
	if(dec_seconds(&dateTime->_time)){
		dec_minutes_date(dateTime);
	}
}

void sub_seconds_date(date_time_t *date, unsigned seconds){
	unsigned int overflow = sub_seconds(&date->_time, seconds);
	while (overflow--){
		dec_minutes_date(date);
	}
}

void inc_what_date_time(char what, date_time_t *dateTime){
	if(what & 0b000001) inc_years(&dateTime->date);
	if(what & 0b000010) inc_months(&dateTime->date);
	if(what & 0b000100) inc_days(&dateTime->date);
	if(what & 0b001000) inc_hours(&dateTime->_time);
	if(what & 0b010000) inc_minutes(&dateTime->_time);
	if(what & 0b100000) inc_seconds(&dateTime->_time);
}
void dec_what_date_time(char what, date_time_t *dateTime){
	if(what & 0b000001) dec_years(&dateTime->date);
	if(what & 0b000010) dec_months(&dateTime->date);
	if(what & 0b000100) dec_days(&dateTime->date);
	if(what & 0b001000) dec_hours(&dateTime->_time);
	if(what & 0b010000) dec_minutes(&dateTime->_time);
	if(what & 0b100000) dec_seconds(&dateTime->_time);
}


void set_time(time_t *timeToChange, time_t *toTime){
	timeToChange->hours = toTime->hours;
	timeToChange->minutes = toTime->minutes;
	timeToChange->seconds = toTime->seconds;
}
void set_date(date_t *dateToChange, date_t *toDate){
	dateToChange->years = toDate->years;
	dateToChange->months = toDate->months;
	dateToChange->days = toDate->days;
}
void set_date_time(date_time_t* toChange, date_time_t* to){
	set_time(&toChange->_time, &to->_time);
	set_date(&toChange->date, &to->date);
}


void _update_alarms(int *currentPage){
	for(int i=0; i<8; i++){
		if(activeAlarms & (1<<i)){
			date_time_t diffAlarms = diff_date_time(&alarms[i], &g_date_time);
			if(diffAlarms.date.years < 0 || (diffAlarms._time.minutes == 0 && diffAlarms._time.hours == 0 && diffAlarms.date.days == 0 && diffAlarms.date.months == 0 && diffAlarms.date.years == 0)){
				alarms_remove(i);
				alarms[i] = (date_time_t) {{0,0,0},{0,0,0}};
				*currentPage = (int) ALARMPAGE;
				alarmPage_ring(i);
			}
		}
	}
}

void _update_timers(int *currentPage, unsigned int ticks){
	for(int i=0; i<8; i++){
		if(activeTimers & (1<<i)){
			if(sub_seconds_time(&timers[i], ticks)){
				timers_remove(i);
				timers[i] = (time_t) {0, 0, 0};
				*currentPage = (int) TIMERPAGE;
				timerPage_ring(i);
			}
		}
	}
}

unsigned int lastTick = 0;
unsigned int ticks = 0;
unsigned int lastMinute = 0;
void update_time(int * currentPage){
	cli();
	if(lastTick && ticks){
//BREAKPOINT  --  MAIN LOOP TOO SLOW!! MISSING SECONDS
		PORTB |= (1<<PORTB6);
	}
	
	add_seconds_date(&g_date_time, ticks);
	_update_timers(currentPage, ticks);
	if(g_date_time._time.minutes != lastMinute){
		_update_alarms(currentPage);
		lastMinute = g_date_time._time.minutes;
	}

	lastTick = ticks;
	ticks = 0;
	sei();
}
	

ISR(TIMER1_COMPA_vect){
	ticks++;
}

char buzzer = 0;
void enable_buzzer(){
	buzzer = 1;
	DDRA |= (1<<PORTA1);
}
void disable_buzzer(){
	buzzer = 0;
	DDRA &= ~(1<<PORTA1);
	PORTA &= ~(1<<PORTA1);
}

ISR(TIMER3_COMPA_vect){
	if(buzzer){
		PORTA ^= (1<<PORTA1);
	}
}

void clock_init(){
	sei();
	TIMSK1 |= (1 << OCIE1A);
	TIMSK3 |= (1 << OCIE3A);
	
	OCR1A = 62499; // 1/((62499+1)/(16 MHz/256)) = 1 Hz
	OCR3A = 15624; // 1/((15624+1)/(16 MHz/1)) = 1.024 kHz
	
	//WGM1[3:0] = 0100 mode 4 CTC with OCR1A as top.
	//CS1[2:0] = 100 to set 256 prescaler.
	//TCCR1A = 0 // (all 0 is default)
	TCCR1B |= (1<<WGM12) | (1<<CS12); 
	
	//WGM3[3:0] = 0100 mode 4 CTC with OCR3A as top.
	//CS3[2:0] = 001 to set 1 prescaler.
	//TCCR3A = 0 // (all 0 is default)
	TCCR3B |= (1<<WGM32) | (1<<CS30); 
	

}