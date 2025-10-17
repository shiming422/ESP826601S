#include "HX711.h"
#include "usart.h"
#include "gpio.h"
#include "dht11.h"
unsigned long val =0; 

long HX711_Buffer = 0,First_Weight=0,Weight=0;
 

long Get_Weight(void)
{
	HX711_Buffer=Get_number();
	Weight = HX711_Buffer;
	Weight=(long)((float)Weight/420);
	return Weight;
}
 
unsigned long Get_number()
{
			val=0;
		  CLK_0;
		  while(!Read_PIN);
		  for(int i=0;i<24;i++)
		  {
			  Delay_us(100);
			  CLK_1;
			  val=val<<1;
			  Delay_us(1);
			  CLK_0;
				Delay_us(1);
			  if(Read_PIN)
			  val++;
			  Delay_us(1);
 
		  }
		  CLK_1;
		  val = val^0x800000; 
		  Delay_us(1);
		  CLK_0;
		  return val;
		  
}



long Get_Weight_l(void)
{
	HX711_Buffer=Get_number_l();
	Weight = HX711_Buffer;
	Weight=(long)((float)Weight/420);
	return Weight;
}
 
unsigned long Get_number_l()
{
			val=0;
		  l_CLK_0;
		  while(!l_Read_PIN);
		  for(int i=0;i<24;i++)
		  {
			  Delay_us(100);
			  l_CLK_1;
			  val=val<<1;
			  Delay_us(1);
			  l_CLK_0;
				Delay_us(1);
			  if(l_Read_PIN)
			  val++;
			  Delay_us(1);
 
		  }
		  l_CLK_1;
		  val = val^0x800000; 
		  Delay_us(1);
		  l_CLK_0;
		  return val;
		  
}