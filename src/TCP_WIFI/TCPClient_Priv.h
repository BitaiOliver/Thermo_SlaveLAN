#ifndef TCPCLIENT_PRIV_H
#define TCPCLIENT_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Rte_Types.h>


/**************** Defines ************/
#define TCPCLIENT_STATE_INIT            1U
#define TCPCLIENT_STATE_CONNECTING      2U
#define TCPCLIENT_STATE_CONNECTED       3U
#define TCPCLIENT_STATE_WAIT_RESPONSE   4U

#define TCPCLIENT_CONNECTION_PORT       1995U

#define TCPCLIENT_DUMMY_COMMAND         9U


#define TCPCLIENT_POSITIVE_RESP_OFFSET  10U

#define TCPCLIENT_CONNECTION_TIMEOUT    3U


/**************** Varables ************/

const char*    host = "192.168.1.185"; //Master's IP/ server IP



WiFiClient client;

typedef struct TcpClient_Data_Type_Tag
{
    uint8       State;
    uint8       ServerCommand;
    uint8       ConnectionTimer;
    uint8       BadResponseCnt;    
} TcpClient_Data_Type;

extern uint8 Rte_TCPClient_ActionFlags;
#define Rte_IRead_TCPClient_ActionFlags    Rte_TCPClient_ActionFlags

extern uint8 Rte_BlynkClient_ServerConnStatus;
#define Rte_IRead_BlynkClient_ServerConnStatus Rte_BlynkClient_ServerConnStatus

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IRead_Temperature_ReadedTemperature    Rte_Temperature_ReadedTemp



/**************** Functions ************/
void tcpclient_CheckWiFiConnection(void);
void tcpclient_CheckServerConnection(void);
void tcpclient_SendData(void);
void tcpclient_ReceiveResponse(void);


#endif