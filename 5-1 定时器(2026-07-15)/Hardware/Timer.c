#include "stm32f10x.h"                  // Device header

#include "LED.h"

extern uint16_t Num;
uint16_t innerNum;

void Timer_Init(void){
	
	LED_Init();
	
	// 总线APB1给TIM2使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	// 使用内部时钟 TIM2的
	TIM_InternalClockConfig(TIM3);
	TIM_InternalClockConfig(TIM2);
	
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = 20000 -1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 -1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	// 解决中断控制一进去就触发中断的问题
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	// 中断输出控制
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct ;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	// 定时器启动
	TIM_Cmd(TIM3,ENABLE);
	// TIM_Cmd(TIM2,ENABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		innerNum++;
		if (innerNum % 60 == 0)
		{
			Num++;
		}
		LED1_ON();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		innerNum++;
		if (innerNum % 60 == 0)
		{
			Num++;
		}
		LED1_OFF();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}