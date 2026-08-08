#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


int main(void)
{
	OLED_Init();
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST))
	{
		RCC_ClearFlag();
	}
	else
	{
		
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499); // 1000ms
	IWDG_ReloadCounter();
	IWDG_Enable();
	
	while(1){
	
	}
}
