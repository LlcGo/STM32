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

char response[256];

void ReadResponse(char* buffer, uint16_t max_len)
{
    uint16_t index = 0;
    uint32_t timeout = 0;
    uint32_t max_timeout = 10000000;
    
    while(index < max_len - 1)
    {
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
        {
            buffer[index++] = USART_ReceiveData(USART2);
            timeout = 0;  // 收到数据，重置超时
        }
        else
        {
            timeout++;
            if(timeout > max_timeout)  // 超时退出
                break;
        }
    }
    buffer[index] = '\0';
}

char* getMessage(char* response)
{
	char* clean = malloc(64);
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

int main(void)
{ 
	OLED_Init();
    Serial_Init();
    
    char response[64];
    
    // 关闭回显
    SendString("ATE0\r\n");
    Delay_ms(1000);
    ReadResponse(response, sizeof(response));  // 清空缓冲区
    
    while(1)
    {
        OLED_Clear();
        
        // 发送 AT 指令
        SendString("AT\r\n");
        
        // 等待并读取完整响应
        ReadResponse(response, sizeof(response));
        char* clean = getMessage(response);
        OLED_ShowString(1, 1, clean);  // 显示 "OK"
        
		 // 发送 AT 指令
        SendString("AT+CWMODE=1\r\n");
		ReadResponse(response, sizeof(response));
        char* clean2 = getMessage(response);
        OLED_ShowString(2, 1, clean2);  // 显示 "OK"
		Delay_ms(1000);
		
		SendString("AT+CWJAP=\"xy\",\"lc6677777\"\r\n");
		
		// ✅ 关键：等待 10 秒让 WiFi 连接完成

    
		ReadResponse(response, sizeof(response));
        char* clean3 = getMessage(response);
        OLED_ShowString(3, 1, clean3);  // 显示 "OK"
		
        Delay_ms(3000);
		while(1);
		
 }
}


void ReadAllResponse(void)
{
    uint16_t index = 0;
    char response[64];
    
    while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
    {
        response[index++] = USART_ReceiveData(USART2);
    }
    response[index] = '\0';  // 添加结束符
    
    // 显示完整响应
    OLED_ShowString(1, 1, response);  // 需要实现字符串显示
}
