#ifndef GFXLIB_H_
#define GFXLIB_H_
#include "timing.h"

typedef struct pos_type {
	int x;
	int y;
} pos_t;

char* gfx_number_to_char(int n, int number);
char* gfx_float_to_char(int n_i, int n_f, float number);

void gfx_draw_layout(date_time_t* date_time, char whatParts, char highlight);
void gfx_draw_date_layout(date_t* date, char whatParts, char highlight);
void gfx_draw_short_date_layout(date_t* date, char whatParts, char highlight);
void gfx_draw_time_layout(time_t* _time, char whatParts, char highlight);
void gfx_draw_button_text(const char btn0[], const char btn1[], const char btn2[]);

void gfx_inverse_text();
void gfx_clear();
void gfx_text_color(int color);
void gfx_set_cursor(int x, int y);
void gfx_put_string(char* characters);
void gfx_put_const_string(const char* characters);
void gfx_put_month(int month);
void gfx_put_short_month(int month);
void gfx_put_weekday(int weekday);
void gfx_put_short_weekday(int weekday);
void gfx_draw_rect(int x1, int y1, int x2, int y2, int color);


#endif /* GFXLIB_H_ */