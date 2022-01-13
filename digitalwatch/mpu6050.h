#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDR	0x68
/* MPU6050 register address */
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define PWR_MGMT_1		0x6B

typedef struct
{
	float x;
	float y;
	float z;
} mpu_data_t;

void mpu_init();
void mpu_get_accel_raw(mpu_data_t* mpu_data);


#endif /* MPU6050_H_ */