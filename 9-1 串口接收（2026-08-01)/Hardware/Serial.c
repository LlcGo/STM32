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
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	// /TIM2_CH4
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART2,ENABLE);
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


