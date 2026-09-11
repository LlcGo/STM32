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
uint8_t stopClear = 0;
uint8_t isOK = 0;

char* getMessage(char* response);
int compareStr(char*res,char*des);

typedef enum {
	AT,
	AT_CWJAP,
	AT_CIPSTART,
	TCP_REC,
	WIFI_WAIT,
	WIFI_DISCONNECT,
}WIFI_STATUS;


WIFI_STATUS getStatus()
{
	if(rx_complete)
	{
		char* res = getMessage(rx_buffer);
		if(stopClear)
		{
			if(compareStr(res,"GOT"))
			{
			   stopClear = 0;
			   free(res);
			   return AT_CWJAP;
			}
		}
		else if(compareStr(res,"OK"))
		{
			free(res);
			return AT;
		}
		else if(compareStr(res,"GOT"))
		{
			free(res);
			return AT_CWJAP;
		}
		else if(compareStr(res,"WIFI DISCONNECT"))
		{
			free(res);
			return WIFI_DISCONNECT;
		}
		else if(compareStr(res,"CONNECT"))
		{
			free(res);
			return AT_CIPSTART;
		}
		else if(compareStr(res,"IPD"))
		{
			free(res);
			return TCP_REC;
		}
		else 
		{
			free(res);
			return WIFI_WAIT;
		}
	}
		
	return WIFI_WAIT;
}

int compareStr(char*res,char*des)
{
	 if (res == NULL || des == NULL) {
          return 0;
    }
	int res1 = strstr(res,des) != NULL; 
	return res1;
}

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
			if(compareStr(rx_buffer,"GOT"))
			{
			   stopClear = 1;
			}
		}
    }
}


int main(void)
{
    OLED_Init();
    Serial_Init();
    SendString("ATE0\r\n");
	SendString("AT\r\n");
	WIFI_STATUS status;
	
    while(1)
    {
		if(!isOK)
		{
			char* res = getMessage(rx_buffer);
			if(compareStr(res,"OK"))
			{
				status = AT;
			}
			free(res);
		}
		
		switch (status){
			case AT:
			    // 连接上WIFI模块
				OLED_ShowString(1,1,"AT OK");
			    isOK = 1;
				// 发送连接WIFI 尝试切换为连接WIFI成功状态
				SendString("AT+CWJAP=\"CMCC-79Ja\",\"fd8cy37a\"\r\n");
			    char* res = getMessage(rx_buffer);
			    if(compareStr(res,"GOT"))
				{
					 status = AT_CWJAP;
				}
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				free(res);
				
				break;
			case AT_CWJAP:
				// 连接上WIFI
				OLED_ShowString(1,1,"WIFI CONNECT");
				// 发送连接TCP接口 尝试切换为连接TCP状态
				SendString("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8088\r\n");
			    res = getMessage(rx_buffer);
				if(compareStr(res,"IP"))
				{
					 status = AT_CIPSTART;
				}
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				free(res);
				
				break;
			case AT_CIPSTART:
				// TCP连接成功
				OLED_ShowString(1,1,"TCP CONNECT");
			
				res = getMessage(rx_buffer);
				if(compareStr(res,"OK"))
				{
					 status = TCP_REC;
				}
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				free(res);
				break;
			case TCP_REC:
				OLED_ShowString(1,1,"TCP REC");
			    res = getMessage(rx_buffer);
			    OLED_ShowString(2,1,res);
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
			    free(res);
				break;
			default:
				OLED_ShowString(1,1,"WAIT...");
				break;
		}
			
		Delay_ms(2000);
		OLED_Clear();
		//Delay_ms(2000);
    }
}


