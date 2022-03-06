#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "uart.h"
#include "gfxLib.h"
#include "timing.h"

pos_t cursor = {0, 0};

void _render(int length, char *data){
	for(int i=0; i<length; i++){
		USART_Transmit(data[i]);
	}
	if(USART_Receive() != 0x06){
		//ERROR the screen has not received a correct command.
		PORTB |= (1<<PORTB5); //Note B5, B6 and B7 is not connected and only used for debugging
	}else{
		PORTB |= (1<<PORTB6);
	}
	PORTB &= ~((1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5));
}

void gfx_draw_button_text(const char btn0[], const char btn1[], const char btn2[]){
	gfx_set_cursor(0, 3);
	gfx_put_const_string(btn0);
	gfx_set_cursor(0, 6);
	gfx_put_const_string(btn1);
	gfx_set_cursor(0, 9);
	gfx_put_const_string(btn2);
}

void gfx_draw_layout(date_time_t* date_time, char whatParts, char highlight){
	if(whatParts & 0b111){
		gfx_set_cursor(6, 6);
		gfx_draw_time_layout(&date_time->_time, whatParts, highlight);
	}
	if(whatParts){
		gfx_draw_date_layout(&date_time->date, whatParts, highlight);
	}
}

void gfx_draw_time_layout(time_t* _time, char whatParts, char highlight){//whatParts -> from left to right bits: Year, month, day, weekday, hours, minutes, seconds
	if(whatParts >= 0b111){
		if(highlight & 0b100){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(2, _time->hours));
		gfx_put_const_string(":");
		if(highlight & 0b010){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(2, _time->minutes));
		gfx_put_const_string(":");
		if(highlight & 0b001){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(2, _time->seconds));
	}else{
		if(whatParts & 0b100){
			if(highlight & 0b100){
				gfx_inverse_text();
			}
			gfx_put_string(gfx_number_to_char(2, _time->hours));
			cursor.x -= -2;
		}
		if(whatParts & 0b10){
			gfx_set_cursor(cursor.x+3, cursor.y);
			if(highlight & 0b010){
				gfx_inverse_text();
			}
			gfx_put_string(gfx_number_to_char(2, _time->minutes));
			cursor.x -= 3;
		}
		if(whatParts & 0b1){
			gfx_set_cursor(cursor.x+6, cursor.y);
			if(highlight & 0b001){
				gfx_inverse_text();
			}
			gfx_put_string(gfx_number_to_char(2, _time->seconds));
		}
	}
}

void gfx_draw_short_date_layout(date_t* date, char whatParts, char highlight){
	if(whatParts & 0b10000){//day
		if(highlight & 0b10000){
			gfx_inverse_text();
		}
		gfx_put_string(gfx_number_to_char(2, date->days));
	}
	if(whatParts & 0b110000){//month and day
		gfx_put_const_string(" ");
	}
	
	if(whatParts & 0b100000){//month
		if(!(whatParts & 0b10000)){
			gfx_set_cursor(cursor.x+3, cursor.y);
		}
		if(highlight & 0b10000){
			gfx_inverse_text();
		}
		gfx_put_short_month(date->months);
	}
}


void gfx_draw_date_layout(date_t* date, char whatParts, char highlight){ //whatParts -> from left to right bits: Year, month, day, weekday, hours, minutes, seconds
	if(whatParts & 0b1000){ //weekday
		if(highlight & 0b1000){
			gfx_inverse_text();
		}
		gfx_set_cursor(6, 3);
		gfx_put_weekday(get_weekday(date));
	}
	if(whatParts & 0b10000){//day
		if(highlight & 0b10000){
			gfx_inverse_text();
		}
		gfx_set_cursor(5, 4);
		gfx_put_string(gfx_number_to_char(2, date->days));
	}
	if(whatParts & 0b100000){//month
		if(highlight & 0b100000){
			gfx_inverse_text();
		}
		gfx_set_cursor(8, 4);
		gfx_put_month(date->months);
	}
	if(whatParts & 0b1000000){//year
		if(highlight & 0b1000000){
			gfx_inverse_text();
		}
		gfx_set_cursor(8, 5);
		gfx_put_string(gfx_number_to_char(4, date->years));
	}
}

void gfx_inverse_text(){
	_render(4, (char[]) {0xFF, 0x74, 0x00, 0x01});
}

char numberArray[10];
char* gfx_number_to_char(int n, int number){ 
    int i;
	for (i = 0; i<10; i++){
		numberArray[i] = 0x00;
	}
    for (i = n-1; i >= 0; --i, number /= 10){
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}

char str_float[10];
char* gfx_float_to_char(int n_i, int n_f, float number){
	char dot = '.';
	for (int i = 0; i<10; i++){
		numberArray[i] = 0x00;
	}

	if(number<0){
		number = -number;
		str_float[0] = '-';
	}else{
		str_float[0] = '+';
	}

	int integerPart = (int) number;
	int floatPart = (int) ((number-integerPart) * pow(10, n_f));
	
	memcpy(str_float + 1, gfx_number_to_char(n_i, integerPart), n_i * sizeof(char));
	memcpy(str_float + n_i + 1, &dot, sizeof(char));
	memcpy(str_float + n_i + 2, gfx_number_to_char(n_f, floatPart), n_f * sizeof(char));
	
	return str_float;

}

void gfx_clear(){
	_render(2, (char[]) {0xFF, 0xD7});
}

void gfx_text_color(int color){
	_render(4, (char[]) {
		0xFF, 0x7F,
		((color & 0xFF00) >>8), (color & 0x00FF)
	});
}

void gfx_set_cursor(int x, int y){
	cursor = ((pos_t) {x, y});
	_render(6, (char[]) {
		0xFF, 0xE4,
		((y & 0xFF00) >>8), (y & 0x00FF),
		((x & 0xFF00) >>8), (x & 0x00FF)
	});
}

void gfx_put_string(char* characters){
	int length = 0;
	while(characters[length++]);
	if(length > 1){
		char command[length+2];
		command[0] = 0x00; //cmd
		command[1] = 0x06; //cmd

		for (int i=0; i<length; i++){
			command[i+2] = characters[i];
		}
	
		_render(length+2, command);
	}
}

void gfx_put_const_string(const char* characters){
	int length = 0;
	while(characters[length++]);
	if(length > 1){
		char command[length+2];
		command[0] = 0x00; //cmd
		command[1] = 0x06; //cmd

		for (int i=0; i<length; i++){
			command[i+2] = characters[i];
		}
		_render(length+2, command);
	}
}

void gfx_draw_rect(int x1, int y1, int x2, int y2, int color){
	_render(12, (char[]) {
		0xFF, 0xCF,
		((x1 & 0xFF00) >>8), (x1 & 0x00FF),
		((y1 & 0xFF00) >>8), (y1 & 0x00FF),
		((x2 & 0xFF00) >>8), (x2 & 0x00FF),
		((y2 & 0xFF00) >>8), (y2 & 0x00FF),
		((color & 0xFF00) >>8), (color & 0x00FF)
	});
}

void gfx_put_weekday(int weekday){
	switch(weekday){
		case 0:
			gfx_put_const_string("Sunday   ");
			break;
		case 1:
			gfx_put_const_string("Monday   ");
			break;
		case 2:
			gfx_put_const_string("Tuesday  ");
			break;
		case 3:
			gfx_put_const_string("Wednesday");
			break;
		case 4:
			gfx_put_const_string("Thursday ");
			break;
		case 5:
			gfx_put_const_string("Friday   ");
			break;
		case 6:
			gfx_put_const_string("Saturday ");
			break;
	}
}


void gfx_put_short_weekday(int weekday){
	switch(weekday){
		case 0:
			gfx_put_const_string("Sun");
			break;
		case 1:
			gfx_put_const_string("Mon");
			break;
		case 2:
			gfx_put_const_string("Tue");
			break;
		case 3:
			gfx_put_const_string("Wed");
			break;
		case 4:
			gfx_put_const_string("Thu");
			break;
		case 5:
			gfx_put_const_string("Fri");
			break;
		case 6:
			gfx_put_const_string("Sat");
			break;
	}
}

void gfx_put_month(int month){
	if(month == 1){
		gfx_put_const_string("January  ");
	} else if(month == 2){
		gfx_put_const_string("February ");
	} else if(month == 3){
		gfx_put_const_string("March    ");
	} else if(month == 4){
		gfx_put_const_string("April    ");
	} else if(month == 5){
		gfx_put_const_string("May      ");
	} else if(month == 6){
		gfx_put_const_string("June     ");
	} else if(month == 7){
		gfx_put_const_string("July     ");
	} else if(month == 8){
		gfx_put_const_string("August   ");
	} else if(month == 9){
		gfx_put_const_string("September");
	} else if(month == 10){
		gfx_put_const_string("October  ");
	} else if(month == 11){
		gfx_put_const_string("November ");
	} else if(month == 12){
		gfx_put_const_string("December ");
	}
}



void gfx_put_short_month(int month){
	if(month == 1){
		gfx_put_const_string("Jan");
	} else if(month == 2){
		gfx_put_const_string("Feb");
	} else if(month == 3){
		gfx_put_const_string("Mar");
	} else if(month == 4){
		gfx_put_const_string("Apr");
	} else if(month == 5){
		gfx_put_const_string("May");
	} else if(month == 6){
		gfx_put_const_string("Jun");
	} else if(month == 7){
		gfx_put_const_string("Jul");
	} else if(month == 8){
		gfx_put_const_string("Aug");
	} else if(month == 9){
		gfx_put_const_string("Sep");
	} else if(month == 10){
		gfx_put_const_string("Oct");
	} else if(month == 11){
		gfx_put_const_string("Nov");
	} else if(month == 12){
		gfx_put_const_string("Dec");
	}
}