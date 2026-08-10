#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"

uint16_t Store_Data[512];


void Store_Init(void)
{
	if(MyFlash_ReadHalfWord(0x800FC00) != 0xA5A5)
	{
		MyFlash_ErasePages(0x0800FC00);
		MyFlash_
	}
}