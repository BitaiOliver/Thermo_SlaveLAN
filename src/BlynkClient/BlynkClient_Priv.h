#ifndef BLYNKCLIENT_PRIV_H
#define BLYNKCLIENT_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Rte_Types.h>



/**************** Defines ************/
#define BLYNKCLIENT_PORT    8080U

#define IP_1st              192U
#define IP_2nd              168U
#define IP_3rd              0U
#define IP_4th              123U
//#define IP_4th              131U



#define BLYNKCLIENT_STATE_INIT              1U
#define BLYNKCLIENT_STATE_CONNECTED         2U
#define BLYNKCLIENT_STATE_NOT_CONNECTED     3U


#define BLYNKCLIENT_RECONNECT_WAIT_TIME     30U

#define BLYNKCLIENT_NO_NOTIFICATION         0U

#define BLYNKCLIENT_TEMP_SCALE              10U

/**************** Varables ************/

typedef struct BlynkClient_Data_Type_Tag
{
    uint8       State; 
    uint8       Notification;
    uint8       ReconnectCounter;
}BlynkClient_Data_Type;

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IRead_BlynkClient_ReadedTemperature    Rte_Temperature_ReadedTemp

extern uint8 Rte_BlynkClient_ActionFlags;
#define Rte_IRead_BlynkClient_ActionFlags Rte_BlynkClient_ActionFlags

extern uint8 Rte_BlynkClient_WiFiStatus;
#define Rte_IWrite_BlynkClient_WifiStatus Rte_BlynkClient_WiFiStatus

extern uint8 Rte_BlynkClient_ServerConnStatus;
#define Rte_IWrite_BlynkClient_ServerConnStatus Rte_BlynkClient_ServerConnStatus


/**************** Functions ************/
void blynkclient_CheckWiFiState(void);

#endif