

/*************************************
 * 1、功能：连接Onenet云平台.c文件
 * 
 * 2、操作：根据文档注释进行更改
 *
 * 3、作者：MH
 *
 ************************************/


#include "Onenet.h"

#include "oled.h"
/***********************以下内容不可修改*******************************************/

/*测试WIFI模块是否可用*/
#define WIFI_AT            "AT\r\n"

/*WIFI模块复位*/
#define WIFI_RST           "AT+RST\r\n"

/*WIFI模块的回显*/
#define WIFI_ATE           "ATE0\r\n"

/*WIFI模块模式选择*/
#define WIFI_MODE          "AT+CWMODE=1\r\n"

/*WIFI模块配置网络协议*/
#define WIFI_INTERNET      "AT+CWDHCP=1,1\r\n"

/*连接云平台服务器*/
#define CON_SERVER         "AT+MQTTCONN=0,\"183.230.40.96\",1883,1\r\n"

/*判断标识符*/
#define AT_POST            "post"
#define AT_OK              "OK"

/*发送AT指令次数*/
#define SEND_NUM    5

/*错误次数、用于调试*/
unsigned char Error_Num = 0;

/*发送、接收AT指令数组（不可修改）*/
unsigned char Send_AT_Data[400] = {0};
unsigned char Recv_AT_Data[400] = {0};

/*接收APP下发的所有数据，可增大*/
unsigned char Recv_APP_Buf[200] = {0};

/*发送数据组包数组*/
char Get_Pack[400] = {0};

/*接收用户使用的数据结构体*/
APP_DATA app_data;

/********************************************************************************/

/*******************************************
 * 1、APP下发数据前参数的初始化值
 * 2、可用可不用
 * 3、初始化内容为获取APP数据的结构体参数
 *******************************************/
void APP_Data_Init(void)
{
	  app_data.Cat_Food_data = 0;
		app_data.Dog_Food_data = 0;
	
    app_data.Cat_Food_F_data = 100;
		app_data.Dog_Food_F_data = 100;
    app_data.Water_F_data = 1400;
		app_data.temp_F_data = 30;
		app_data.Dog_data=0;
		app_data.Cat_data=0;
}

/**************************************************
 * 1、发送数据上云组包函数
 * 2、需要填写云平台数据标识符和参数
 * 3、不可修改
 * 4、参数：
 *  （1）上传数据的标识符（Name）
 *  （2）上传数据的参数（Data）
 *  （3）数据位置（0为最后一个数据，1为前面数据）
 *************************************************/
void Get_Send_Pack(char *Name, uint32_t Data, unsigned char Cmd)
{
    char dat[50];
    
    if(Cmd)
    {
        memset(dat, 0, sizeof(dat));   
				sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%d\\}\\,", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
							//{\\\"id\\\":\\\"123\\\"\\\,\\\"version\\\":\\\"1.0\\\"\\\,\\\"params\\\":{     \\\"cat\\\":{\\\"value\\\":0}      }}
    else
    {
        memset(dat, 0, sizeof(dat));
        sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%d\\}", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
}

//浮点数
void Get_Send_float_Pack(char *Name, float Data, unsigned char Cmd)
{
    char dat[50];
    
    if(Cmd)
    {
        memset(dat, 0, sizeof(dat));   
				sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%.1f\\}\\,", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
							
    else
    {
        memset(dat, 0, sizeof(dat));
        sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%.1f\\}", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
}

/*******************************************
 * 1、接收APP下发指令函数
 * 2、接收数据发布返回值
 * 3、需要在头文件定义接收标识符
 * 4、引用标识符判断
 * 5、必须修改（函数内注释）
 *******************************************/
unsigned char RecvData_Dispose(const unsigned char *Recv_Data)
{
	char str[20];
	HAL_UART_Transmit(&huart1,Recv_Data,strlen(Recv_Data),0x200);
    char *Dis_Buf;
    
    /*不可修改、接收发布返回值*/
    if(strstr((char *)Recv_Data, (const char *)AT_POST) != NULL)
    {
        return 0;
    }
    /*可修改宏定义标识符（例如：TEMP_F）可在头文件增删修，判断接收APP参数*/

    else if(strstr((char *)Recv_Data, (const char *)Cat_Food_F) != NULL)
    {
        Dis_Buf = strstr((char *)Recv_Data, (const char *)Cat_Food_F);
        Dis_Buf = strstr((char *)Dis_Buf, (const char *)":");
        app_data.Dog_Food_F_data = atoi(Dis_Buf+1);
			
        sprintf(str,"dfF:%d  ",app_data.Dog_Food_F_data);
        OLED_ShowString(25,7,str,8);
        return 0;
    }
		else if(strstr((char *)Recv_Data, (const char *)Dog_Food_F) != NULL)
    {
        Dis_Buf = strstr((char *)Recv_Data, (const char *)Dog_Food_F);
        Dis_Buf = strstr((char *)Dis_Buf, (const char *)":");
        app_data.Cat_Food_F_data = atoi(Dis_Buf+1);
			
        sprintf(str,"cfF:%d  ",app_data.Cat_Food_F_data);
        OLED_ShowString(0,6,str,8);
        return 0;
    }
		else if(strstr((char *)Recv_Data, (const char *)Water_F) != NULL)
    {
        Dis_Buf = strstr((char *)Recv_Data, (const char *)Water_F);
        Dis_Buf = strstr((char *)Dis_Buf, (const char *)":");
        app_data.Water_F_data = atoi(Dis_Buf+1);
        sprintf(str,"wF:%d  ",app_data.Water_F_data);
       OLED_ShowString(72,6,str,8);
        return 0;
    }
		
		else if(strstr((char *)Recv_Data, (const char *)temp_F) != NULL)
    {
        Dis_Buf = strstr((char *)Recv_Data, (const char *)temp_F);
        Dis_Buf = strstr((char *)Dis_Buf, (const char *)":");
        app_data.temp_F_data = atoi(Dis_Buf+1);
				sprintf(str,"tF:%d     ",app_data.temp_F_data);
        OLED_ShowString(83,7,str,8);
        return 0;
    }

		else
        return 0;
}

/*********************************************
 * 1、连接云平台AT指令处理函数
 * 2、不可修改
 *********************************************/
unsigned char AT_Dispose(const unsigned char *Recv_Data, const unsigned char *Find_Data)
{
    if(NULL == strstr((char *)Recv_Data, (char *)Find_Data))
        return 1;
    
    return 0;
}

/**************************************************
 * 1、中断接收数据函数
 * 2、接收数据发布返回值和APP下发数据
 * 3、无需修改
 * 4、需要调用（在中断服务函数中调用）
 *************************************************/
void RecvData_Onenet(void)
{
    if(__HAL_UART_GET_FLAG(&UART, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&UART);
        HAL_UART_DMAStop(&UART);
        RecvData_Dispose(Recv_APP_Buf);
//HAL_UART_Transmit(&huart1, Recv_APP_Buf,200, 500);//串口1调试
        memset(Recv_APP_Buf, 0, sizeof(Recv_APP_Buf));
        HAL_UART_Receive_DMA(&UART,Recv_APP_Buf,200);
    }
}

/******************************************************
 * 1、发送数据上云接口函数
 * 2、无需修改
 * 3、需要在main.c中调用发数据
 * 4、注意：发送速度不宜过快，控制在1s左右
 ******************************************************/
void SendData_Onenet(void)
{
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    
    /*包头：不可修改*/
    sprintf((char *)Send_AT_Data, "AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"3\\\"\\,\\\"params\\\":{", ProductId, DeviceName);
    /*数据*/
    strcat((char *)Send_AT_Data, (char *)Get_Pack);
    /*包尾*/
    strcat((char *)Send_AT_Data, "}}\",0,0\r\n");
    
    /*清空组包数组*/
    memset(Get_Pack, 0, sizeof(Get_Pack));
    
    HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 3000);
//		HAL_UART_Transmit(&huart1, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);//串口1调试
//	HAL_UART_Transmit(&huart1,"777",3, 500);
}

/****************************************
 * 1、设置发送数据上云平台
 * 2、不可修改
 * 3、无需调用
 ***************************************/
void Wifi_Set_SendData(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, "AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",1\r\n", ProductId, DeviceName);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 6, 3000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 6, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/**********************************************
 * 1、设置订阅APP数据函数
 * 2、不可修改
 * 3、无需调用
 **********************************************/
void Wifi_Set_RecvData(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, "AT+MQTTSUB=0,\"$sys/%s/%s/#\",0\r\n", ProductId, DeviceName);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 49, 3000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 49, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/*******************************************
 * 1、连接云平台服务器函数
 * 2、不可修改
 * 3、无需调用
 *******************************************/
void Wifi_Connet_Server(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, CON_SERVER);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 59, 3000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
        HAL_UART_Transmit(&huart1, Recv_AT_Data, 59, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/*****************************************
 * 1、连接云平台指定设备函数
 * 2、不可修改
 * 3、无需调用
 *****************************************/
void Wifi_Connet_Device(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",DeviceName,ProductId,Token);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 6, 3000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 6, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/****************************************
 * 1、连接热点函数
 * 2、不可修改
 * 3、无需调用
 ****************************************/
void Wifi_Connet_Hotspot(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, "AT+CWJAP=\"%s\",\"%s\"\r\n", HOTSPOT_NAME, HOTSPOT_PASS);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 52, 5000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
				HAL_UART_Transmit(&huart1, Recv_AT_Data, 52, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/****************************************
 * 1、设置配置网络函数
 * 2、不可修改
 * 3、无需调用
 ****************************************/
void Wifi_Internet(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, WIFI_INTERNET);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 6, 2000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 6, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/****************************************
 * 1、设置WIFI模块模式函数
 * 2、不可修改
 * 3、无需调用
 ****************************************/
void Wifi_Mode(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, WIFI_MODE);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 6, 2000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 6, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/****************************************
 * 1、测试WIFI模块是否可用函数
 * 2、不可修改
 * 3、无需调用
 ****************************************/
void Wifi_At(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, WIFI_AT);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
				
        HAL_UART_Receive(&UART, Recv_AT_Data, 10, 2000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1,Recv_AT_Data,10, 200);//串口1调试
    }while(Res && Send_Out_Num--);
    
    /*关闭回显*/
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, WIFI_ATE);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 13, 2000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 13, 500);//串口1调试
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/****************************************
 * 1、WIFI模块复位函数
 * 2、不可修改
 * 3、无需调用
 ****************************************/
void Wifi_Rst(void)
{
    unsigned char Send_Out_Num = SEND_NUM;
    unsigned char Res = 0;
    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
    sprintf((char *)Send_AT_Data, WIFI_RST);
    
    do
    {
        memset(Recv_AT_Data, 0, sizeof(Recv_AT_Data));
        HAL_UART_Transmit(&UART, Send_AT_Data, strlen((const char*)Send_AT_Data), 500);
        HAL_UART_Receive(&UART, Recv_AT_Data, 14, 2000);
        Res = AT_Dispose(Recv_AT_Data, (unsigned char *)AT_OK);
			HAL_UART_Transmit(&huart1, Recv_AT_Data, 14, 500);
    }while(Res && Send_Out_Num--);
    
    if(Res == 1)
        Error_Num++;
}

/*************************************************
 * 1、WIFI模块初始化函数
 * 2、不可修改
 * 3、需在main.c初始化时调用
 * 4、上电时需先打开热点、设置热点名称、密码
 *************************************************/
void Onenet_Init(void)
{
    Wifi_Rst();
    
    Wifi_At();
    
    Wifi_Mode();
    
    Wifi_Internet();
    
    Wifi_Connet_Hotspot();
    
    Wifi_Connet_Device();
    
    Wifi_Connet_Server();
    
    Wifi_Set_RecvData();
    
    Wifi_Set_SendData();
    
    APP_Data_Init();

    /*开启中断接收数据*/
    HAL_UART_Receive_DMA(&UART,Recv_APP_Buf,200);

	__HAL_UART_ENABLE_IT(&UART,UART_IT_IDLE);

}
