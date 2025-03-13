#include <BlynkClient.h>


#include "BlynkClient_Priv.h"
#include "CompilerSwitch.h"

//#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

/* Global Variables */

BlynkClient_Data_Type BlynkClient_Data;

String BlynkClient_PrintString;

uint8 BlynkClient_OneTimeCounter = 1U;
String BlynkClient_SerialPrint = "";


/* Functions */
/********************************************************
* BlynkClient Init function. 
********************************************************/
void BlynkClient_Init(void)
{
#ifdef SERIAL_PRINT_BLYNK
  BlynkClient_SerialPrint = "BlynkClient: Init Start.";
  Serial.println(BlynkClient_SerialPrint);
#endif

    BlynkClient_Data.State = BLYNKCLIENT_STATE_INIT;
    BlynkClient_Data.Notification = BLYNKCLIENT_NO_NOTIFICATION;
    /* Init with max value to start connect at startup.*/
    BlynkClient_Data.ReconnectCounter = BLYNKCLIENT_RECONNECT_WAIT_TIME;

    Rte_IRead_BlynkClient_ActionFlags = 0U;

    BlynkClient_PrintString = "";

    /* WiFi Configuration */
    WiFi.mode(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED) 
    {
#ifdef SERIAL_PRINT_BLYNK
        BlynkClient_SerialPrint = "BlynkClient: Wifi Start.";
        Serial.println(BlynkClient_SerialPrint);
#endif
        WiFi.begin(Blynk_WiFi_Name, Blynk_WiFi_Password);
#ifdef SERIAL_PRINT_BLYNK
        BlynkClient_SerialPrint = "BlynkClient: Wifi Stop.";
        Serial.println(BlynkClient_SerialPrint);
#endif
    }

#ifdef SERIAL_PRINT_BLYNK
  BlynkClient_SerialPrint = "BlynkClient: Init Stop.";
  Serial.println(BlynkClient_SerialPrint);
#endif

}
/********************************************************
* BlynkClient Main function. 
********************************************************/
void BlynkClient_Main(void)
{
    if (0U < BlynkClient_OneTimeCounter)
    {
        BlynkClient_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_BLYNK
        BlynkClient_SerialPrint = "BlynkClient: Loop Start.";
        Serial.println(BlynkClient_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

     blynkclient_CheckWiFiState();
}

/********************************************************
* BlynkClient chech connection status to the WiFI. 
********************************************************/
void blynkclient_CheckWiFiState(void)
{
   if(WiFi.status() != WL_CONNECTED)
   {
       Rte_IWrite_BlynkClient_WifiStatus = WIFI_STATUS_NOT_CONNECTED;
#ifdef SERIAL_PRINT_BLYNK
            BlynkClient_PrintString = "Blynk: WiFI Not connected ";
            //Serial.println(BlynkClient_PrintString);
#endif
   }
   else
   {
       Rte_IWrite_BlynkClient_WifiStatus = WIFI_STATUS_CONNECTED;
#ifdef SERIAL_PRINT_BLYNK
            BlynkClient_PrintString = "Blynk: WiFi Connected ";
            //Serial.println(BlynkClient_PrintString);
#endif

   }

}
