#ifndef __ONENET_H__
#define __ONENET_H__

#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/********************************************************************/

/*****************************************************
 * 宏定义：接收APP下发数据的标识符
 * APP需要下发多少个，则定义多少个
 * 需要在onenet.c文件中的RecvData_Dispose()函数添加
 *****************************************************/
#define   Cat_Food         "Food"
#define   Dog_Food         "dogFood"
 
#define   Cat_Food_F         "Cat_Food_F"
#define   Dog_Food_F         "dogFood_F"
#define   Water_F          "Water_F"
#define   temp_F          "temp_f"
#define   temp          "temp"
#define   	dog								"dog"
#define    cat							"cat"
/***************************************
 * 1、接收APP数据
 * 2、需要修改结构体内参数对应标识符
 * 3、外部调用使用
 ***************************************/
typedef struct
{
	  uint32_t Cat_Food_data;
		uint32_t Dog_Food_data;
	
    uint32_t Cat_Food_F_data;
		uint32_t Dog_Food_F_data;
		uint32_t temp_data;
	
		uint32_t Dog_data;
		uint32_t Cat_data;
    
}APP_DATA;

extern APP_DATA app_data;
 
/*热点名称和密码*/
#define HOTSPOT_NAME       "C413C413"
#define HOTSPOT_PASS       "C413C413"

/*串口：需要配置DMA接收*/
#define UART huart3

/*Onenet云平台参数定义*/
#define ProductId  "kr2HSP5Z9J"
#define DeviceName "light"
#define Token      "version=2018-10-31&res=products%2FxT9O2pO2aI%2Fdevices%2FPetSystem&et=1779338301&method=md5&sign=kwUqxyWPZfh7y2mSNLGfdw%3D%3D"

/*****************************************************************************/

/*初始化函数*/
void Onenet_Init(void);

/**********************************************************************
 * 1、功能：组包函数
 * 2、参数：
 *      （1）Name：对应云平台物模型标识符
 *      （2）Data：需要发送的数据（unsigned char类型）
 *      （3） Cmd：参数输入：0或1（0：末尾一个数据；1：末尾前的数据）
 **********************************************************************/
void Get_Send_Pack(char *Name, uint32_t Data, unsigned char Cmd);
void Get_Send_float_Pack(char *Name, float Data, unsigned char Cmd);
/*发送数据函数*/
void SendData_Onenet(void);

/*接收数据函数*/
void RecvData_Onenet(void);

#endif

