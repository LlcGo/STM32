#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	SendByte(0X41);
	while(1){
		if (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
		{
			 OLED_ShowHexNum(1,1,USART_ReceiveData(USART2),2);
		}
	}
}
