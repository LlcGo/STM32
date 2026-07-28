#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t DataA[] = {0X01};
uint8_t DataB[] = {0};

int main(void)
{
	OLED_Init();
	
	OLED_ShowHexNum(1,1,DataA[0],1);
	OLED_ShowHexNum(2,1,DataB[0],1);
	MYDMA_Init((uint32_t)DataA,(uint32_t)DataB,1);
	OLED_ShowHexNum(3,1,DataA[0],1);
	OLED_ShowHexNum(4,1,DataB[0],1);
	while(1){
	
	}
}
