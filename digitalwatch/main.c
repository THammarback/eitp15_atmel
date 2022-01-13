/*
 * main.c
 *
 * Created: 12/21/2021 5:09:28 PM
 * Author: Tor Hammarbäck
 */ 

#define F_CPU 16000000UL

#define thermometer DDRA0
#define buzzer DDRA1

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "buttonHandler.h"
#include "timing.h"
#include "twi_master.h"
#include "mpu6050.h"

#include "pages/pages.h"
#include "pages/mainPage.h"
#include "pages/setTimePage.h"
#include "pages/timerPage.h"

void update_page(int *currentPage);

int main (void)
{
	tw_init(TW_FREQ_400K, true); // set I2C Frequency, enable internal pull-up
	mpu_init();
	_delay_ms(3000); //wait for screen to start
	USART_Init();
	clock_init();	
	
	int currentPage = 0;
	
	mainPage_init();
	
	DDRA |= (1<<buzzer);
	DDRB |= (1<<DDRB7)|(1<<DDRB6)|(1<<DDRB5); // only for debug, not connected on the actual board.
	
	while(1){
		update_page(&currentPage);
		
		update_buttons();
	}
}

void update_page(int *currentPage){
	switch(*currentPage){
		case MAINPAGE:
			mainPage_update(currentPage);
			break;
		case SETTIMEPAGE:
			setTimePage_update(currentPage);
			break;
		case TIMERPAGE:
			timerPage_update(currentPage);
			break;
	}
}