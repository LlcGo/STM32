#include "stm32f10x.h"                  // Device header

uint8_t Count = 0;

void Infrared_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	EXTI_InitTypeDef E;
	E.EXTI_Line = EXTI_Line8;
	E.EXTI_LineCmd = ENABLE;
	E.EXTI_Mode = EXTI_Mode_Interrupt;
	E.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&E);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitS;
	NVIC_InitS.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 1;
	
    NVIC_Init(&NVIC_InitS);
	
	
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void EXTI9_5_IRQHandler(void)
{
	if(SET==EXTI_GetITStatus(EXTI_Line8))
	{
		Count = !Count;
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else
	{
		Count = 0;
	}
}

/**
void EXTI9_5_IRQHandler(void)
{
	if(SET==EXTI_GetITStatus(EXTI_Line8))
	{
		Count++;
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}
**/

uint8_t Infrared_Read(void){
   return Count;	
}