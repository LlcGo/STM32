#include "stm32f10x.h"                  // Device header

void MYDMA_Init(uint32_t Addr1,uint32_t Addr2,uint32_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_Channel_TypeDef DMA_ChannelStruct;
	DMA_InitTypeDef DMA_InitTypeStruct;
	
	DMA_InitTypeStruct.DMA_PeripheralBaseAddr = Addr1;
	DMA_InitTypeStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	
	
	DMA_InitTypeStruct.DMA_MemoryBaseAddr = Addr2;
	DMA_InitTypeStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitTypeStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitTypeStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitTypeStruct.DMA_BufferSize = Size;
	DMA_InitTypeStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitTypeStruct.DMA_M2M = DMA_M2M_Enable;

	DMA_Init(DMA1_Channel1,&DMA_InitTypeStruct);
	DMA_Cmd(DMA1_Channel1,ENABLE);
}