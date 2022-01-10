#include "../gfxLib.h"
#include "../buttonHandler.h"
#include "pages.h"
#include "mainPage.h"

void setTimePage_init(){
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0xffff);
}

void setTimePage_update(int* currentPage){
	if(on_button_down(0b001)){
		gfx_clear();
		gfx_draw_rect(9, 9, 117, 117, 0x0ff0);
	}
	if(on_button_down(0b010)){
		gfx_clear();
		gfx_draw_rect(10, 10, 118, 118, 0xf000);
	}
	if(on_button_down(0b100)){
		*currentPage = (int) MAINPAGE;
		mainPage_init();
	}

}