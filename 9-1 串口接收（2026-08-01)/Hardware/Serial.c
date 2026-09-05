#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdbool.h>
#include <stdio.h>    // 提供 sprintf 等函数（如果需要）
#define MAX_BUFFER 256

void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode =USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);
	
	USART_Cmd(USART2,ENABLE);
}

void SendByte(uint16_t Data)
{
	USART_SendData(USART2,Data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void SendString(char* array)
{
    for(int i = 0; array[i] != '\0'; i++)  // 修正循环条件
    {
        USART_SendData(USART2, array[i]);  // 发送当前字符，不是 array[i]
        
        // 等待发送完成（每次发送后都要等待）
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

uint16_t RECByte()
{
	if (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == RESET)
	{
		return USART_ReceiveData(USART2);
	}
	return 0xFF;
}


char receive_buffer[MAX_BUFFER];
uint16_t buffer_index = 0;

// 清空缓冲区
void ClearBuffer()
{
    memset(receive_buffer, 0, MAX_BUFFER);
    buffer_index = 0;
}

// 接收并构建字符串（带超时）
int ReceiveString(char* output, uint16_t max_len, uint32_t timeout_ms)
{
    uint32_t start_time = 0;
    uint16_t index = 0;
    
    ClearBuffer();
    
    while (index < max_len - 1) 
    {
        // 检查是否有数据
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
        {
            uint16_t data = USART_ReceiveData(USART2);
            receive_buffer[index] = (char)data;  // 存入缓冲区
            index++;
            
            // 如果收到换行符，认为接收完成
            if (data == '\n' || data == '\r') 
            {
                receive_buffer[index] = '\0';  // 字符串结束符
                strcpy(output, receive_buffer);
                return true;
            }
            
            start_time = 0;  // 重置超时计时
        }
        else
        {
            // 简单超时处理
            if (++start_time > timeout_ms) 
            {
                if (index > 0) 
                {
                    receive_buffer[index] = '\0';
                    strcpy(output, receive_buffer);
                    return true;  // 超时但有数据
                }
                return false;  // 超时且无数据
            }
        }
    }
    
    receive_buffer[index] = '\0';
    strcpy(output, receive_buffer);
    return true;
}
/**
void SendByte(uint16_t Data)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	for(int i = 0; i < 8; i++)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)Data &(0x01 << i));
	}
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}


void RecByte()
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == 0)
	{
		uint8_t rec = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
		for(int i = 0; i < 8; i++)
		{
			Rec[i] = rec & 0x01;
		}
	}
}
**/


