#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "uart.h"

void _render(int length, char *data){
	cli();
	for(int i=0; i<length; i++){
		USART_Transmit(data[i]);
	}
	if(USART_Receive() != 0x06){
		//ERROR the screen has not received a correct command.
		PORTB |= (1<<PORTB5); //Note B5, B6 and B7 is not connected and only used for debugging
	}else{
		PORTB |= (1<<PORTB6);
	}
	sei();
	PORTB &= ~((1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5));
}

char* gfx_number_to_char(int n, int number){ // function found on stackoverflow
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10){
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}

char* gfx_float_to_char(int n_i, int n_f, float number){
	char* str_float = calloc(n_i + n_f + 2, sizeof(char)); // array to hold the result
	char dot = '.';

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
	_render(6, (char[]) {
		0xFF, 0xE4,
		((y & 0xFF00) >>8), (y & 0x00FF),
		((x & 0xFF00) >>8), (x & 0x00FF)
	});
}

void gfx_put_string(const int length, char* characters){
	char command[length+3];
	command[0] = 0x00; //cmd
	command[1] = 0x06; //cmd

	for (int i=0; i<length; i++){
		command[i+2] = characters[i];
	}
	command[length+2] = 0x00; //end with null
	
	_render(length+3, command);
}

void gfx_put_const_string(const int length, const char* characters){
	char command[length+3];
	command[0] = 0x00; //cmd
	command[1] = 0x06; //cmd

	for (int i=0; i<length; i++){
		command[i+2] = characters[i];
	}
	command[length+2] = 0x00; //end with null
	
	_render(length+3, command);
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
			gfx_put_const_string(9, "Sunday   ");
			break;
		case 1:
			gfx_put_const_string(9, "Monday   ");
			break;
		case 2:
			gfx_put_const_string(9, "Tuesday  ");
			break;
		case 3:
			gfx_put_const_string(9, "Wednesday");
			break;
		case 4:
			gfx_put_const_string(9, "Thursday ");
			break;
		case 5:
			gfx_put_const_string(9, "Friday   ");
			break;
		case 6:
			gfx_put_const_string(9, "Saturday ");
			break;
	}
}

void gfx_put_month(int month){
	if(month == 1){
		gfx_put_const_string(9, "January  ");
	} else if(month == 2){
		gfx_put_const_string(9, "February ");
	} else if(month == 3){
		gfx_put_const_string(9, "March    ");
	} else if(month == 4){
		gfx_put_const_string(9, "April    ");
	} else if(month == 5){
		gfx_put_const_string(9, "May      ");
	} else if(month == 6){
		gfx_put_const_string(9, "June     ");
	} else if(month == 7){
		gfx_put_const_string(9, "July     ");
	} else if(month == 8){
		gfx_put_const_string(9, "August   ");
	} else if(month == 9){
		gfx_put_const_string(9, "September");
	} else if(month == 10){
		gfx_put_const_string(9, "October  ");
	} else if(month == 11){
		gfx_put_const_string(9, "November ");
	} else if(month == 12){
		gfx_put_const_string(9, "December ");
	}
}