#include "stm32f10x.h"                  // Device header

uint32_t MYDMA_Size;

void MYDMA_Init(uint32_t Addr1,uint32_t Addr2,uint32_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = Size;
	MYDMA_Size = Size;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设作为数据传输的来源
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = Addr1;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	
	DMA_InitStruct.DMA_MemoryBaseAddr = Addr2;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
    DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MYDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MYDMA_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}