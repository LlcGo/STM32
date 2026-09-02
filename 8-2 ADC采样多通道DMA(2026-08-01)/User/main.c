#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint8_t ADC_Value;

int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowNum(1,1,ADC_Value,5);   
	while(1){
		ADC_Value = AD_GetValue();
	}
}
