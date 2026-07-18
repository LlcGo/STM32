#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	while(1){
	   OLED_ShowNum(1,1,Timer_GetCounter(),5);
//	   OLED_ShowNum(2,1,TIM_GetCounter(TIM2),5);
	}
}
