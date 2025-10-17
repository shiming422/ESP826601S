#include "bujin.h"

void MOTOR_CONTROL(uint16_t direction)//对电机进行控制，使用八拍
{
	static uint8_t step=0;
	if(1== direction)//控制电机方向
	{
		if(0==step)
		{
			step=8;
		}
		step--;
	}	
	else if(0== direction)
	{
		step++;
		if(8==step)
		{
			step=0;
		}
	}
	if(0==step)
	{
		MOTOR_A_H;
		MOTOR_B_L;
		MOTOR_C_L;
		MOTOR_D_L;
	}
	else if(1==step)
	{
		MOTOR_A_H;
		MOTOR_B_H;
		MOTOR_C_L;
		MOTOR_D_L;
	}
	else if(2==step)
	{
		MOTOR_A_L;
		MOTOR_B_H;
		MOTOR_C_L;
		MOTOR_D_L;
	}
	else if(3==step)
	{
		MOTOR_A_L;
		MOTOR_B_H;
		MOTOR_C_H;
		MOTOR_D_L;
	}
	else if(4==step)
	{
		MOTOR_A_L;
		MOTOR_B_L;
		MOTOR_C_H;
		MOTOR_D_L;
	}
	else if(5==step)
	{
		MOTOR_A_L;
		MOTOR_B_L;
		MOTOR_C_H;
		MOTOR_D_H;
	}
	else if(6==step)
	{
		MOTOR_A_L;
		MOTOR_B_L;
		MOTOR_C_L;
		MOTOR_D_H;
	}
	else if(7==step)
	{
		MOTOR_A_H;
		MOTOR_B_L;
		MOTOR_C_L;
		MOTOR_D_H;
	}
}
 
void Step_MOTOR_Start(uint16_t angle,uint8_t direction)//电机启动
{
	int i=0;
	int pulse =(int)((float)(angle*64/5.625));//脉冲与步距角的关系
	for(i=0;i<pulse;i++)
	{
		MOTOR_CONTROL(direction);
		HAL_Delay(2);
	}
}
 
void Step_MOTOR_Stop(void)//四相都是低电平代表电机停止
{
	MOTOR_A_L;
	MOTOR_B_L;
	MOTOR_C_L;
	MOTOR_D_L;
}






void r_MOTOR_CONTROL(uint16_t direction)//对电机进行控制，使用八拍
{
	static uint8_t step=0;
	if(1== direction)//控制电机方向
	{
		if(0==step)
		{
			step=8;
		}
		step--;
	}	
	else if(0== direction)
	{
		step++;
		if(8==step)
		{
			step=0;
		}
	}
	if(0==step)
	{
		r_MOTOR_A_H;
		r_MOTOR_B_L;
		r_MOTOR_C_L;
		r_MOTOR_D_L;
	}
	else if(1==step)
	{
		r_MOTOR_A_H;
		r_MOTOR_B_H;
		r_MOTOR_C_L;
		r_MOTOR_D_L;
	}
	else if(2==step)
	{
		r_MOTOR_A_L;
		r_MOTOR_B_H;
		r_MOTOR_C_L;
		r_MOTOR_D_L;
	}
	else if(3==step)
	{
		r_MOTOR_A_L;
		r_MOTOR_B_H;
		r_MOTOR_C_H;
		r_MOTOR_D_L;
	}
	else if(4==step)
	{
		r_MOTOR_A_L;
		r_MOTOR_B_L;
		r_MOTOR_C_H;
		r_MOTOR_D_L;
	}
	else if(5==step)
	{
		r_MOTOR_A_L;
		r_MOTOR_B_L;
		r_MOTOR_C_H;
		r_MOTOR_D_H;
	}
	else if(6==step)
	{
		r_MOTOR_A_L;
		r_MOTOR_B_L;
		r_MOTOR_C_L;
		r_MOTOR_D_H;
	}
	else if(7==step)
	{
		r_MOTOR_A_H;
		r_MOTOR_B_L;
		r_MOTOR_C_L;
		r_MOTOR_D_H;
	}
}
 
void r_Step_MOTOR_Start(uint16_t angle,uint8_t direction)//电机启动
{
	int i=0;
	int pulse =(int)((float)(angle*64/5.625));//脉冲与步距角的关系
	for(i=0;i<pulse;i++)
	{
		r_MOTOR_CONTROL(direction);
		HAL_Delay(2);
	}
}
 
void r_Step_MOTOR_Stop(void)//四相都是低电平代表电机停止
{
	r_MOTOR_A_L;
	r_MOTOR_B_L;
	r_MOTOR_C_L;
	r_MOTOR_D_L;
}
