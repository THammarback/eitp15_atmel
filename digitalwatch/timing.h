#ifndef TIMING_H_
#define TIMING_H_

typedef struct time_type {
	int hours;
	int minutes;
	int seconds;
} time_t;

typedef struct date_type{
	int days;
	int months;
	int years;
} date_t;

typedef struct date_time_type{
	date_t date;
	time_t _time;
} date_time_t;


void update_time(int * currentPage);
void clock_init();
void enable_buzzer();
void disable_buzzer();

char g_time_changed();
char what_changed(date_time_t* now, date_time_t* last);
char what_time_changed(time_t* now, time_t* last);

date_time_t diff_date_time(date_time_t * date_time1, date_time_t * date_time2);
date_t diff_date(date_t* date1, date_t* date2);
time_t diff_time(time_t* time1, time_t* time2);


date_time_t* get_g_date_time();
date_t* get_g_date();
time_t* get_g_time();


char inc_years(date_t *date);
char inc_months(date_t *date);
char inc_days(date_t *date);
char inc_hours(time_t *_time);
char inc_minutes(time_t *_time);
char inc_seconds(time_t *_time);
unsigned int add_seconds(time_t *time, unsigned int seconds);

char inc_hours_time(time_t *_time);
char inc_minutes_time(time_t *_time);
char inc_seconds_time(time_t *_time);
char add_seconds_time(time_t *_time, unsigned int seconds);

void inc_months_date(date_time_t *dateTime);
void inc_days_date(date_time_t *dateTime);
void inc_hours_date(date_time_t *dateTime);
void inc_minutes_date(date_time_t *dateTime);
void inc_seconds_date(date_time_t *dateTime);
void add_seconds_date(date_time_t *date, unsigned int seconds);

char dec_years(date_t *date);
char dec_months(date_t *date);
char dec_days(date_t *date);
char dec_hours(time_t *_time);
char dec_minutes(time_t *_time);
char dec_seconds(time_t *_time);
unsigned int sub_seconds(time_t *_time, unsigned int seconds);

char dec_hours_time(time_t *_time);
char dec_minutes_time(time_t *_time);
char dec_seconds_time(time_t *_time);
char sub_seconds_time(time_t *_time, unsigned int seconds);

void dec_months_date(date_time_t *dateTime);
void dec_days_date(date_time_t *dateTime);
void dec_hours_date(date_time_t *dateTime);
void dec_minutes_date(date_time_t *dateTime);
void dec_seconds_date(date_time_t *dateTime);
void sub_seconds_date(date_time_t *date, unsigned seconds);

void inc_what_date_time(char what, date_time_t *dateTime);
void dec_what_date_time(char what, date_time_t *dateTime);


void set_time(time_t *timeToChange, time_t *toTime);
void set_date(date_t *dateToChange, date_t *todate);
void set_date_time(date_time_t *ToChange, date_time_t *to);

time_t* timer_get(int i);
time_t* timer_get_last(int i);
void timers_add(int i);
void timers_remove(int i);
char timers_active();

date_time_t* alarm_get(int i);
date_time_t* alarm_get_diff(int i);
void alarms_add(int i);
void alarms_remove(int i);
char alarms_active();

int days_in_month(int month);
int get_weekday(date_t* date);

#endif /* TIMING_H_ */