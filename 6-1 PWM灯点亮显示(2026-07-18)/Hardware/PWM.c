#include "stm32f10x.h"                  // Device header


void PWM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TimBaseStruct;
	TimBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimBaseStruct.TIM_Period =  100 -1;
	TimBaseStruct.TIM_Prescaler = 720 -1;
	TimBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TimBaseStruct);
	
	TIM_OCInitTypeDef TIMOCInitStruct;
	
	
	TIM_OCStructInit(&TIMOCInitStruct);
	TIMOCInitStruct.TIM_OCMode =TIM_OCMode_PWM1;
	TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2,&TIMOCInitStruct);

	//TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	//TIM_OC1PolarityConfig(TIM2,TIM_OCPolarity_High);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}