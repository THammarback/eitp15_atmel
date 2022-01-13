#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "mainPage.h"


void timerPage_init(){
	
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x0ff0);
	gfx_text_color(0xf00f);
	gfx_set_cursor(8, 0);
	gfx_put_const_string(5, "Timer");
	gfx_set_cursor(0, 3);
	gfx_put_const_string(4, "Mode");
}

void timerPage_update(int* currentPage){
	
	
	if(on_button_down(0b001)){ //top button

	}else if(on_button_down(0b010)){ //mid button
		
	}else if(on_button_down(0b100)){ // lower button
		*currentPage = (int) MAINPAGE;
		mainPage_init();
	}
}