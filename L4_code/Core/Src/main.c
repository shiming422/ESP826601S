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
#include "dma.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"	
#include "list.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t LCD_Show_string_L_MIN[50];
uint8_t LCD_Show_string_L_MAX[50];
uint8_t LCD_Show_string_A_MIN[50];
uint8_t LCD_Show_string_A_MAX[50];
uint8_t LCD_Show_string_B_MIN[50];
uint8_t LCD_Show_string_B_MAX[50];

uint8_t choose_log[20];
uint8_t mode_str[10];

LAB_list *L_MIN,*L_MAX,*A_MIN,*A_MAX,*B_MIN,*B_MAX;

uint8_t key_state=0;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint16_t line_buffer[320];

lcd_io lcd_io_desc = {
    .spi = &hspi1,
    .rst = {LCD_RST_GPIO_Port, LCD_RST_Pin, 0},
    .bl  = {LCD_POWER_GPIO_Port, LCD_POWER_Pin, 0},
    .cs  = {LCD_CS_GPIO_Port, LCD_CS_Pin, 0},
    .dc  = {LCD_DC_GPIO_Port,  LCD_DC_Pin,  0},
    .te  = { /* TE */ }
};

lcd lcd_desc = {
    .io = &lcd_io_desc,
    .line_buffer = line_buffer,
};



HAL_SD_CardInfoTypeDef  SDCardInfo;

void printf_sdcard_info(void)
{
	uint64_t CardCap;      	//SD
	HAL_SD_CardCIDTypeDef SDCard_CID; 

	HAL_SD_GetCardCID(&hsd1,&SDCard_CID);	//CID
	HAL_SD_GetCardInfo(&hsd1,&SDCardInfo);                    //???SD?????
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//????SD??????
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
				printf("Card Type:SDSC V1\r\n");
			else if(SDCardInfo.CardVersion == CARD_V2_X)
				printf("Card Type:SDSC V2\r\n");
		}
		break;
		case CARD_SDHC_SDXC:printf("Card Type:SDHC\r\n");break;
		default:break;
	}	
		
  printf("Card ManufacturerID: %d \r\n",SDCard_CID.ManufacturerID);				//??????ID	
 	printf("CardVersion:         %d \r\n",(uint32_t)(SDCardInfo.CardVersion));		//??????
	printf("Class:               %d \r\n",(uint32_t)(SDCardInfo.Class));		    //
 	printf("Card RCA(RelCardAdd):%d \r\n",SDCardInfo.RelCardAdd);					//???????
	printf("Card BlockNbr:       %d \r\n",SDCardInfo.BlockNbr);						//??????
 	printf("Card BlockSize:      %d \r\n",SDCardInfo.BlockSize);					//????
	printf("LogBlockNbr:         %d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));		//?????????
	printf("LogBlockSize:        %d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));		//???????
	printf("Card Capacity:       %d MB\r\n",(uint32_t)(CardCap>>20));				//??????
}




char uart1_uart_receive_buff[255];    
char uart3_uart_receive_buff[255]; 
uint8_t UART_Recv_IT[5];
char LAB_Transmit_Buff[255];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t sd_datus;
	uint8_t sd_buf;
	int count=0;
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	
	
	/*****************FATFS文件系统初始化配置****************************/
	SD_Driver.disk_initialize(0);
	printf_sdcard_info();
		sd_datus = SD_Driver.disk_write(0,"SD CARD TEST",20,2);
	
	printf("sd write result:%d\r\n",sd_datus);
	sd_datus = SD_Driver.disk_read(0,&sd_buf,20,2);
	printf("sd reak result:%d\r\n",sd_datus);
	printf("sd read content:\r\n%s\r\n",&sd_buf);
	/*****************************************************************/
	/****************LCD屏幕初始化配置*********************/
  lcd_init_dev(&lcd_desc, LCD_1_47_INCH, LCD_ROTATE_90);
	//lcd_fill(&lcd_desc,0,0,480,240,WHITE);
	/****************串口空闲中断+DMA初始化配置*********************/
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*)uart1_uart_receive_buff, 255);
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart3, (uint8_t*)uart3_uart_receive_buff, 255);
	/*************LAB阈值调节参数设置*************/
	
	L_MIN=(LAB_list *)malloc(sizeof(LAB_list));
	L_MAX=(LAB_list *)malloc(sizeof(LAB_list));
	A_MIN=(LAB_list *)malloc(sizeof(LAB_list));
	A_MAX=(LAB_list *)malloc(sizeof(LAB_list));
	B_MIN=(LAB_list *)malloc(sizeof(LAB_list));
	B_MAX=(LAB_list *)malloc(sizeof(LAB_list));
	
	
	list_init(&(L_MIN->list));
	list_insert_after(&(L_MIN->list),&(L_MAX->list));
	list_insert_after(&(L_MAX->list),&(A_MIN->list));
	list_insert_after(&(A_MIN->list),&(A_MAX->list));
	list_insert_after(&(A_MAX->list),&(B_MIN->list));
	list_insert_after(&(B_MIN->list),&(B_MAX->list));
	
	L_MIN->count=45;
	L_MAX->count=65;
	A_MIN->count=-10;
	A_MAX->count=55;
	B_MIN->count=0;
	B_MAX->count=40;
	
	L_MIN->show_line=10;
	L_MAX->show_line=35;
	A_MIN->show_line=60;
	A_MAX->show_line=85;
	B_MIN->show_line=110;
	B_MAX->show_line=135;

	now_list=L_MIN;
	//lcd_draw_rectangle(&lcd_desc, 10, now_list->show_line-5, 120,now_list->show_line+20, RED);
	/*****************************************************************/
	int i=0;
	char str[20];
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2,1);
	uint8_t road_str[20];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		//HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
		HAL_UART_Receive(&huart2,road_str,4,100);
		if(road_str[0]=='M')
			if(road_str[1]=='1')
				HAL_UART_Transmit(&huart3,(uint8_t*)'1',1,100);
			if(road_str[1]=='2')
				HAL_UART_Transmit(&huart3,(uint8_t*)'2',1,100);
			if(road_str[1]=='3')
				HAL_UART_Transmit(&huart3,(uint8_t*)'3',1,100);
			if(road_str[1]=='4')
				HAL_UART_Transmit(&huart3,(uint8_t*)'4',1,100);
		lcd_print(&lcd_desc,20,20,(char *)road_str);
//		i=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
//		sprintf(str,"%d   ",i);
//		lcd_print(&lcd_desc,20,20,(char *)str);
		
			
//		/**************lcd交互设计***************/
//		sprintf((char *)L_MIN->lcd_show_string,"L_MIN:%d  ",L_MIN->count);
//		sprintf((char *)L_MAX->lcd_show_string,"L_MAX:%d  ",L_MAX->count);
//		sprintf((char *)A_MIN->lcd_show_string,"A_MIN:%d  ",A_MIN->count);
//		sprintf((char *)A_MAX->lcd_show	_string,"A_MAX:%d  ",A_MAX->count);
//		sprintf((char *)B_MIN->lcd_show_string,"B_MIN:%d  ",B_MIN->count);
//		sprintf((char *)B_MAX->lcd_show_string,"B_MAX:%d  ",B_MAX->count);
//		
//		if(now_list->mode==1)
//		{
//			lcd_draw_line(&lcd_desc, 300, 25, 300, 45, YELLOW);
//			lcd_draw_line(&lcd_desc, 290, 35, 310, 35, YELLOW);
//		}
//		else
//		{
//			lcd_draw_line(&lcd_desc, 300, 25, 300, 45, BLACK);
//			lcd_draw_line(&lcd_desc, 290, 35, 310, 35, YELLOW);
//		}
//		lcd_print(&lcd_desc,20,L_MIN->show_line,(char *)L_MIN->lcd_show_string);
//		lcd_print(&lcd_desc,20,L_MAX->show_line,(char *)L_MAX->lcd_show_string);
//		lcd_print(&lcd_desc,20,A_MIN->show_line,(char *)A_MIN->lcd_show_string);
//		lcd_print(&lcd_desc,20,A_MAX->show_line,(char *)A_MAX->lcd_show_string);
//		lcd_print(&lcd_desc,20,B_MIN->show_line,(char *)B_MIN->lcd_show_string);
//		lcd_print(&lcd_desc,20,B_MAX->show_line,(char *)B_MAX->lcd_show_string);
//		if(last_list!=NULL)
//			lcd_draw_rectangle(&lcd_desc, 10, last_list->show_line-3,120,last_list->show_line+18, BLACK);
//		lcd_draw_rectangle(&lcd_desc, 10, now_list->show_line-3,120,now_list->show_line+18, RED);
		/********************************/
		
		
		/**************串口数据发送******************/
//		sprintf(LAB_Transmit_Buff,"LMIN%dLMAX%dAMIN%dAMAX%dBMIN%dBMAX%d",L_MIN->count,L_MAX->count,A_MIN->count,A_MAX->count,B_MIN->count,B_MAX->count);
//	
//		HAL_UART_Transmit(&huart2,(uint8_t *)LAB_Transmit_Buff,strlen(LAB_Transmit_Buff),0x200); 
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
	
	/********CRC校验验证*********/
	char rece_buff[20];
	while(0)
	{
		HAL_UART_Receive(&huart2,(uint8_t *)rece_buff,1,100);
		//HAL_UART_Transmit(&huart2,(uint8_t *)"00",7,0x200);
		lcd_print(&lcd_desc,20,10,(char *)rece_buff);		
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  if(huart ==&huart3)
	  {
		  	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);
		  HAL_UART_Receive_IT(&huart3, UART_Recv_IT, 4);
	  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 key_state=__builtin_ctz(GPIO_Pin);
		HAL_TIM_Base_Start_IT(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint16_t pin_mask = (1U << key_state);
   if(htim==&htim1)
  {
				HAL_TIM_Base_Stop_IT(&htim1);
       if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,pin_mask)==0)
			 {
				 HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);

				 if(key_state-9==1)
				 {
					 last_list=now_list;
					 list_find_before(now_list);
				 }					 
				 else if(key_state-9==2)
				 {
					 last_list=now_list;
					 list_find_next(now_list);
				 }
				 else if(key_state-9==3)
				 {
					 if(now_list->mode==0)
						 now_list->mode=1;
					 else if(now_list->mode==1)
						 now_list->mode=0;
				 }
				 else if(key_state-9==4)
				 {
					if(now_list->mode==1)
						now_list->count++;
					else
						now_list->count--;
				 }
			}
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
