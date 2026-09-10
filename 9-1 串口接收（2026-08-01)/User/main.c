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
uint8_t g_initStep = 0;

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
		if(compareStr(res,"OK"))
		{
			g_initStep = 1;
			return AT;
		}
		else if(compareStr(res,"GOT"))
		{
			g_initStep =2;
			return AT_CWJAP;
		}
		else if(compareStr(res,"WIFI DISCONNECT"))
		{
			return WIFI_DISCONNECT;
		}
		else if(compareStr(res,"CONNECT"))
		{
			return AT_CIPSTART;
		}
		else if(compareStr(res,"IPD"))
		{
			return TCP_REC;
		}
		else 
		{
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
	return strstr(res,des) != NULL;
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
		}
    }
}


void processWifiInit(void) {
    // 如果还没收到回复，等待
    if (!rx_complete) {
        return;
    }
    
    // 根据状态执行下一步
    switch (g_initStep) {
        case 1:
            Delay_ms(5000);
            SendString("AT+CWJAP=\"CMCC-79Ja\",\"fd8cy37a\"\r\n");
            break;
        case 2:
            Delay_ms(5000);
            SendString("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8088\r\n");
            break;
        default:
            break;
    }
	
    // 清理缓冲区
    //rx_complete = 0;
    //rx_index = 0;
    //memset(rx_buffer, 0, sizeof(rx_buffer));
}

int main(void)
{
    OLED_Init();
    Serial_Init();
    SendString("ATE0\r\n");
	SendString("AT\r\n");
    while(1)
    {
		WIFI_STATUS status = getStatus();
		processWifiInit();
		switch (status){
			case AT:
				OLED_ShowString(1,1,"AT OK");
			    rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				break;
			case AT_CWJAP:
				OLED_ShowString(1,1,"WIFI CONNECT");
			    rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				break;
			case AT_CIPSTART:
				OLED_ShowString(1,1,"TCP CONNECT");
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				break;
			case TCP_REC:
				OLED_ShowString(1,1,"TCP REC");
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
				break;
			case WIFI_DISCONNECT:
				OLED_ShowString(1,1,"WIFI_DISCONNECT");
				rx_complete = 0;
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
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


