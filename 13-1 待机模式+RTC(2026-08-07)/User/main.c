#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


int main(void)
{
	OLED_Init();
	uint32_t Alarm =  RTC_GetCounter()+10;
	RTC_SetAlarm(Alarm);
	while(1){
	 
		PWR_EnterSTANDBYMode();
	}
}
