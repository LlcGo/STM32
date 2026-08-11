#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"

uint16_t Store_Data[512];


void Store_Init(void)
{
	if(MyFlash_ReadHalfWord(0x800FC00) != 0xA5A5)
	{
		MyFlash_ErasePages(0x0800FC00);
		MyFlash_ProgramWord(0x800FC00,0xA5A5);
		for(uint16_t i = 1; i < 512; i++)
		{
			MyFlash_ProgramHalfWord(0x800FC00+i,0x0000);
		}
	}
	
	for(uint16_t i = 0; i < 512; i++)
	{
		Store_Data[i] = MyFlash_ReadHalfWord(0x800FC00+i*2);
	}
}

void Store_Save(void)
{
	MyFlash_ErasePages(0x800FC00);
	for(uint16_t i = 0; i < 512; i++)
	{
		MyFlash_ProgramHalfWord(0x800FC00+i*2,Store_Data[i]);
	}
}

void Store_Clear(void)
{
	for(uint16_t i = 1; i < 512; i++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save();
}