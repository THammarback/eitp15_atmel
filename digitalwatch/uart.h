#ifndef uart_H_
#define uart_H_

void USART_Init();
unsigned char USART_Receive(void);
void USART_Transmit(const unsigned char data);

#endif /* uart_H_ */