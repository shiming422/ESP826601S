#include "oled.h"

uint8_t str1[100];
uint8_t str2[100];
uint8_t str3[100];
void show_str(int food_data1,int food_data2,uint32_t water_data,uint32_t temperature_data)
{
	/*食物重量*/
	sprintf(str2,"Cw: %dg  ",food_data1);
	OLED_ShowString(0,0,str2,16);
	sprintf(str3,"Dw: %dg  ",food_data2);
	OLED_ShowString(63,0,str3,16);
	
	/*水位数据*/
	OLED_ShowCHinese(0,2,11);
	OLED_ShowCHinese(18,2,12);
	OLED_ShowCHinese(36,2,5);
	OLED_ShowCHinese(54,2,6);
	OLED_ShowCHinese(72,2,4);
	OLED_ShowNum(79,2,water_data,4,16);
	OLED_ShowString(110,2,"mm",16);
	
	/*温度*/
	OLED_ShowCHinese(0,4,9);
	OLED_ShowCHinese(18,4,10);
	OLED_ShowCHinese(36,4,7);
	OLED_ShowCHinese(54,4,8);
	OLED_ShowCHinese(72,4,4);
	OLED_ShowNum(78,4, temperature_data,sizeof(temperature_data),16);
	OLED_ShowCHinese(110,4,16);
}