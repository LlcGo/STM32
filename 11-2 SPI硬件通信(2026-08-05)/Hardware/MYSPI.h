#ifndef __MYSPI_H
#define __MYSPI_H

void MYSPI_Init(void);
void MYSPI_Start();
void MYSPI_Stop();
uint8_t MYSPI_SwapByte(uint8_t ByteSend);

#endif