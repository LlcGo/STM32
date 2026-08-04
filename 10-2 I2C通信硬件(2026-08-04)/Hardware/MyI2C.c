#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);
}

void I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}	

uint8_t I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}

void MYI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_10);
}

void MYI2C_Start()
{
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	
	I2C_W_SDA(0);
	I2C_W_SCL(0);
}	

void MYI2C_Stop()
{
	I2C_W_SDA(0);
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}	

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
    {
		I2C_W_SDA(Byte & 0x80 >> i);
		I2C_W_SCL(0);
		I2C_W_SCL(1);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	I2C_W_SDA(1);
	for(i = 0; i < 8; i++)
    {
		I2C_W_SCL(1);
		if (I2C_R_SDA() == 0)
		{
			Byte |= (0x80 >> i);
		}
		I2C_W_SCL(0);
	}
	return Byte;
}


void Send_ACK(uint8_t AckBit)
{
	    I2C_W_SDA(AckBit);
		I2C_W_SCL(1);
		I2C_W_SCL(0);
}

uint8_t Rec_ACK()
{
	uint8_t AckBit;
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	AckBit = I2C_R_SDA();
	I2C_W_SCL(0);
	return AckBit;
}
