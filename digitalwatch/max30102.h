/*
 * max30102.h
 *
 * Created: 2022-01-12 14:44:33
 *  Author: torha
 */ 


#ifndef MAX30102_H_
#define MAX30102_H_

#define MAX30102_ADDR 0xAE
#define FIFO_WPOINTER 0x04 //FIFO Write Pointer
#define FIFO_RPOINTER 0x06 //FIFO Read Pointer
#define FIFO_DR 0x07 //FIFO Read Pointer
#define FIFO_CONF 0x08 //FIFO Configuration
#define MODE_CONF 0x09 //Mode Configuration
#define SPO2_CONF 0x0A //SpO2 Configuration
#define LED1_PAMP 0x0C //LED1 Pulse Amplitude
#define LED2_PAMP 0x0D //LED2 Pulse Amplitude


#endif /* MAX30102_H_ */