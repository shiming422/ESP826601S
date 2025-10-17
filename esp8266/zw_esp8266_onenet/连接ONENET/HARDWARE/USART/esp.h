#ifndef __esp_H
#define __esp_H

int contains_substring(const u8 *main_string, const char *substring);
char esp_Init(void);
char Esp_PUB(void);
int esp_Get_LED(void);
void esp_Get_Data(void);

/**********************************************************************
 * 1、功能：组包函数
 * 2、参数：
 *      （1）Name：对应云平台物模型标识符
 *      （2）Data：需要发送的数据（unsigned char类型）
 *      （3） Cmd：参数输入：0或1（0：末尾一个数据；1：末尾前的数据）
 **********************************************************************/
void Get_Send_Pack(char *Name, unsigned char Data, unsigned char Cmd);
void Get_Send_float_Pack(char *Name, float Data, unsigned char Cmd);
/*发送数据函数*/
void SendData_Onenet(void);
extern unsigned char Send_AT_Data[400];
#endif
