#include "stm32f1xx.h"

#include "dht11.h"

uint8_t Data[5]={0x00,0x00,0x00,0x00,0x00};
		
void Delay_us(uint16_t us){
	uint16_t differ = 0xffff-us;
	__HAL_TIM_SET_COUNTER(&htim1,differ);
	HAL_TIM_Base_Start(&htim1);
	
	while(differ < 0xffff){
		differ = __HAL_TIM_GET_COUNTER(&htim1);
	}
	HAL_TIM_Base_Stop(&htim1);
}



void DHT_GPIO_SET_OUTPUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=GPIO_PIN_0;
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void DHT_GPIO_SET_INPUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=GPIO_PIN_0;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t DHT_Read_Byte(void)
{
	 uint8_t ReadData=0;
	 uint8_t temp;
	 uint8_t retry=0;
	 uint8_t i;    
	 for(i=0; i<8; i++)
	 {
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0 && retry<100)

			{
					Delay_us(1);
				  retry++;
			}
			retry=0;
			Delay_us(40);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)   temp=1;
			 else   temp=0;

			 while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 && retry<100)
			 {
				 Delay_us(1);
				 retry++;
			 }
			 retry=0;
			 
			 ReadData<<=1;
			 ReadData |= temp;
	 }

		return ReadData;
}

uint8_t DHT_Read(void)
{
	 uint8_t retry=0;
	 uint8_t i;
		
	 DHT_GPIO_SET_OUTPUT();
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	 HAL_Delay(18);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	 Delay_us(20);
	

	DHT_GPIO_SET_INPUT();
	Delay_us(20);
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0)
	{

		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0 && retry<100)
		{
		   Delay_us(1);
			 retry++;
		}
		retry=0;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 && retry<100)
		{
		   Delay_us(1);
			 retry++;
		}
		retry=0;
		

		for(i=0; i<5; i++)
		{
			 Data[i] = DHT_Read_Byte();
		}
		Delay_us(50);

	}
	
	 uint32_t sum=Data[0]+Data[1]+Data[2]+Data[3];
	 if((sum)==Data[4])    return 1;  
	   else   return 0;
	
}
