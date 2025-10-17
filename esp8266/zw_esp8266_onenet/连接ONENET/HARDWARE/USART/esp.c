#include "stm32f10x.h" // Device header
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "esp.h"

#include "led.h"

#define ProductId  "xT9O2pO2aI"
#define DeviceName "PetSystem"

// OneNet平台参数
#define PRODUCT_ID    "13t0wKIn7p"
#define DEVICE_NAME   "Pet"
#define DEVICE_TOKEN  "version=2018-10-31&res=products%%2F13t0wKIn7p%%2Fdevices%%2FPet&et=1779338301&method=sha1&sign=BBmQvZGwQs8c2kAXKViAW8vmIUE%%3D"

// Wi-Fi配置
#define WIFI_SSID     "C413C413"
#define WIFI_PASSWORD "C413C413"

extern u8 USART_RX_BUF[];
/*发送、接收AT指令数组（不可修改）*/
unsigned char Send_AT_Data[400] = {0};
char Get_Pack[400] = {0};


//void upload_data_to_onenet() {
//    // 1. 连接Wi-Fi
//    char cmd[128];
//    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID, WIFI_PASSWORD);
//    if (!esp_Cmd(cmd, "OK")) {
//        DEBUG_PRINT("Wi-Fi连接失败\n");
//        return;
//    }

//    // 2. 建立TCP连接（假设平台支持HTTP，否则需处理SSL）
//    if (!esp_Cmd("AT+CIPSTART=\"TCP\",\"open.iot.10086.cn\",80", "OK")) {
//        DEBUG_PRINT("TCP连接失败\n");
//        return;
//    }

//    // 3. 构造POST请求
//    const char *json_data = 
//        "{"
//        "\"id\":\"123\","
//        "\"version\":\"1.0\","
//        "\"params\":{"
//            "\"cat\":{\"value\":0},"
//            "\"dog\":{\"value\":0}"
//        "}"
//        "}";

//    char http_request[512];
//    snprintf(http_request, sizeof(http_request),
//        "POST /fuse/http/device/thing/property/post?%s HTTP/1.1\r\n"
//        "Host: open.iot.10086.cn\r\n"
//        "Content-Type: application/json\r\n"
//        "Content-Length: %d\r\n\r\n"
//        "%s",
//        DEVICE_TOKEN,
//        (int)strlen(json_data),
//        json_data
//    );

//    // 4. 发送数据
//    char cipsend_cmd[32];
//    sprintf(cipsend_cmd, "AT+CIPSEND=%d", (int)strlen(http_request));
//    if (!esp_Cmd(cipsend_cmd, ">")) {
//        DEBUG_PRINT("CIPSEND失败\n");
//        return;
//    }
//    if (!esp_Cmd(http_request, "OK")) {
//        DEBUG_PRINT("数据发送失败\n");
//    }

//    // 5. 关闭连接
//    esp_Cmd("AT+CIPCLOSE", "OK");
//}




/**************************************************
 * 1、发送数据上云组包函数
 * 2、需要填写云平台数据标识符和参数
 * 3、不可修改
 * 4、参数：
 *  （1）上传数据的标识符（Name）
 *  （2）上传数据的参数（Data）
 *  （3）数据位置（0为最后一个数据，1为前面数据）
 *************************************************/
void Get_Send_Pack(char *Name, unsigned char Data, unsigned char Cmd)
{
    char dat[50];
    
    if(Cmd)
    {
        memset(dat, 0, sizeof(dat));   
				sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%d}", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
							
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
				sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%f}", Name, Data);
        strcat((char *)Get_Pack, dat);
    }
							
    else
    {
        memset(dat, 0, sizeof(dat));
        sprintf(dat, "\\\"%s\\\":{\\\"value\\\":%f\\}", Name, Data);
        strcat((char *)Get_Pack, dat);
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
    sprintf((char *)Send_AT_Data, "AT+MQTTPUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"version\\\":\\\"1.0\\\"\\\,\\\"params\\\":{" );
    /*数据*/														
    strcat((char *)Send_AT_Data, (char *)Get_Pack);
    /*包尾*/
    strcat((char *)Send_AT_Data, "}}\",0,0");
    
    /*清空组包数组*/
    memset(Get_Pack, 0, sizeof(Get_Pack));
    esp_Cmd(Send_AT_Data,"success");
}
/**
 * @brief 判断主字符串中是否包含子字符串
 *
 * @param main_string 主字符串
 * @param substring 子字符串
 * @return int 如果包含子字符串返回1，否则返回0
 */
int contains_substring(const u8 *main_string, const char *substring)
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
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 清空接收缓冲区
	printf("AT+RST\r\n");						   // 发送复位指令
	while (0 == contains_substring(USART_RX_BUF, "OK\r\n"))
		;										   // 等待模块响应"OK\r\n"
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 再次清空接收缓冲区
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
	while (0 == contains_substring(USART_RX_BUF, res)) // 等待期望的响应
	{
		memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 清空接收缓冲区
		printf("%s\r\n", str);						   // 发送指令
		delay_ms(1000);
		// 等待响应
	}
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 清空接收缓冲区

	return 0;
}

/**
 * @brief 初始化ESP8266模块
 *
 * @return char 初始化结果，始终返回0
 */
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
	esp_Cmd("AT+MQTTUSERCFG=0,1,\"PetSystem\",\"xT9O2pO2aI\",\"version=2018-10-31&res=products%2FxT9O2pO2aI%2Fdevices%2FPet&et=1779338301&method=md5&sign=GJH7xQg%2Bqy5Yz61sV2HFIA%3D%3D\",0,0,\"\"", "OK");

	/* 6.建立MQTT连接 */
	//esp_Cmd("AT+MQTTCONN=0,\"183.230.40.96\",1883,1","OK");
	esp_Cmd("AT+MQTTCONN=0,\"183.230.40.96\",1883,1","OK");
	//AT+MQTTCONN=0,"183.230.40.96",1883,0
//mqtts.heclouds.com
	//esp_Cmd("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1","OK");
	/* 7.订阅主题 */
	// 主题用于接收服务器对客户端发布消息的回复
	esp_Cmd("AT+MQTTSUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post/reply\",1", "OK");

	// esp_Cmd("AT+MQTTSUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post/set_reply\",1", "OK");

	// 主题用于接收服务器下发的属性设置命令
	esp_Cmd("AT+MQTTSUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/set\",1", "OK");

	/* 8.发布消息 */
	//esp_Cmd("AT+MQTTPUB=0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\\,\\\"version\\\":\\\"1.0\\\"\\\,\\\"params\\\":{\\\"cat\\\":{\\\"value\\\":0}}}\",0,0", "success");

	return 0;
}


/**
 * @brief 检查接收到的数据中是否包含特定的设备和属性。
 *
 * @param res1 设备名或关键字，用于识别设备。
 * @param res2 属性值，用于识别设备的属性状态。
 * @return int 如果接收到的数据中包含指定的设备和属性，则返回1，否则返回0。
 */
int esp_Get(const char *res1, const char *res2)
{
	// 示例接收数据格式："+MQTTSUBRECV:0,\"$sys/xT9O2pO2aI/PetSystem/thing/property/set\",58,{\"id\":\"58\",\"version\":\"1.0\",\"params\":{\"LightSwitch\":false}}"

	/* 检查接收缓冲区中是否包含指定的设备名或关键字 */
	if (1 == contains_substring(USART_RX_BUF, res1))
	{
		/* 如果包含设备名，再检查是否包含指定的属性值 */
		if (1 == contains_substring(USART_RX_BUF, res2))
		{
			return 1; // 找到匹配的设备和属性
		}
	}
	return 0; // 未找到匹配的设备和属性
}

/**
 * @brief 根据接收到的数据设置PetSystem的状态。
 *
 * @return int 返回处理结果，此处没有具体实现返回值，实际应用中可能需要返回状态。
 */
int esp_Get_LED(void)
{

	if (1 == esp_Get("LightSwitch", "false"))
	{
		alert_on();
	}
	else if (1 == esp_Get("LightSwitch", "true"))
	{
		alert_off();
	}
	return 1;
}



void esp_Get_Data(void)
{
	esp_Get_LED();
	
	
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 清空接收缓冲区
}

