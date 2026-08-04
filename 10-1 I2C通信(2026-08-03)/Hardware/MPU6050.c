#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	MYI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	Rec_ACK();
	MyI2C_SendByte(RegAddress);
	Rec_ACK();
    MyI2C_SendByte(Data);
	Rec_ACK();
	MYI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MYI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	Rec_ACK();
	MyI2C_SendByte(RegAddress);
	Rec_ACK();
    
	MYI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	Rec_ACK();
	Data = MyI2C_ReceiveByte();
	Send_ACK(1);
	MYI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MYI2C_Init();
}


