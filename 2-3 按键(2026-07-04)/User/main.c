#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "LightSensor.h"
#include "Buzzer.h"

uint8_t KeyNumber = 0;

int main(void)
{
	Buzzer_Init();
	Light_Init();

	while(1){
	  if(Light_Get() == 1)
	  {
		  Buzzer_ON();
	  }
	  else 
	  {
		  Buzzer_OFF();
	  }
	}
}
