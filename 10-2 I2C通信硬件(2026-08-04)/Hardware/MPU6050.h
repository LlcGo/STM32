#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_Init(void);
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);

#endif