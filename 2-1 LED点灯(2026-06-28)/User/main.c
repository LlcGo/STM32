#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//uint16_t GPIO_Pin_Array []  ={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7};

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	int i = 0;
	int j = 0x0001;
	while(1)
	{
		for(i= 0; i<8; i++){
			GPIO_Write(GPIOA,~(j << i));
			Delay_ms(500);
		}
	}
}
