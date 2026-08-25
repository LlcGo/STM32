#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t NUM;

int main(void)
{
	OLED_Init();
	Timer_Init();
	while(1){
	   OLED_ShowNum(1,1,NUM,5);
	}
}
