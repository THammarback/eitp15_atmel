#include <avr/io.h>
#define BAUD_9600 103
#define BAUD_250k 3

unsigned char USART_Receive(void) {
	/* Wait for data to be received */
	unsigned int i=0;
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit(const unsigned char data){	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	
	UDR0 = data;
}
void USART_Init() {
	/* Set baud rate */
	UBRR0 = (int) BAUD_9600;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}
