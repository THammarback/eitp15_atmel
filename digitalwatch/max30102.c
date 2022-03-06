#include "twi_master.h"

static const uint8_t MAX30102_ADDR  = 0xAE;
static const uint8_t FIFO_WPOINTER  = 0x04; //FIFO Write Pointer
static const uint8_t FIFO_RPOINTER  = 0x06; //FIFO Read Pointer
static const uint8_t FIFO_DATA_REG	= 0x07; //FIFO data register
static const uint8_t FIFO_CONF		= 0x08; //FIFO Configuration
static const uint8_t MODE_CONF		= 0x09; //Mode Configuration
static const uint8_t SPO2_CONF		= 0x0A; //SpO2 Configuration
static const uint8_t LED1_PAMP		= 0x0C; //LED1 Pulse Amplitude
static const uint8_t LED2_PAMP		= 0x0D; //LED2 Pulse Amplitude
/*
void max_init(){
	ret_code_t retCode;
	uint8_t data[3];
	uint8_t read_mode = 0b00000111; //multi-LED mode, reading both heart frequency and SpO2
	uint8_t spo2_conf = 0b01000101; //ADC full-scale range 8192nA, 100samples/s, 16 bit resolution

	data = {MODE_CONF, read_mode, SPO2_CONF}
	retCode = tw_master_transmit(MAX30102_ADDR, data, 2, true);
	retCode = tw_master_transmit(MAX30102_ADDR, , 1, true);
	
	retCode = tw_master_transmit(MAX30102_ADDR, mode, 1, true);

	
	
}

void max_get_data(){
	uint8_t samplesPointer;
	ret_code_t retCode; 

	retCode = tw_master_transmit(MAX30102_ADDR, FIFO_WPOINTER, 1, true);
	retCode = tw_master_receive(MAX30102_ADDR, samplesPointer, 1);
	uint8_t samplesToRead = samplesPointer - FIFO_RPOINTER;
	
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
	retCode = tw_master_transmit(MAX30102_ADDR, FIFO_DATA_REG, 1, true);
	for (uint8_t i = 0; i < tw_master_receive; i++) {
		retCode = tw_master_receive(MAX30102_ADDR, data0, 1);
		retCode = tw_master_receive(MAX30102_ADDR, data1, 1);
		retCode = tw_master_receive(MAX30102_ADDR, data2, 1);
	}	
}
*/