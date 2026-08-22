#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"


int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();

	PWM_SetPrescaler(720-1);
	PWM_SetCompare1(50);

	while(1){
	  OLED_ShowNum(1,1,IC_GetFreq(),5);
	}
}
