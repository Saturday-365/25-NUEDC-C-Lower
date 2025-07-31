#include "stm32f4xx.h"                  // Device header

void Delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;
      while(i--) ;    
   }
}
