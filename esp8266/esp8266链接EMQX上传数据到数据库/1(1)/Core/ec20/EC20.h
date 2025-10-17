#ifndef __EC20_H__
#define __EC20_H__

#include "main.h"
void 	EC20_send_str(char*str);
void  EC20_Init(void);
void  MQTT_Init(void);
void Mqtt_Savedata();
void a();/*
		uint8_t *t_payload,
    int car_number,
    char carrier[],
    char plate[]);/*,
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
		);*/
void Pub_Msgdata(void);

#endif /*__ EC20_H__ */
