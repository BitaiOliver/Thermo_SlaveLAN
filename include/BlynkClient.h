#ifndef BLYNKCLIENT_H
#define BLYNKCLIENT_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Defines ************/
#define BLYNKCLIENT_SEND_DATA_ID          0U
#define BLYNKCLIENT_SEND_NOTIF_ID         1U

/**************** Varables ************/



/**************** Functions ************/
void BlynkClient_Init(void);
void BlynkClient_Main(void);

#endif