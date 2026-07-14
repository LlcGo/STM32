#include "stm32f10x.h"                  // Device header

uint16_t Count;

void Encoder_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	
	EXTI_InitTypeDef E;
	E.EXTI_Line = EXTI_Line1|EXTI_Line0;
	E.EXTI_LineCmd = ENABLE;
	E.EXTI_Mode = EXTI_Mode_Interrupt;
	E.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&E);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitS;
	NVIC_InitS.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitS);
	
	NVIC_InitS.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 2;
	
    NVIC_Init(&NVIC_InitS);
}

void EXTI0_IRQHandler(void)
{
	if(SET==EXTI_GetITStatus(EXTI_Line0))
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
		{
			Count--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(SET==EXTI_GetITStatus(EXTI_Line1))
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
		{
			Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

uint16_t Encoder_Read(void){
   uint16_t Temp = 0;
   Temp = Count;
   Count = 0;
   return Temp;	
}