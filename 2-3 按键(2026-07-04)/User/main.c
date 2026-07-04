#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNumber = 0;

int main(void)
{
	LED_Init();
	Key_Init();
	
	while(1){
	   KeyNumber = Key_GetNum();
	   if(KeyNumber == 1)
	   {
		   LED1_ON();
	   }
	    if(KeyNumber == 2)
	   {
		   LED1_OFF();
	   }	   
	}
}
