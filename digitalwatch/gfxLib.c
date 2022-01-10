#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

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

char* gfx_number_to_char(int n, int number){ // function found on stackoverflow
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10){
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
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
		((x & 0xFF00) >>8), (x & 0x00FF),
		((y & 0xFF00) >>8), (y & 0x00FF)
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

void gfx_put_month(int month){
	char const *str_months[12] = {
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};
	
	if(month == 1){
		gfx_put_string(7, *str_months[0]);
	} else if(month == 2){
		gfx_put_string(8, *str_months[1]);
	} else if(month == 3){
		gfx_put_string(5, *str_months[2]);
	} else if(month == 4){
		gfx_put_string(5, *str_months[3]);
	} else if(month == 5){
		gfx_put_string(3, *str_months[4]);
	} else if(month == 6){
		gfx_put_string(4, *str_months[5]);
	} else if(month == 7){
		gfx_put_string(4, *str_months[6]);
	} else if(month == 8){
		gfx_put_string(6, *str_months[7]);
	} else if(month == 9){
		gfx_put_string(9, *str_months[8]);
	} else if(month == 10){
		gfx_put_string(7, *str_months[9]);
	} else if(month == 11){
		gfx_put_string(8, *str_months[10]);
	} else if(month == 12){
		gfx_put_string(8, *str_months[11]);
	}
}