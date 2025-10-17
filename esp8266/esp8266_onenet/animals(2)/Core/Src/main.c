/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "headfile.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WATER_VALUE 1400
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*识别动物类别*/
volatile int animals_flag=2;
/*喂食时间变量*/
unsigned char Send_Time = 0;
/*水位设置*/
uint32_t water_data=0;
/*猫狗对应的秤的数据*/
int Cat_First_weight=0,Cat_weight=0,
		Dog_First_weight=0,Dog_weight=0;
/*喂食次数，二者叠加*/
int feed_time=0;
unsigned char feed_flag=0;
/*模拟空调开闭*/
int LED_flag=0;
/*k210数据接收*/
char k210_str[10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Send_Onenet_Data()
{
	if(Send_Time>1)
	{					
		Get_Send_Pack("Feed",feed_time, 1);
		Get_Send_Pack("cat",app_data.Cat_data, 1);
		Get_Send_Pack("Food",Cat_weight, 1);
		Get_Send_Pack("dogFood",Dog_weight, 1);
	  Get_Send_Pack("dog",app_data.Dog_data, 0);
		
	
    SendData_Onenet();
    HAL_Delay(50);
	
		Get_Send_Pack("Cat_Food_F",app_data.Cat_Food_F_data, 1);
    Get_Send_Pack("dogFood_F",app_data.Dog_Food_F_data, 1);
		Get_Send_Pack("Water",water_data, 1);
		Get_Send_Pack("Water_F",app_data.Water_F_data, 0);
		SendData_Onenet();
    HAL_Delay(50);
		
		Get_Send_Pack("temp",Data[2], 1);
		Get_Send_Pack("temp_f",app_data.temp_F_data,0);

    SendData_Onenet();
		Send_Time=0;
	}	
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart2,k210_str,1);

	
	Cat_First_weight=Get_Weight_l();
	Dog_First_weight=Get_Weight();
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,2,"Waiting....",16);
	APP_Data_Init();
	Onenet_Init();
	OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  char test_str[20];

	while (1)
  {//HAL_GPIO_WritePin(GPIOA,VOICE_Pin,1);
		Send_Time++;
		/*压力检测*/                                                                                                                                                             
		/*猫粮称重*/
		Cat_weight=-(-Get_Weight_l()+Cat_First_weight);
		Cat_weight-=1590;

		if(Cat_weight<0)
			Cat_weight=0;		
		Cat_weight/=10;
		/*狗粮称重*/
		Dog_weight=-(-Get_Weight()+Dog_First_weight);
		if(Dog_weight<0)
			Dog_weight=0;
		Dog_weight/=10;
		/*温度检测*/
		if(DHT_Read())
			HAL_Delay(100);

		/*水位检测*/
		HAL_ADC_Start(&hadc2) ;
		water_data=HAL_ADC_GetValue(&hadc2);
		water_data=40-water_data/100;
		/*动物出现判断*/
		/******************************/
		if(animals_flag==0)//当检测到猫
		{	app_data.Cat_data=1;
			app_data.Dog_data=0;
			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,0);
//				HAL_GPIO_WritePin(GPIOA,VOICE_Pin,1);
//  			HAL_Delay(200);
//  			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,0);
			OLED_ShowString(0,7,"cat",8);
			/*播放音乐*/
			
			HAL_Delay(20);
			/*水位低于阈值*/
			if(water_data<app_data.Water_F_data)
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
			else
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
			/*重量低于阈值*/
			
			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,1);
			if(Cat_weight<app_data.Cat_Food_F_data)
			{
				Step_MOTOR_Start(10,1);	
				if(feed_flag==0)
				{
					feed_time++;
					feed_flag=1;
				}
			}
			else
			{
				Step_MOTOR_Stop();//电机停止
				feed_flag=0;
			}
			//温度是否在阈值范围内
			if(Data[2]>app_data.temp_F_data)
			{
				HAL_GPIO_WritePin(GPIOB,LED_Pin,1);
				LED_flag=1;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,LED_Pin,0);
				LED_flag=0;
			}	
		}
		else if(animals_flag==1)//当检测到狗
		{
			app_data.Dog_data=1;
			app_data.Cat_data=0;
			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,0);
			OLED_ShowString(0,7,"dog",8);
			/*播放音乐*/
			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,0);
			HAL_Delay(20);
			/*水位低于阈值*/
			if(water_data<app_data.Water_F_data)
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
			else
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
			/*重量低于阈值*/
			HAL_GPIO_WritePin(GPIOA,VOICE_Pin,1);
			if(Dog_weight<app_data.Dog_Food_F_data)
			{
				r_Step_MOTOR_Start(50,1);	
				if(feed_flag==0)
				{
					feed_time++;
					feed_flag=1;
				}
			}
			else
			{
				r_Step_MOTOR_Stop();//电机停止
				feed_flag=0;
			}
			//温度是否在阈值范围内
			if(Data[2]>app_data.temp_F_data)
			{
				HAL_GPIO_WritePin(GPIOB,LED_Pin,1);
				LED_flag=1;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,LED_Pin,0);
				LED_flag=0;
			}	
		}
		else
		{
			/*关闭语音*/
			//HAL_GPIO_WritePin(GPIOA,VOICE_Pin,1);
			/*关闭续水*/ 
			HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
			/*关闭续粮*/
			Step_MOTOR_Stop();//电机停止
			r_Step_MOTOR_Stop();//电机停止
			/*关闭模拟空调*/
			HAL_GPIO_WritePin(GPIOB,LED_Pin,0);
		}
		show_str(Cat_weight,Dog_weight,water_data,Data[2]);
		/*数据汇总发送*/
		Send_Onenet_Data();
		    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {

        char c = k210_str[0];

        if (c == '0')
        {
            animals_flag = 0;//HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, 100);
        }
        else if (c == '1')
        {
            animals_flag = 1;//HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, 100);
        }
//        else
//        {
//            animals_flag = 2;
//        }

        HAL_UART_Receive_IT(&huart2, k210_str, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
