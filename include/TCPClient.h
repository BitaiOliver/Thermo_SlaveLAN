#ifndef TCPCLIENT_H
#define TCPCLIENT_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Defines ************/
#define TCPCLIENT_SEND_DATA_ID          0U
#define TCPCLIENT_SERVER_TEST_ID        1U


/**************** Varables ************/



/**************** Functions ************/
void TCPClient_Init();
void TCPClient_Main();



#endif