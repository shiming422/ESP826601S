#ifndef __esp_H__
#define __esp_H__

#include "main.h"
int contains_substring(const char *main_string, const char *substring);
char esp_Init(void);
uint8_t wait_for_prompt(void);
/*发送数据函数*/
void Str_Pub_Msgdata(char *message_name,char *message_data);
void Num_Pub_Msgdata(char *message_name,char *message_data);
int calc_escaped_length(const char *json);
void Save_data();//char *message_name,char *message_data
#endif
