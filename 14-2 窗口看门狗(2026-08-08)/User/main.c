#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


int main(void)
{
	OLED_Init();
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST))
	{
		RCC_ClearFlag();
	}
	else
	{
		
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40| 21);
	WWDG_Enable(0x40|54);
	
	while(1){
	  WWDG_SetCounter(0x40|54);
	}
}
