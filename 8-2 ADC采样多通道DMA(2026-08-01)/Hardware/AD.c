#include "stm32f10x.h"                  // Device header

uint32_t ADC_VALUE[4];
void DMAF(uint32_t Addr1,uint32_t Addr2,uint32_t Size);

void AD_Init(void)
{
	// 开启 RCC ADC,GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	// ADC分配率配置
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// GPIOA使能
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,4,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStruct;
	// 是否一直读取 or 只是读取一次
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 4;
	// 单通道还是多通道
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_InitStruct);
	
	DMAF((uint32_t)&ADC1->DR,(uint32_t)ADC_VALUE,4);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
}

void DMAF(uint32_t Addr1,uint32_t Addr2,uint32_t Size)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = Size;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设作为数据传输的来源
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = Addr1;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_InitStruct.DMA_MemoryBaseAddr = Addr2;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 循环的转运
	DMA_InitStruct.DMA_MemoryInc = DMA_Mode_Circular;
	
	// 硬件触发
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
    DMA_Cmd(DMA1_Channel1,ENABLE);
}

uint8_t AD_GetValue(uint8_t ADC_Channel)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,4);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}