#include "EC20.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
char *strx,*extstrx,*Readystrx;
/********************数据定义*************/
int car_number = 99;
int people_animals_number = 10;
int break_number = 9;
int sundries_number = 0;
int device_id = 0;
int state = 1;
int error = 0;

char carrier[] = "car";
char plate[] = "苏A·00001";
char Violation_category[] = "highspeed";

double speed = 100.123;
double weight = 100.456;
double temperature = 33.123;
double humidity = 55.334;
double air = 40.555;
double co2 = 500.5;
double co = 2.3;
double pm = 35.6;
double longitude = 125.555;
double latitude = 37.555;

/****************************************/

void EC20_send_str(char*str)
{
	HAL_UART_Transmit(&huart6,(uint8_t*)str,strlen(str),100);
	///printf("%s",EC20_RxBuffer2);
}
void  EC20_Init(void)
{
	  EC20_send_str("AT\r\n");
	  HAL_Delay(500);
	
	  strx=strstr((const char*)EC20_RxBuffer2,(const char*)"OK");//返回OK
	
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  while(strx==NULL)
	  {
	    EC20_send_str("AT\r\n");
	    HAL_Delay(500);
	    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"OK");//返回OK
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  }
	  EC20_send_str("AT+QGPS=1\r\n"); //对GNSS开机
	  HAL_Delay(500);
	  EC20_send_str("AT+QMTDISC=0\r\n");
	  HAL_Delay(500);
	  EC20_send_str("ATE0\r\n"); //关闭回显
	  HAL_Delay(500);
	  EC20_send_str("AT+CSQ\r\n"); //检查CSQ
	  HAL_Delay(500);
	  EC20_send_str("ATI\r\n"); //检查模块的版本号
	  HAL_Delay(500);
	  EC20_send_str("AT+CPIN?\r\n");//检查SIM卡是否在位
	  HAL_Delay(500);
	  strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CPIN: READY");		//查看是否返回ready
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  while(strx==NULL)
	  {
	   
	    EC20_send_str("AT+CPIN?\r\n");
	    HAL_Delay(500);
	    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CPIN: READY");
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  //检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
	  }
	  
	  EC20_send_str("AT+CREG?\r\n");//查看是否注册GSM网络
	  HAL_Delay(500);
	  strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CREG: 0,1");			//返回正常
	  extstrx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CREG: 0,5");		//返回正常，漫游
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  while(strx==NULL&&extstrx==NULL)
	  {
	    
	    EC20_send_str("AT+CREG?\r\n");//查看是否注册GSM网络
	    HAL_Delay(500);
	    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CREG: 0,1");		//返回正常
	    extstrx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CREG: 0,5");		//返回正常，漫游
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  }
	  
	  /////////////////////////////////////
	  EC20_send_str("AT+CGREG?\r\n");//查看是否注册GPRS网络
	  HAL_Delay(500);
	  strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CGREG: 0,1");			//这里重要，只有注册成功，才可以进行GPRS数据传输。
	  extstrx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CGREG: 0,5");		//返回正常，漫游
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  while(strx==NULL&&extstrx==NULL)
	  {
	    
	    EC20_send_str("AT+CGREG?\r\n");//查看是否注册GPRS网络
	    HAL_Delay(500);
	    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CGREG: 0,1");		//这里重要，只有注册成功，才可以进行GPRS数据传输。
	    extstrx=strstr((const char*)EC20_RxBuffer2,(const char*)"+CGREG: 0,5");	//返回正常，漫游
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	  }
	  
	  EC20_send_str("AT+COPS?\r\n");//查看注册到哪个运营商，支持移动 联通 电信
	  HAL_Delay(500);
	  
}


void  MQTT_Init(void)
{
    EC20_send_str("AT+QMTOPEN=0,\"122.51.67.189\",1883\r\n");

    HAL_Delay(500);
    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTOPEN: 0,0");		//看下返回状态
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
    while(strx==NULL)
    {
      strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTOPEN: 0,0");		//确认返回值正确
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
    }
    
    EC20_send_str("AT+QMTCONN=0,\"road\",\"road\",\"Jit666666!\"\r\n");			//去登录MQTT服务器，设备ID，账号和密码.用户根据实际需要进行更改
    HAL_Delay(500);
    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTCONN: 0,0,0");	//看下返回状态
		memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
		while(strx==NULL)
    {
      strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
    }
    
}
void a()/*
		uint8_t *t_payload,
    int car_number,
    char carrier[],
    char plate[])/*,
    char Violation_category[],
    double speed,
    double weight,
    int people_animals_number,
    int break_number,
    int sundries_number,
    int device_id,
    int state,
    int error,
    double temperature,
    double humidity,
    double air,
    double co2,
    double co,
    double pm,
    double longitude,
    double latitude
		)*/
{
	printf("start2\r\n");
	char json[]="{\"car_number\":%d,\"carrier\":\"%s\",\"plate\":\"%s\"}";//,\"Violation_category\":\"%s\",\"speed\":%f,\"weight\":%f,\"people_animals_number\":%d,\"break_number\":%d,\"sundries_number\":%d,\"device_id\":%d,\"state\":%d,\"error\":%d,\"temperature\":%f,\"humidity\":%f,\"air\":%f,\"co2\":%f,\"co\":%f,\"pm\":%f,\"longitude\":%f,\"latitude\":%f}";
	printf("1");
	char t_json[1024];
	unsigned short json_len;
	/*格式化字符串*/
	sprintf(t_json, json,
    car_number,
    carrier,
    plate);printf("2");/*,
    Violation_category,
    speed,
    weight,
    people_animals_number,
    break_number,
    sundries_number,
    device_id,
    state,
    error,
    temperature,
    humidity,
    air,
    co2,
    co,
    pm,
    longitude,
    latitude
);*/

	json_len = strlen(t_json)/sizeof(char);
	//memcpy(t_payload, t_json, json_len);
	printf("3");
	//return json_len;
}
void Pub_Msgdata(void)
{
	printf("start1\r\n");
	char str_datastr[1024];
	uint8_t data_len,datastr[1024],send_jason[1024];
	printf("start1.5\r\n");
	a();
	//Mqtt_Savedata();
	/*
		send_jason,
    car_number,
    carrier,
    plate);/*,
    Violation_category,
    speed,
    weight,
    people_animals_number,
    break_number,
    sundries_number,
    device_id,
    state,
    error,
    temperature,
    humidity,
    air,
    co2,
    co,
    pm,
    longitude,
    latitude
		);*/
		printf("4");
	sprintf(datastr,"%d",data_len); 
	sprintf(str_datastr,"AT+QMTPUBEX=0,0,0,0,\"road\",%s\r\n",datastr);
	EC20_send_str(str_datastr);//发布消息
		printf(str_datastr);
		printf(EC20_RxBuffer2);
	HAL_Delay(100);
	EC20_send_str(send_jason);//数据发送
	strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTPUBEX: 0,0,0");//发布成功   +QMTPUBEX: 0,0,0
	memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	while(strx==NULL)
	{
	    strx=strstr((const char*)EC20_RxBuffer2,(const char*)"+QMTPUBEX: 0,0,0");//发布成功   +QMTPUBEX: 0,0,0
			memset(EC20_RxBuffer2, 0, sizeof(EC20_RxBuffer2));  // 清空整个数组
	}
	HAL_Delay(1000);
}
