#include "stm32f10x.h"                  // Device header

#include "LED.h"
extern uint16_t NUM;

void Timer_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period =10000-1;  // ACC
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;  // PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	//EXTI_InitTypeDef EXTI_InitStruct;
	//EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	//EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	//EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	//EXTI_InitStruct.EXTI_LineCmd =ENABLE ;
	//EXTI_Init(&EXTI_InitStruct);
	
	TIM_Cmd(TIM4,ENABLE);
}


void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		NUM++;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}




