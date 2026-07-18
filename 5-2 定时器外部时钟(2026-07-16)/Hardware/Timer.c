#include "stm32f10x.h"                  // Device header

extern uint16_t Num;

void Timer_Init(void){
	
	// 总线APB1给TIM2使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);
	
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	// 解决中断控制一进去就触发中断的问题
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	// 中断输出控制
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct ;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	// 定时器启动
	TIM_Cmd(TIM2,ENABLE);
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

//void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//	{
//		Num++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}