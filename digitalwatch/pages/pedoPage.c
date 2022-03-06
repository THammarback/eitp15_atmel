#include "pages.h"
#include "../buttonHandler.h"
#include "../gfxLib.h"
#include "../timing.h"
#include "../mpu6050.h"
#include "mainPage.h"

void _display_mpu_data();

unsigned int pedo[7] = {0, 0, 0, 0, 0, 0, 0};
unsigned int last_pedo[7] = {0, 0, 0, 0, 0, 0, 0};
unsigned int get_pedo(int i){
	return pedo[i];
}
void clear_pedo(int i){
	pedo[i] = 0;
}

int last_day = 0;
int _weekday = 0;
char ready = 1;
void update_pedo(){
	mpu_data_t accel;
	mpu_get_accel_raw(&accel);
	
	int day = get_g_date()->days;
	if(day != last_day){
		_weekday = get_weekday(get_g_date());
		last_day = day;
	}
	
	int mag = accel.x*accel.x+accel.y*accel.y+accel.z*accel.z;
	if(ready && mag > 4){
		pedo[_weekday]++;
		ready = 0;
	}else if(!ready && mag <= 1){
		ready = 1;
	}
}


void pedoPage_init(){
	
	gfx_clear();
	gfx_draw_rect(0, 0, 127, 127, 0x8888);
	gfx_text_color(0x0f08);
	gfx_set_cursor(6, 0);
	gfx_put_const_string("Pedometer");
	gfx_draw_button_text("Mode", "", "");
	for (int i=0; i<7; i++){
		gfx_set_cursor(5, i+3);
		gfx_put_short_weekday(i);
		gfx_put_const_string(":");
		gfx_put_string(gfx_number_to_char(5,pedo[i]));	
	}
}

void _display_mpu_data(){
	mpu_data_t accel;
	mpu_get_accel_raw(&accel);

	gfx_set_cursor(5, 7);
	gfx_put_const_string("x: ");
	gfx_put_string(gfx_float_to_char(1, 3, accel.x));
	gfx_set_cursor(5, 8);
	gfx_put_const_string("y: ");
	gfx_put_string(gfx_float_to_char(1, 3, accel.y));
	gfx_set_cursor(5, 9);
	gfx_put_const_string("z: ");
	gfx_put_string(gfx_float_to_char(1, 3, accel.z));
}

void pedoPage_update(int* currentPage){
	if(last_pedo[_weekday] != pedo[_weekday]){
		gfx_set_cursor(5, _weekday+3);
		gfx_put_short_weekday(_weekday);
		gfx_put_const_string(":");
		gfx_put_string(gfx_number_to_char(5,pedo[_weekday]));
		last_pedo[_weekday] = pedo[_weekday];
	}

	if(on_button_down(0b100)){ // lower button
		*currentPage = (int) MAINPAGE;
		mainPage_init();
	}
}