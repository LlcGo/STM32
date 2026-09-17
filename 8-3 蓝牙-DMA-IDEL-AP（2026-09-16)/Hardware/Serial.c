#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdbool.h>
#include <stdio.h>    // 提供 sprintf 等函数（如果需要）
#include "Serial.h"
uint8_t RecMessage[MAX_BUFFER];

void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 串口配置
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode =USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);
	
	
	// 设置DMA
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = MAX_BUFFER;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设作为数据传输的来源
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)RecMessage;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	// 循环的转运
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 硬件触发
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel6,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
	// 数据搬运
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	
	// 等待出发中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
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
        USART_SendData(USART2, array[i]);  // 发送当前字符
        
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


