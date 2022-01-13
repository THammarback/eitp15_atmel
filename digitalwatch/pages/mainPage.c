#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "setTimePage.h"
#include "../timing.h"
#include "../mpu6050.h"

void _draw_time();
void _display_mpu_data();

void mainPage_init(){
	
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0xf000);
	gfx_text_color(0x0ff0);
	gfx_set_cursor(8, 0);
	gfx_put_const_string(4, "Main");
	gfx_set_cursor(0, 3);
	gfx_put_const_string(4, "Mode");
	_draw_time();
}

void _draw_time(){
	gfx_set_cursor(6, 2);
	gfx_put_string(2, gfx_number_to_char(2, time_get_hours()));
	gfx_put_const_string(1, ":");
	gfx_put_string(2, gfx_number_to_char(2, time_get_minutes()));
	gfx_put_const_string(1, ":");
	gfx_put_string(2, gfx_number_to_char(2, time_get_seconds()));
	
	gfx_set_cursor(6, 3);
	gfx_put_weekday(time_get_weekday());
	
	gfx_set_cursor(5, 4);
	gfx_put_string(2, gfx_number_to_char(2, time_get_days()));
	gfx_put_const_string(1, " ");
	gfx_put_month(time_get_months());
	gfx_set_cursor(8, 5);
	gfx_put_string(4, gfx_number_to_char(4, time_get_years()));
	
}

void _display_mpu_data(){
	mpu_data_t accel;
	mpu_get_accel_raw(&accel);

	gfx_set_cursor(5, 7);
	gfx_put_const_string(3, "x: ");
	gfx_put_string(6, gfx_float_to_char(1, 3, accel.x));
	gfx_set_cursor(5, 8);
	gfx_put_const_string(3, "y: ");
	gfx_put_string(6, gfx_float_to_char(1, 3, accel.y));
	gfx_set_cursor(5, 9);
	gfx_put_const_string(3, "z: ");
	gfx_put_string(6, gfx_float_to_char(1, 3, accel.z));
	
}

int currentSeconds = 0;
int lastSeconds = 0;
void mainPage_update(int* currentPage){
	currentSeconds = time_get_seconds();
	if(currentSeconds =! lastSeconds){
		_draw_time();
		
		_display_mpu_data();
	}
	lastSeconds = currentSeconds;
	
	
	if(on_button_down(0b001)){ //top button			

	}else if(on_button_down(0b010)){ //mid button
			
	}else if(on_button_down(0b100)){ // lower button
		*currentPage = (int) SETTIMEPAGE;
		setTimePage_init();
	}
}