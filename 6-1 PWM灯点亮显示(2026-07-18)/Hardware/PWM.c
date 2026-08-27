#include "stm32f10x.h"                  // Device header


void PWM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 -1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStrutc;
	TIM_OCStructInit(&TIM_OCInitStrutc);
	TIM_OCInitStrutc.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStrutc.TIM_OCPolarity  = TIM_OCPolarity_High;
	TIM_OCInitStrutc.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStrutc.TIM_Pulse =90;
	TIM_OC1Init(TIM3,&TIM_OCInitStrutc);
	
	TIM_Cmd(TIM3,ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3,Compare);
}