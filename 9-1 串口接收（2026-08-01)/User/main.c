#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h> 

char rx_buffer[128];
uint16_t rx_index = 0;
uint8_t rx_complete = 0;

char* getMessage(char* response)
{
	char* clean = malloc(128);
	uint16_t j = 0;
	for(uint16_t i = 0; response[i] != '\0'; i++)
	{
		if(response[i] != '\r' && response[i] != '\n')
		{
			clean[j++] = response[i];
		}
	}
    clean[j] = '\0';
	return clean;
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        uint16_t data = USART_ReceiveData(USART2);
		rx_buffer[rx_index++] = (char)data;
		if(data == '\n')
		{
			rx_buffer[rx_index] = '\0';
			rx_complete = 1;
		}
    }
}

int main(void)
{
    OLED_Init();
    Serial_Init();
	
   
    /**
	SendString("AT\r\n");
    Delay_ms(5000);
	SendString("AT+CWJAP=\"CMCC-79Ja\",\"fd8cy37a\"\r\n");
        
	Delay_ms(5000);
	SendString("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8088\r\n");
	**/
    while(1)
    {
		OLED_Clear();
		SendString("ATE0\r\n");
		Delay_ms(2000);
		SendString("AT\r\n");
		if(rx_complete)
		{
			char* clean_msg = getMessage(rx_buffer);
			OLED_ShowString(1, 1,clean_msg);
			free(clean_msg);
			
			OLED_ShowNum(4,1,rx_index,2);
			memset(rx_buffer, 0, sizeof(rx_buffer));
			rx_index=0;
			rx_complete = 0;
		}
		
		SendString("AT+CWJAP=\"CMCC-79Ja\",\"fd8cy37a\"\r\n");
		Delay_ms(5000);
		if(rx_complete)
		{
			char* clean_msg = getMessage(rx_buffer);
			OLED_ShowString(2, 1,clean_msg);
			free(clean_msg);
			
			OLED_ShowNum(4,1,rx_index,2);
			memset(rx_buffer, 0, sizeof(rx_buffer));
			rx_index=0;
			rx_complete = 0;
		}
		
		SendString("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8088\r\n");
		Delay_ms(6000);
		if(rx_complete)
		{
			char* clean_msg = getMessage(rx_buffer);
			OLED_ShowString(3, 1,clean_msg);
			free(clean_msg);
			
			OLED_ShowNum(4,1,rx_index,2);
			memset(rx_buffer, 0, sizeof(rx_buffer));
			rx_index=0;
			rx_complete = 0;
		}
		
		
		Delay_ms(5000);
		OLED_Clear();
		while(1)
		{
			if(rx_complete)
			{
				char* clean_msg = getMessage(rx_buffer);
				OLED_ShowString(2, 1, clean_msg);
				free(clean_msg);
				
				OLED_ShowNum(4, 1, rx_index, 2);
				memset(rx_buffer, 0, sizeof(rx_buffer));
				rx_index = 0;
				rx_complete = 0;
			}
			Delay_ms(3000);  // 一定要加延时！
			OLED_Clear();
		}
		//Delay_ms(2000);
    }
}


