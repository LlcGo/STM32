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

typedef enum{
	ST_IDLE,
	AP_CWMODE,
	AP_CIPMUX,
	AP_CIPSERVER,
	AP_SERVER,
	AP_REC,
}AP_STATUS;


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

	AP_STATUS status = ST_IDLE;
	
    while(1)
    {
		// 接收到数据后开始执行状态机
		if (!RECFlage)continue;
		
		// 因为case后面不能申明变量所以迁移值上方获取值
		char* res = getMessage((char*)RecMessage);
		
		switch (status){
			case ST_IDLE:
				if(compareStr(res,"OK"))
				{
					OLED_ShowString(1,1,"AT OK");
					// 往wifi模块发送数据切换为换为AP模式
					SendString("AT+CWMODE=2\r\n");
					status = AP_CWMODE;
				}
				break;
			case AP_CWMODE:
				OLED_ShowString(1,1,"                            ");
				// AP模式切换成功
				if(compareStr(res,"OK"))
				{
					OLED_ShowString(1,1,"AT+CWMODE OK");
					// 改为多客户端连接
					SendString("AT+CIPMUX=1\r\n");
					status = AP_CIPMUX;
				}
				break;
			case AP_CIPMUX:
				OLED_ShowString(1,1,"                            ");
				// 多客户端连接切换成功
				if(compareStr(res,"OK"))
				{
					OLED_ShowString(1,1,"AT+CIPMUX=1 OK");
					// 开启TCP服务
					SendString("AT+CIPSERVER=1,8080\r\n");
					status = AP_CIPSERVER;
				}
				
				break;
			case AP_CIPSERVER:
				OLED_ShowString(1,1,"                            ");
				if(compareStr(res,"no change") || compareStr(res,"OK"))
				{
					OLED_ShowString(1,1,"AT+CIPSERVER=1,8080 OK");
					status = AP_SERVER;
				}
				break;
			case AP_SERVER:
				OLED_ShowString(1,1,"                ");
				if (!compareStr(res,"FAIL"))
				{
					if( compareStr(res,"CONNECT"))
					{
						OLED_ShowString(1,1,"connect success");
						status = AP_REC;
					}
				}else
				{
					OLED_ShowString(1,1,"connect wait");
				}
				break;
			case AP_REC:
				OLED_Clear();
				OLED_ShowString(1,1,"WAIT MESSAGE");
				if(compareStr(res,"IPD"))
				{
					OLED_ShowString(2,1,"success");
					OLED_ShowString(3,1,res);
					memset((char*)RecMessage,0,MAX_BUFFER);
				}else if(compareStr(res,"FAIL"))
				{
					OLED_ShowString(1,1,"QUIT");
					status = AP_SERVER;
				}
				break;
			default:
				OLED_ShowString(1,1,"WAIT...");
				break;
		}
		//memset((char*)RecMessage,0,MAX_BUFFER);
		free(res);
	}
}


