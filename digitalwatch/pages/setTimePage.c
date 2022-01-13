#include "../gfxLib.h"
#include "../buttonHandler.h"
#include "pages.h"
#include "mainPage.h"
#include "../timing.h"
#include "timerPage.h"

int changeWhat; //0=year, 1=month, 2=day, 3=hour, 4=minute, 5=seconds, 10=nothing (Init mode)

void _draw_frame();
void _display_time();
void _display_button_text();
void _add_time();
void _sub_time();

void setTimePage_init(){
	changeWhat=10;
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x000f);
	
	gfx_set_cursor(0, 9);
	gfx_put_const_string(4, "Back");
	gfx_set_cursor(0, 6);
	gfx_put_const_string(3, "Set");
	gfx_set_cursor(0, 3);
	gfx_put_const_string(4, "Mode");
	
	gfx_set_cursor(6, 0);
	gfx_put_const_string(10, "Set Time");
	
	_display_time();
}

void _draw_frame(){
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x000f);
	_display_button_text();
}

void _display_time(){
	if (changeWhat == 4){
		gfx_set_cursor(5, 0);
		gfx_put_const_string(10, "Set minute");
	}else if(changeWhat == 3){
		gfx_set_cursor(6, 0);
		gfx_put_const_string(8, "Set hour");
	}else if(changeWhat == 2){
		gfx_set_cursor(6, 0);
		gfx_put_const_string(7, "Set day");
	}else if(changeWhat == 1){
		gfx_set_cursor(5, 0);
		gfx_put_const_string(9, "Set month");
	}else if(changeWhat == 0){
		gfx_set_cursor(6, 0);
		gfx_put_const_string(8, "Set year");
	}
	
	switch(changeWhat){
		case 10:
		case 5:
		gfx_set_cursor(11, 2);
		gfx_put_const_string(1, ":");
		gfx_put_string(2, gfx_number_to_char(2, time_get_seconds()));
		case 4://minute
		gfx_set_cursor(8, 2);
		gfx_put_const_string(1, ":");
		gfx_put_string(2, gfx_number_to_char(2, time_get_minutes()));
		
		case 3://hour
		gfx_set_cursor(6, 2);
		gfx_put_string(2, gfx_number_to_char(2, time_get_hours()));
		
		case 2://day
		gfx_set_cursor(6, 3);
		gfx_put_weekday(time_get_weekday());
		gfx_set_cursor(5, 4);
		gfx_put_string(2, gfx_number_to_char(2, time_get_days()));

		case 1://month
		gfx_set_cursor(8, 4);
		gfx_put_month(time_get_months());
		
		case 0://year
		gfx_set_cursor(8, 5);
		gfx_put_string(4, gfx_number_to_char(4, time_get_years()));
	}
}

void _add_time(){
	int temp;
	if (changeWhat == 4){//minutes
		temp = time_get_minutes();
		if(temp == 59){
			time_set_minutes(0);
		}else{
			time_set_minutes(temp+1);
		}
	}else if(changeWhat == 3){//hours
		temp = time_get_hours();
		if(temp == 23){
			time_set_hours(0);
		}else{
			time_set_hours(temp+1);
		}
	}else if(changeWhat == 2){//days
		temp = time_get_days();
		if(temp == days_in_month(time_get_months())){
			time_set_days(1);
		}else{
			time_set_days(temp+1);
		}
	}else if(changeWhat == 1){//months
		temp = time_get_months();
		if(temp == 12){
			time_set_months(1);
		}else{
			time_set_months(temp+1);
		}
	}else if(changeWhat == 0){//year
		temp = time_get_years();
		if(temp == 9999){
			time_set_years(0);
		}else{
			time_set_years(temp+1);
		}
	}
}

void _sub_time(){
	int temp;
	if (changeWhat == 4){//minutes
		temp = time_get_minutes();
		if(temp == 0){
			time_set_minutes(59);
		}else{
			time_set_minutes(temp-1);
		}
	}else if(changeWhat == 3){//hours
		temp = time_get_hours();
		if(temp == 0){
			time_set_hours(23);
		}else{
			time_set_hours(temp-1);
		}
	}else if(changeWhat == 2){//days
		temp = time_get_days();
		if(temp == 1){
			time_set_days(days_in_month(time_get_months()));
		}else{
			time_set_days(temp-1);
		}
	}else if(changeWhat == 1){//months
		temp = time_get_months();
		if(temp == 1){
			time_set_months(12);
		}else{
			time_set_months(temp-1);
		}
	}else if(changeWhat == 0){//year
		temp = time_get_years();
		if(temp == 0){
			time_set_years(9999);
		}else{
			time_set_years(temp-1);
		}
	}
}

void _display_button_text(){
	gfx_set_cursor(0, 3);
	gfx_put_const_string(2, "Ok");
	gfx_set_cursor(0, 6);
	gfx_put_const_string(1, "+");
	gfx_set_cursor(0, 9);
	gfx_put_const_string(1, "-");
}

void setTimePage_update(int* currentPage){
	if(changeWhat == 10){//INIT, change nothing
		if(on_button_down(0b001)){
			*currentPage = (int) MAINPAGE;
			mainPage_init();
		}
		if(on_button_down(0b010)){
			changeWhat = 0;
			_draw_frame();
			_display_time();
		}
		if(on_button_down(0b100)){
			*currentPage = (int) TIMERPAGE;
			timerPage_init();
		}
	}else{// Actual time changing
		if(on_button_down(0b001)){
			_sub_time();
			_display_time();
		}
		if(on_button_down(0b010)){
			_add_time();
			_display_time();
		}
		if(on_button_down(0b100)){
			if(changeWhat < 4){
				changeWhat++;
				_draw_frame();
				_display_time();
			}else{
				time_set_seconds(0);
				*currentPage = (int) MAINPAGE;
				mainPage_init();	
			}
		}
	}
}