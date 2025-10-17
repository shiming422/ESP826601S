#ifndef __BUJIN_H__
#define __BUJIN_H__

#include "headfile.h"

#define MOTOR_A_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
#define MOTOR_A_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	
#define MOTOR_B_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
#define MOTOR_B_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	
#define MOTOR_C_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
#define MOTOR_C_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	
#define MOTOR_D_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
#define MOTOR_D_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);


void MOTOR_CONTROL(uint16_t direction);//对电机进行控制，使用八拍
void Step_MOTOR_Start(uint16_t angle,uint8_t direction);//电机启动
void Step_MOTOR_Stop(void);//四相都是低电平代表电机停止

#endif