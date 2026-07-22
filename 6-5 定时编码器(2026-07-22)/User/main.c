#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
//	Timer_Init();
	Encoder_Init();
	while(1){
	   OLED_ShowNum(1,1,Encoder_Get(),5);
	}
}
