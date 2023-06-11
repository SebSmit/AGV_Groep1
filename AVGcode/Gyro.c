#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Gyro.h"
#define MPU6050_ADDR 0x68

volatile int Gyro_count = 0;
volatile float DS_1 = 0;
volatile float DS_2 = 0;

void I2C_Init() {
    // Set SCL frequency to 100kHz
    TWBR = 72;
}

void I2C_Start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_Stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void I2C_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t I2C_ReadACK() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_ReadNACK() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void MPU6050_WriteReg(uint8_t reg, uint8_t data) {
    I2C_Start();
    I2C_Write(MPU6050_ADDR << 1);
    I2C_Write(reg);
    I2C_Write(data);
    I2C_Stop();
}

int16_t MPU6050_ReadGyro(uint8_t reg) {
    int16_t value;
    I2C_Start();
    I2C_Write(MPU6050_ADDR << 1);
    I2C_Write(reg);
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0x01);
    value = (I2C_ReadACK() << 8) | I2C_ReadNACK();
    I2C_Stop();
    return value;
}

float Read_Gyro_X(void){
    int16_t GyroX = MPU6050_ReadGyro(0x43);
    GyroX += 197;
    float GyroX_degPerSec = (float)GyroX / 131.0;
    return GyroX_degPerSec;
}
float Read_Gyro_Y(void){
    int16_t GyroY = MPU6050_ReadGyro(0x45);
    float GyroY_degPerSec = (float)GyroY / 131.0;
    return GyroY_degPerSec;
}
float Read_Gyro_Z(void){
    int16_t GyroZ = MPU6050_ReadGyro(0x47);
    float GyroZ_degPerSec = (float)GyroZ / 131.0;
    return GyroZ_degPerSec;
}


void MPU6050_Init() {
    MPU6050_WriteReg(0x6B, 0x00); // Power on the MPU6050
}

void initTimerGyro(void){
    TCCR0A = 0;
    TCCR0B |= (1<<CS00);
    //16000000 / 256 = 62,500
    //1 / 62500 = 0.000016s = 0.016ms = 16us
    //62500x/sec
    TIMSK0 = (1<<TOIE0);
    sei();
}
ISR(TIMER0_OVF_vect){
    Gyro_count++;
}

float UpdateHeading(float CurrentHeading){

    float DeltaTijd = 0;
    float DeltaHeading;

    // Read gyroscopic rotation data
    DS_2 = Read_Gyro_X();
    //Timer uitlezen
    //16us * count is aantal s
    DeltaTijd = Gyro_count;

    //reset timer
    TCNT0 = 0;
    TIFR0 = 1<<TOV0;
    Gyro_count = 0;
    DeltaTijd = DeltaTijd * 0.000016; //DT nu in s
    DeltaHeading = ((DS_1 + DS_2)*(DeltaTijd))/2.0;
    CurrentHeading = CurrentHeading + DeltaHeading;
    DS_1 = DS_2;

	return CurrentHeading;
}

void init_Gyro(){
    I2C_Init();
    MPU6050_Init();
    initTimerGyro();
}
