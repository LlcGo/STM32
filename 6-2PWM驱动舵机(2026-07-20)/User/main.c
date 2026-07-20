#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"


int main(void)
{
	OLED_Init();
	Servo_Init();
	
	Servo_SetAngle(90);
	while(1){
	  
	}
}
