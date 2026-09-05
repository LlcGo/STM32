#ifndef __SERIAL_H
#define __SERIAL_H


void Serial_Init(void);
void SendByte(uint16_t Data);
void SendString(char* string);
uint16_t RECByte();
int ReceiveString(char* output, uint16_t max_len, uint32_t timeout_ms);
void ClearBuffer();
#endif