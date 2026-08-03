#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	
	while(1){
	   while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET);
	   uint16_t recData = USART_ReceiveData(USART1);
	}
}
