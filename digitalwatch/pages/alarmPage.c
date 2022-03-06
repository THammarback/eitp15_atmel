#include <avr/io.h>
#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "../timing.h"
#include "mainPage.h"
#include "pedoPage.h"

char alarmState = 0;
int alarm_selector = 0;

void _alarmPage_draw_frame(){
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x0A00);
	gfx_text_color(0xC800);
	gfx_set_cursor(7, 0);
	gfx_put_const_string("Alarms");
	if(alarmState != 0){
		gfx_set_cursor(12, 0);
		gfx_put_const_string(" ");
		gfx_put_string(gfx_number_to_char(1, alarm_selector+1));
	}
}

void alarmPage_ring(int i){
	enable_buzzer();
	alarms_remove(i);
	alarmState = 10;
	alarm_selector = i;
	_alarmPage_draw_frame();
	gfx_draw_button_text("Stop", "", "");
}

void alarmPage_init(){
	alarmState = 0;
	_alarmPage_draw_frame();
	gfx_draw_button_text("Mode", "Select", "+");
	
	for (char i = 0; i < 8; i++){
		gfx_set_cursor(7, i+2);
		if(i == alarm_selector){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(1,i+1));
		gfx_put_const_string(".");
		if (alarms_active() & (1<<i)){
			date_time_t alarmDiff = diff_date_time(alarm_get(i), get_g_date_time());
			if (alarmDiff.date.years < 0 || (alarmDiff.date.years == 0 && alarmDiff.date.months == 0 && alarmDiff.date.days == 0)){
				gfx_draw_time_layout(&alarm_get(i)->_time, 0xff, 0);
			}else{
				gfx_draw_short_date_layout(&alarm_get(i)->date, 0xff, 0);
			}
		}
	}
}

char map_state_to_layout(char state){
	switch(state-1){
		case 1: //year
			return 0b1000000;
		case 2: //month
			return 0b100000;
		case 3: //day
			return 0b11000;
		case 4: //hour
			return 0b100;
		case 5: //minute
			return 0b10;
		case 0: //init, has no mapping;
		default:
			return 0;
	}
}

void alarmPage_update(int* currentPage){
	if(alarmState == 10){ // ringing state
		if(on_button_down(0b100)){
			disable_buzzer();
			*currentPage = (int) MAINPAGE;
			mainPage_init();
		}
	}else if(alarmState == 0){
		if(on_button_down(0b001)){ //top button
			gfx_set_cursor(7, alarm_selector+2);
			gfx_put_string(gfx_number_to_char(1,alarm_selector+1));

			if(alarm_selector++ == 7){
				alarm_selector = 0;
			}
			gfx_set_cursor(7, alarm_selector+2);
			gfx_inverse_text();
			gfx_put_string(gfx_number_to_char(1,alarm_selector+1));

		}else if(on_button_down(0b010)){ //mid button
			alarmState = 1;
			_alarmPage_draw_frame();
			
			gfx_set_cursor(6, 4);
			if (alarms_active() & (1<<alarm_selector)){
				gfx_draw_button_text("Set", "Stop", "Back");
			}else{
				set_date_time(alarm_get(alarm_selector), get_g_date_time());
				alarm_get(alarm_selector)->_time.seconds = 0;
				gfx_draw_button_text("Set", "", "Back");
			}
			gfx_draw_layout(alarm_get(alarm_selector), 0xff, 0);
		}else if(on_button_down(0b100)){ // lower button
			*currentPage = (int) PEDOPAGE;
			pedoPage_init();
		}
	}else if(alarmState == 1){
		if(on_button_down(0b001)){ //top button
			alarmPage_init();
		}else if(on_button_down(0b010)){ //mid button
			alarms_remove(alarm_selector);
			*currentPage = (int) MAINPAGE;
			mainPage_init();
		}else if(on_button_down(0b100)){ // lower button
			gfx_draw_button_text("Ok ", "+   ", "-   ");
			alarms_remove(alarm_selector);
			alarmState = 2;
			gfx_draw_layout(alarm_get(alarm_selector), 0xff, 0b1000000);
		}
	}else if(alarmState >= 2){
		
		// Actual time changing
		if(on_button_down(0b001)){
			dec_what_date_time(1<<(alarmState-2), alarm_get(alarm_selector));
			char layout = map_state_to_layout(alarmState);
			gfx_draw_layout(alarm_get(alarm_selector), layout, layout);
		}
		if(on_button_down(0b010)){
			inc_what_date_time(1<<(alarmState-2), alarm_get(alarm_selector));
			char layout = map_state_to_layout(alarmState);
			gfx_draw_layout(alarm_get(alarm_selector), layout, layout);
		}
		if(on_button_down(0b100)){
			if(alarmState < 6){
				char layout = map_state_to_layout(alarmState);
				gfx_draw_layout(alarm_get(alarm_selector), layout, 0); //remove old highlight
				alarmState++;
				layout = map_state_to_layout(alarmState);
				gfx_draw_layout(alarm_get(alarm_selector), layout, layout);
			}else{
				alarms_add(alarm_selector);
				alarmState = 0;
				*currentPage = (int) MAINPAGE;
				mainPage_init();
			}
		}
	}
}