#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Infrared.h"
#include "LED.h"

int main(void)
{
	OLED_Init();
	Infrared_Init();
	LED_Init();
	
	while(1){
		
		//Infrared_Read()
	// LED1_OFF();
	// Delay_ms(1000);
	// LED1_ON();
	// Delay_ms(1000);
		if (Infrared_Read() == 0)
		{
			LED1_OFF();
		}
		else
		{
			LED1_ON();
		}
    //OLED_ShowNum(1,1,Infrared_Read(),4);		
//	  if(Infrared_Read() == 0)
//	  {
//		  
//	  }
//	  else
//	  {
//		  Count++;
//		  OLED_ShowNum(1,1,Count,4);
//	  }
	}
}
