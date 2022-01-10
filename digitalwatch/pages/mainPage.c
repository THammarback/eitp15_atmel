#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "setTimePage.h"
#include "../timing.h"

void _draw_time();

void mainPage_init(){
	char str_main[] = "Main";
	
	gfx_clear();
	gfx_text_color(0x0ff0);
	gfx_set_cursor(0, 8);
	gfx_put_string(4, str_main);
	_draw_time();
}

void _draw_time(){
	char str_colon[] = ":";
	
	gfx_set_cursor(1, 6);
	gfx_put_string(2, gfx_number_to_char(2, time_get_hours()));
	gfx_put_string(1, str_colon);
	gfx_put_string(2, gfx_number_to_char(2, time_get_minutes()));
	gfx_put_string(1, str_colon);
	gfx_put_string(2, gfx_number_to_char(2, time_get_seconds()));
	
	gfx_set_cursor(2, 6);
	//gfx_put_month(time_get_months());	
}

int currentSeconds = 0;
int lastSeconds = 0;
void mainPage_update(int* currentPage){
	currentSeconds = time_get_seconds();
	if(currentSeconds =! lastSeconds){
		_draw_time();
	}
	lastSeconds = currentSeconds;
	
	
	if(on_button_down(0b001)){ //top button
		gfx_clear();
		gfx_draw_rect(10, 10, 118, 118, 0x0ff0);			
				
	}else if(on_button_down(0b010)){ //mid button
		char hello[] = "Something\ndifferent";

		gfx_clear();
		gfx_set_cursor(2, 2);
		gfx_text_color(0x0ff0);
		gfx_put_string(19, hello);
		gfx_draw_rect(9, 9, 119, 119, 0xf000);			
	}
	else if(on_button_down(0b100)){ // lower button
		*currentPage = (int) SETTIMEPAGE;
		setTimePage_init();
	}
}