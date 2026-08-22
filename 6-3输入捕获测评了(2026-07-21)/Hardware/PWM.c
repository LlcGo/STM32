#include "stm32f10x.h"                  // Device header


void PWM_Init()
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 时基单元初始化
   	// 总线APB1给TIM2使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	// 使用内部时钟 TIM2的
	TIM_InternalClockConfig(TIM2);
	
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 -1; //ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 -1; //PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	// 初始化
	TIM_OCStructInit(&TIM_OCInitStruct);
	// 设置输出比较模式
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	// 设置输出比较的极性
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	// 设置输出使能
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	// 设置CCR
	TIM_OCInitStruct.TIM_Pulse = 0; // CCR
	// 比较器初始化
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	
	// 定时器启动
	TIM_Cmd(TIM2,ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
}