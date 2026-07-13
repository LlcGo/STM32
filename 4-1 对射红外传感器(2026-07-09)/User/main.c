#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Infrared.h"

int main(void)
{
	OLED_Init();
	Infrared_Init();
	while(1){
	
      OLED_ShowNum(1,1,Infrared_Read(),4);		
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
