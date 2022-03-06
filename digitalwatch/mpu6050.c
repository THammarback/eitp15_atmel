#include "twi_master.h"
#include "mpu6050.h"

void ERROR_CHECK(ret_code_t error_code){
	if (error_code != SUCCESS){
		//Not sure how to handle this yet
	}
}

void mpu_init(){
	ret_code_t error_code;
	//Write 0 to PWR_MGMT_1 reg to wakeup MPU
	uint8_t data[2] = {PWR_MGMT_1, 0};
	error_code = tw_master_transmit(MPU6050_ADDR, data, sizeof(data), false);
	ERROR_CHECK(error_code);
}

void mpu_get_accel_raw(mpu_data_t* mpu_data){
	ret_code_t error_code;
	/* 2 registers for each of accel x, y and z data */
	uint8_t data[6];
	
	data[0] = ACCEL_XOUT_H;
	error_code = tw_master_transmit(MPU6050_ADDR, data, 1, true);
	ERROR_CHECK(error_code);	
	error_code = tw_master_receive(MPU6050_ADDR, data, sizeof(data));
	ERROR_CHECK(error_code);
	
	/* Default accel config +/- 2g */
	mpu_data->x = (int16_t)(data[0] << 8 | data[1]) / 16384.0;
	mpu_data->y = (int16_t)(data[2] << 8 | data[3]) / 16384.0;
	mpu_data->z = (int16_t)(data[4] << 8 | data[5]) / 16384.0;
}