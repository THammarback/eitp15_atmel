#include <avr/io.h>

// buttons actual port PINA5, PINA6 and PINA7
char buttons = 0; //button 1 on 001, button 2 on 010 and so on
char prevButtons = 0;

char on_button_down(char mask){
	return ((~prevButtons & buttons) & mask);
}

char on_button_up(char mask){
	return ((prevButtons & ~buttons) & mask);
}

void update_buttons(){
	prevButtons = buttons;
	buttons = (PINA>>5);
}