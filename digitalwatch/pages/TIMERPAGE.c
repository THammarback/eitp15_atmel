#include <avr/io.h>
#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "../timing.h"
#include "mainPage.h"
#include "alarmPage.h"

char timerState = 0;
int selector = 0;

void _timerPage_draw_frame(){
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x0ff0);
	gfx_text_color(0xf00f);
	gfx_set_cursor(7, 0);
	gfx_put_const_string("Timers");
	if(timerState != 0){
		gfx_set_cursor(12, 0);
		gfx_put_const_string(" ");
		gfx_put_string(gfx_number_to_char(1, selector+1));
	}
}

void _inc_time(){
	switch (timerState){
		case 2:
			inc_seconds(timer_get(selector));
			break;
		case 3:
			inc_minutes(timer_get(selector));
			break;
		case 4:
			inc_hours(timer_get(selector));
			break;
	}
}
void _dec_time(){
	switch (timerState){
		case 2:
			dec_seconds(timer_get(selector));
			break;
		case 3:
			dec_minutes(timer_get(selector));
			break;
		case 4:
			dec_hours(timer_get(selector));
			break;
	}
}

void timerPage_ring(int i){
	enable_buzzer();
	timerState = 10;
	selector = i;
	_timerPage_draw_frame();
	gfx_draw_button_text("Stop", "", "");
}

void timerPage_init(){
	timerState = 0;
	_timerPage_draw_frame();
	gfx_draw_button_text("Mode", "Select", "+");
	
	for (char i = 0; i < 8; i++){
		gfx_set_cursor(7, i+2);
		if(i == selector){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(1,i+1));
		gfx_put_const_string(".");
		if (timers_active() & (1<<i)){
			gfx_draw_time_layout(timer_get(i), 0xff, 0);
		}
	}
}


void timerPage_update(int* currentPage){
	if(timerState == 10){ // ringing state
		if(on_button_down(0b100)){
			disable_buzzer();
			*currentPage = (int) MAINPAGE;
			mainPage_init();
		}
	}else if(timerState == 0){
		for (char i = 0; i < 8; i++){
			if (timers_active() & (1<<i)){
				char change = what_time_changed(timer_get(i), timer_get_last(i));
				if(change){
					gfx_set_cursor(9, i+2);
					gfx_draw_time_layout(timer_get(i), change,  0);
				}
			}
		}
		
		if(on_button_down(0b001)){ //top button
			gfx_set_cursor(7, selector+2);
			gfx_put_string(gfx_number_to_char(1,selector+1));

			if(selector++ == 7){
				selector = 0;
			}
			gfx_set_cursor(7, selector+2);
			gfx_inverse_text();
			gfx_put_string(gfx_number_to_char(1,selector+1));

		}else if(on_button_down(0b010)){ //mid button
			timerState = 1;
			_timerPage_draw_frame();
			
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), 0xff, 0);
			if (timers_active() & (1<<selector)){
				gfx_draw_button_text("Set", "Stop", "Back");
			}else{
				gfx_draw_button_text("Set", "", "Back");
			}
		}else if(on_button_down(0b100)){ // lower button
			*currentPage = (int) ALARMPAGE;
			alarmPage_init();
		}
	}else if(timerState == 1){
		char change = what_time_changed(timer_get(selector), timer_get_last(selector));
		if(change){
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), change, 0);
		}
		
		if(on_button_down(0b001)){ //top button
			timerPage_init();
		}else if(on_button_down(0b010)){ //mid button
			timers_remove(selector);
			*currentPage = (int) MAINPAGE;
			mainPage_init();
		}else if(on_button_down(0b100)){ // lower button
			gfx_draw_button_text("Ok ", "+   ", "-   ");
			timers_remove(selector);
			timerState = 2;
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), 1, 1);
		}
	}else if(timerState >= 2){
		if(on_button_down(0b001)){ //top button
			_dec_time();
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), (1<<(timerState-2)), (1<<(timerState-2)));
		}else if(on_button_down(0b010)){ //mid button
			_inc_time();
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), (1<<(timerState-2)), (1<<(timerState-2)));
		}else if(on_button_down(0b100)){ // lower button
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), (1<<(timerState-2)), 0);
			timerState++;
			gfx_set_cursor(6, 4);
			gfx_draw_time_layout(timer_get(selector), (1<<(timerState-2)), (1<<(timerState-2)));
			if(timerState >= 5){
				timers_add(selector);
				timerState = 0;
				*currentPage = (int) MAINPAGE;
				mainPage_init();
			}
		}	
	}
}