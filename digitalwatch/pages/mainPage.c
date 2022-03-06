#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "setTimePage.h"
#include "../timing.h"
#include "../mpu6050.h"

void mainPage_init(){
	
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0xf000);
	gfx_text_color(0x0ff0);
	gfx_set_cursor(8, 0);
	gfx_put_const_string("Main");
	gfx_draw_button_text("Mode", "Theme", "");
	
	gfx_draw_layout(get_g_date_time(), 0xff, 0);
}

void mainPage_update(int* currentPage){
	gfx_draw_layout(get_g_date_time(), g_time_changed(), 0);
	
	if(on_button_down(0b001)){ //top button			
		disable_buzzer();
	}else if(on_button_down(0b010)){ //mid button
		enable_buzzer();
	}else if(on_button_down(0b100)){ // lower button		
		*currentPage = (int) SETTIMEPAGE;
		setTimePage_init();
	}
}