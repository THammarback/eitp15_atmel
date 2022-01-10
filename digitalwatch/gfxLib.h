#ifndef GFXLIB_H_
#define GFXLIB_H_


void _render(int length, const char *data);

char* gfx_number_to_char(int n, int number);

void gfx_clear();
void gfx_text_color(int color);
void gfx_set_cursor(int x, int y);
void gfx_put_string(int length, char* characters);
void gfx_draw_rect(int x1, int y1, int x2, int y2, int color);


#endif /* GFXLIB_H_ */