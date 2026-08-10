#include "stm32f10x.h"                  // Device header

uint8_t Count;

void Infrared_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitTypeDef E;
	E.EXTI_Line = EXTI_Line14;
	E.EXTI_LineCmd = ENABLE;
	E.EXTI_Mode = EXTI_Mode_Interrupt;
	E.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&E);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitS;
	NVIC_InitS.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 1;
	
    NVIC_Init(&NVIC_InitS);
	
	
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}


void EXTI15_10_IRQHandler(void)
{
	if(SET==EXTI_GetITStatus(EXTI_Line14))
	{
		Count++;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

uint8_t Infrared_Read(void){
   return Count;	
}