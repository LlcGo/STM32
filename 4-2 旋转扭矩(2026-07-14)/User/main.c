#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	while(1){
	  Num +=  Encoder_Read();
      OLED_ShowNum(1,1,Num,4);		
//	  if(Infrared_Read() == 0)
//	  {
//		  
//	  }
//	  else
//	  {
//		  Count++;
//		  OLED_ShowNum(1,1,Count,4);
//	  }
	}
}
