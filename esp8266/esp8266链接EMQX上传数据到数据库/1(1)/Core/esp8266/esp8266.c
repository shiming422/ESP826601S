#include "main.h" // Device header
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "esp8266.h"
/******************************************************
 * 1、发送数据上云接口函数
 * 2、无需修改
 * 3、需要在main.c中调用发数据
 * 4、注意：发送速度不宜过快，控制在1s左右
 ******************************************************/
//void SendData_Onenet(void)
//{
//    memset(Send_AT_Data, 0, sizeof(Send_AT_Data));
//    
//    /*包头：不可修改*/
//    sprintf((char *)Send_AT_Data, "AT+MQTTPUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"version\\\":\\\"1.0\\\"\\\,\\\"params\\\":{" );
//    /*数据*/														
//    strcat((char *)Send_AT_Data, (char *)Get_Pack);
//    /*包尾*/
//    strcat((char *)Send_AT_Data, "}}\",0,0");
//    
//    /*清空组包数组*/
//    memset(Get_Pack, 0, sizeof(Get_Pack));
//    esp_Cmd(Send_AT_Data,"success");
//}
/**
 * @brief 判断主字符串中是否包含子字符串
 *
 * @param main_string 主字符串
 * @param substring 子字符串
 * @return int 如果包含子字符串返回1，否则返回0
 */
int contains_substring(const char *main_string, const char *substring)
{
	// 如果strstr()返回非空指针，则表示找到了子字符串
	if (strstr((const char *)main_string, substring) != NULL)
	{
		return 1; // 找到子字符串
	}
	else
	{
		return 0; // 未找到子字符串
	}
}

/**
 * @brief 复位ESP8266模块
 *
 */
void esp_Rst(void)
{
	memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 清空接收缓冲区
	HAL_UART_Transmit(&huart6,(const uint8_t *)"AT+RST\r\n",strlen("AT+RST\r\n"),100);// 发送复位指令
	//printf("AT+RST\r\n");						   
	while (0 == contains_substring(esp8266_RxBuffer, "OK\r\n"));// 等待模块响应"OK\r\n"
	memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 再次清空接收缓冲区
}

/**
 * @brief 向ESP8266发送指令并等待响应
 *
 * @param str 要发送的指令
 * @param res 期望的响应
 * @return int 执行结果，始终返回0
 */
int esp_Cmd(const char *str, const char *res)
{
	
	HAL_UART_Transmit(&huart6,(const uint8_t *)str,strlen(str),100);// 发送指令
	
	while (0 == contains_substring(esp8266_RxBuffer, res)) // 等待期望的响应
	{
		memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 清空接收缓冲区
		//printf("%s\r\n", esp8266_RxBuffer);						   
		HAL_Delay(80);
		// 等待响应
	}
	memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 清空接收缓冲区

	return 0;
}

/******************/
char esp_Init(void)
{
	/* 1.复位指令 */
	esp_Rst();

	/* 2.设置为station模式 */
	esp_Cmd("AT+CWMODE=1", "OK");

	/* 3.启动DHCP */
	esp_Cmd("AT+CWDHCP=1,1", "OK");

	/* 4.连接热点 */
	esp_Cmd("AT+CWJAP=\"C413C413\",\"C413C413\"", "OK");

	/* 5.配置MQTT用户信息 */
	esp_Cmd("AT+MQTTUSERCFG=0,1,\"road\",\"road\",\"Jit666666!\",0,0,\"\"\r\n", "OK");

	/* 6.建立MQTT连接 */
	esp_Cmd("AT+MQTTCONN=0,\"122.51.67.189\",1883,1\r\n","OK");

	/* 7.订阅主题 */
	// 主题用于接收服务器对客户端发布消息的回复
	//esp_Cmd("AT+MQTTSUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post/reply\",1", "OK");

	// 主题用于接收服务器下发的属性设置命令
	//esp_Cmd("AT+MQTTSUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/set\",1", "OK");

	/* 8.发布消息 */
	//esp_Cmd("AT+MQTTPUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\\,\\\"version\\\":\\\"1.0\\\"\\\,\\\"params\\\":{\\\"cat\\\":{\\\"value\\\":0}}}\",0,0", "success");

	return 0;
}