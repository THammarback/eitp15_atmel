#include <avr/io.h>
#include <avr/interrupt.h>
#include "gfxLib.h"

int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 1;
int months = 1;
int years = 2022;

char weekday[] = "Saturday";

int time_get_seconds(){
	return (seconds);
}
int time_get_minutes(){
	return (minutes);
}
int time_get_hours(){
	return (hours);
}
int time_get_days(){
	return (days);
}
int time_get_months(){
	return (months);
}
int time_get_years(){
	return (years);
}

void _update_time(){
	if(seconds++ != 59){
		return;
	}
	seconds = 0;
	if(minutes++ != 59){
		return;
	}
	minutes = 0;
	if(hours++ != 23){
		return;
	}
	hours = 0;
	if(++days != 31){
		return;
	}
	days = 0;
	if(++months != 12){
		return;
	}
	months = 0;
	years++;
}


ISR(TIMER1_COMPA_vect){
	_update_time();
}

void clock_init(){
	sei();
	TIMSK1 |= (1 << OCIE1A);
	
	OCR1A = 62499; // (62499+1)/(16 MHz/256) = 1s
	
	//WGM1[3:0] = 0100 mode 4 CTC with OCR1A as top.
	//CS1[2:0] = 100 to set 256 prescaler.
	
	//TCCR1A = 0 // (all 0 is default)
	TCCR1B |= (1<<WGM12) | (1<<CS12); 
}