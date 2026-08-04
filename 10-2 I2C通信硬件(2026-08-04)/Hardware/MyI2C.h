#ifndef __MYI2C_H
#define __MYI2C_H

void I2C_W_SCL(uint8_t BitValue);
void I2C_W_SDA(uint8_t BitValue);
uint8_t I2C_R_SDA(void);
void MYI2C_Init(void);
void MYI2C_Start();
void MYI2C_Stop();
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void Send_ACK(uint8_t AckBit);
uint8_t Rec_ACK();

#endif