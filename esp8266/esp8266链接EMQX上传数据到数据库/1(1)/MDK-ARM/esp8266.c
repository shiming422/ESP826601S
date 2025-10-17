#include "main.h" // Device header
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "esp8266.h"


extern int car_number;
extern int people_animals_number;
extern int break_number;
extern int sundries_number;
extern int device_id;

extern char carrier[];
extern char plate[];
extern char Violation_category[];

extern double speed;
extern double weight;
extern double temperature;
extern double humidity;
extern double air;
extern double longitude;
extern double latitude;

void Save_data() {
    char json[] = "\"{\\\"car_number\\\":%d,\\\"people_animals_number\\\":%d,\\\"break_number\\\":%d,\\\"sundries_number\\\":%d,\\\"device_id\\\":%d,\\\"carrier\\\":\\\"%s\\\",\\\"plate\\\":\\\"%s\\\",\\\"Violation_category\\\":\\\"%s\\\",\\\"speed\\\":%lf,\\\"weight\\\":%lf,\\\"temperature\\\":%lf,\\\"humidity\\\":%lf,\\\"air\\\":%lf,\\\"longitude\\\":%lf,\\\"latitude\\\":%lf}\"";
    
		char t_json[512];	
		snprintf(t_json, sizeof(t_json), json, car_number,people_animals_number,break_number,sundries_number,device_id,carrier,plate,Violation_category,speed,weight,temperature,humidity,air,longitude,latitude);
    unsigned int json_len = strlen(t_json);
		
		char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+MQTTPUBRAW=0,\"road\",%d,0,0\r\n", json_len);
    printf("%s",cmd);
	  uint8_t retry = 0;
    while(strstr(esp8266_RxBuffer, ">") == NULL);
			printf("%s",t_json);
    HAL_Delay(300);
	 while(strstr(esp8266_RxBuffer, "+MQTTPUB:OK") == NULL);
}


void Str_Pub_Msgdata(char *message_name,char *message_data) 
{
	  char send_str[255];
		sprintf(send_str,"{\\\"%s\\\":\\\"%s\\\"}",message_name,message_data);
		
    printf("AT+MQTTPUB=0,\"road\",\"%s\",0,0\r\n", send_str);
    memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer));
    HAL_Delay(80);
}

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
		return 1; // 找到子字符串
	else
		return 0; // 未找到子字符串
}

/**
 * @brief 复位ESP8266模块
 *
 */
void esp_Rst(void)
{
	memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 清空接收缓冲区
	
	printf("AT\r\n");		
	HAL_UART_Transmit(&huart1,(const uint8_t *)esp8266_RxBuffer,strlen(esp8266_RxBuffer),100);// 发送复位指令	
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
	printf("%s\r\n", str);
	
	while (0 == contains_substring(esp8266_RxBuffer, res)) // 等待期望的响应
	{
		memset(esp8266_RxBuffer, 0, sizeof(esp8266_RxBuffer)); // 清空接收缓冲区
		   
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

	return 0;
}