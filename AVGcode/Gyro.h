#ifndef _GYRO_H_
#define _GYRO_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define MPU6050_ADDR 0x68

void I2C_Init();
void I2C_Start();
void I2C_Stop();
void I2C_Write(uint8_t data);
uint8_t I2C_ReadACK();
uint8_t I2C_ReadNACK();
void MPU6050_WriteReg(uint8_t reg, uint8_t data);
int16_t MPU6050_ReadGyro(uint8_t reg);
float Read_Gyro_X();
float Read_Gyro_Y();
float Read_Gyro_Z();
void MPU6050_Init();
void initTimerGyro();
void init_Gyro();
float UpdateHeading(float CurrentHeading);

#endif // _GYRO_H_
