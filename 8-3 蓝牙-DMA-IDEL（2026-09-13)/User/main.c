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

int RECFlage = 0;
int start = 0;
uint16_t currentLen = 0;
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
    if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
		uint8_t* REC = RecMessage;
		
		volatile uint32_t tmp;
		tmp = USART2->SR;      // 先读 SR
		tmp = USART2->DR;      // 再读 DR，才能清 IDLE
		(void)tmp;

		// 停止转运
		DMA_Cmd(DMA1_Channel6, DISABLE);
		
		uint16_t DCurrDataCount = DMA_GetCurrDataCounter(DMA1_Channel6);
		currentLen = MAX_BUFFER - DCurrDataCount;
		
		// 重新从头开始转运
		DMA_SetCurrDataCounter(DMA1_Channel6,MAX_BUFFER);
		// 重新转运
		DMA_Cmd(DMA1_Channel6, ENABLE);

		// 可以开始处理数据
		RECFlage = 1;
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
		if (RECFlage)
		{
			if(!start)
			{
				char* res = getMessage((char*)RecMessage);
				if(compareStr(res,"OK"))
				{
						status = AT;
				}
				memset((char*)RecMessage,0,MAX_BUFFER);
				free(res);
				start = 1;
				// 处理数据完毕
				RECFlage = 0;
			}

			switch (status){
				case AT:
					// 连接上WIFI模块
					OLED_ShowString(1,1,"AT OK");

					// 发送连接WIFI 尝试切换为连接WIFI成功状态
					SendString("AT+CWJAP=\"CMCC-79Ja\",\"fd8cy37a\"\r\n");
					//SendString("AT+CWJAP=\"Xiaomi 14 Ultra_kFumOP6_MI\",\"hubingjiehaha\"\r\n");
				    
				    char* res = getMessage((char*)RecMessage);
					if(compareStr(res,"GOT"))
					{
						status = AT_CWJAP;
						memset((char*)RecMessage,0,MAX_BUFFER);
						//RECFlage = 0;
					}
					free(res);
					break;
				case AT_CWJAP:
					OLED_ShowString(1,1,"                ");
					// 连接上WIFI
					OLED_ShowString(1,1,"WIFI CONNECT");
				
					// 发送连接TCP接口 尝试切换为连接TCP状态
					SendString("AT+CIPSTART=\"TCP\",\"192.168.1.5\",8088\r\n");
				
					res = getMessage((char*)RecMessage);
					if(compareStr(res,"CONNECT"))
					{
						status = AT_CIPSTART;
						memset((char*)RecMessage,0,MAX_BUFFER);
						//RECFlage = 0;
					}
					free(res);
					break;
				case AT_CIPSTART:
					OLED_ShowString(1,1,"                ");
					// TCP连接成功
					OLED_ShowString(1,1,"TCP CONNECT");
					
				    res = getMessage((char*)RecMessage);
					if(compareStr(res,"IPD"))
					{
						status = TCP_REC;
						memset((char*)RecMessage,0,MAX_BUFFER);
						//RECFlage = 0;
					}
					free(res);
					break;
				case TCP_REC:
					OLED_ShowString(1,1,"                ");
					OLED_ShowString(1,1,"TCP REC");
				    res = getMessage((char*)RecMessage);
					if(compareStr(res,"IPD"))
					{
						OLED_ShowString(2,1,res);
						Delay_ms(1000);
						OLED_Clear();
						status = TCP_REC;
					}else if(compareStr(res,"CLOSED"))
					{
						OLED_ShowString(2,1,res);
						OLED_ShowString(3,1,"TO AT_CWJAP");
						Delay_ms(2000);
						OLED_Clear();
						status = AT_CWJAP;
					}
					memset((char*)RecMessage,0,MAX_BUFFER);
					free(res);
					//RECFlage = 0;
					
					break;
				default:
					OLED_ShowString(1,1,"WAIT...");
					break;
			}
		}
		
		//OLED_Clear();
		
    }
}


