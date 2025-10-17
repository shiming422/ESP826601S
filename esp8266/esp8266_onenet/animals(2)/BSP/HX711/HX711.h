#ifndef _HX711_H_
#define _HX711_H_
#include "headfile.h"
 
#define CLK_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1)
#define CLK_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0)
#define Read_PIN HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
unsigned long Get_number(void);
long Get_Weight(void);
 
 
#define l_CLK_1 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,1)
#define l_CLK_0 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,0)
#define l_Read_PIN HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)
unsigned long Get_number_l(void);
long Get_Weight_l(void);
#endif