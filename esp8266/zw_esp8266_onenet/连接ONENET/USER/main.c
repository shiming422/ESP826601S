#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "esp.h"

void Send_Onenet_Data()
{
		Get_Send_Pack("Fan", 2, 1);	//json数据赋值
    SendData_Onenet();					//编写一帧数据包
    delay_ms(200);
    SendData_Onenet();
	
		Get_Send_Pack("Feed", 2, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("Food", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("Food_F", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("Water", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("Water_F", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_Pack("cat", 2, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
	  Get_Send_Pack("dog", 2, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("temp", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
		
		Get_Send_float_Pack("temp_f", 2.1, 1);
    SendData_Onenet();
    delay_ms(200);
    SendData_Onenet();
}

int main(void)
{			 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		uart_init(115200);	 
		delay_init();	    	
		LED_Init();
		alert_Init();		
		esp_Init();
	
		while(1)
		{
			Send_Onenet_Data();

	//		 alertLED = 0;
	//		 delay_ms(2000);
	//		 alertLED = 1;
	//		 delay_ms(2000);	
			
			esp_Get_Data();
	
		}
}
