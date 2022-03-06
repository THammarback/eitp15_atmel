#include "../gfxLib.h"
#include "../buttonHandler.h"
#include "pages.h"
#include "mainPage.h"
#include "../timing.h"
#include "timerPage.h"

int changeWhat = 0;

void setTimePage_init(){
	changeWhat=0;
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x000f);
	
	gfx_draw_button_text("Mode", "Set", "");
	
	gfx_set_cursor(6, 0);
	gfx_put_const_string("Set Time");
	
	gfx_set_cursor(6, 6);
	gfx_draw_layout(get_g_date_time(), 0xff, 0);
}

char map_changeWhat_to_layout(int changeWhat){
	switch(changeWhat){
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

void setTimePage_update(int* currentPage){
	if(changeWhat == 0){//INIT, change nothing	
		gfx_draw_layout(get_g_date_time(), g_time_changed(), 0);
			
		if(on_button_down(0b001)){

		}
		if(on_button_down(0b010)){
			changeWhat = 1;
			char layout = map_changeWhat_to_layout(changeWhat);
			gfx_draw_button_text("ok  ", "+  ", "-");
			gfx_draw_layout(get_g_date_time(), layout, layout);
		}
		if(on_button_down(0b100)){
			*currentPage = (int) TIMERPAGE;
			timerPage_init();
		}
	}else{// Actual time changing
		if(on_button_down(0b001)){
			dec_what_date_time(1<<(changeWhat-1), get_g_date_time());
			char layout = map_changeWhat_to_layout(changeWhat);
			gfx_draw_layout(get_g_date_time(), layout, layout);
		}
		if(on_button_down(0b010)){
			inc_what_date_time(1<<(changeWhat-1), get_g_date_time());
			char layout = map_changeWhat_to_layout(changeWhat);
			gfx_draw_layout(get_g_date_time(), layout, layout);
		}
		if(on_button_down(0b100)){
			if(changeWhat < 5){
				char layout = map_changeWhat_to_layout(changeWhat);
				gfx_draw_layout(get_g_date_time(), layout, 0); //remove old highlight
				changeWhat++;
				layout = map_changeWhat_to_layout(changeWhat);
				gfx_draw_layout(get_g_date_time(), layout, layout);
			}else{
				get_g_time()->seconds = 0;
				*currentPage = (int) MAINPAGE;
				mainPage_init();	
			}
		}
	}
}