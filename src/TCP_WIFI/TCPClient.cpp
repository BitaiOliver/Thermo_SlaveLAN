//#ifdef TCPCOMPILER

#include <TCPClient.h>
#include <Temperature.h>

#include "TCPClient_Priv.h"
#include "CompilerSwitch.h"

/* Global variables */
TcpClient_Data_Type TcpClient_Data;

uint8 TcpClient_OneTimeCounter = 1U;
String TcpClient_SerialPrint = "";


/********************************************************
* TCP Client Init function. 
********************************************************/
void TCPClient_Init()
{
#ifdef SERIAL_PRINT_TCP
    TcpClient_SerialPrint = "TcpClient: Init Start.";
    Serial.println(TcpClient_SerialPrint);
#endif

    TcpClient_Data.State = TCPCLIENT_STATE_INIT;
    TcpClient_Data.ServerCommand = 0U;
    TcpClient_Data.ConnectionTimer = 0U;
    TcpClient_Data.BadResponseCnt = 0U;

    //WiFi.begin(ssid, password);
#ifdef SERIAL_PRINT_TCP
    TcpClient_SerialPrint = "TcpClient: Init Stop.";
    Serial.println(TcpClient_SerialPrint);
#endif

}

/********************************************************
* TCP Client Main function. 
********************************************************/
void TCPClient_Main()
{
    if (0U < TcpClient_OneTimeCounter)
    {
        TcpClient_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_TCP
        TcpClient_SerialPrint = "TcpClient: Loop Start.";
        Serial.println(TcpClient_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    /* Check WiFi connection */
    tcpclient_CheckWiFiConnection();
    switch (TcpClient_Data.State)
    {
        case TCPCLIENT_STATE_INIT:
        {
            /* Do nothing */
            break;
        }
        case TCPCLIENT_STATE_CONNECTING:
        {
            /* Connect to the server */
            tcpclient_CheckServerConnection();
            break;
        }
        case TCPCLIENT_STATE_CONNECTED:
        {
            tcpclient_SendData();
            break;
        }
        case TCPCLIENT_STATE_WAIT_RESPONSE:
        {
            tcpclient_ReceiveResponse();
            break;
        }

        default:
            break;
    }
}

/********************************************************
* Function to handle CONNECTED state. 
* Receive response from the Server.
********************************************************/
void tcpclient_ReceiveResponse(void)
{
    uint16 tempval = 0;
    uint16 temperature = 0U;
    if (client.available())
    {
        temperature = (Rte_IRead_Temperature_ReadedTemperature.Data * TEMPERATURE_READ_TEMP_FACTOR);
        String line = client.readStringUntil('\n');
        Serial.println("Response: " + line);
        tempval = (uint16)line.toInt();
        if(temperature != (tempval - TCPCLIENT_POSITIVE_RESP_OFFSET))
        {
            /* Negative respose */
            if (TCPCLIENT_CONNECTION_TIMEOUT > TcpClient_Data.BadResponseCnt)
            {
                /* Try to send again data*/
                TcpClient_Data.BadResponseCnt++;
                Rte_IRead_TCPClient_ActionFlags |= (1 << TCPCLIENT_SEND_DATA_ID);
            }
            else
            {
                /* Threshold reached -> Reset counter to wait till the next cycle. */
                /* Reset bad response counter*/
                TcpClient_Data.BadResponseCnt = 0U;
            }
#ifdef SERIAL_PRINT_TCP
            TcpClient_SerialPrint = "TcpClient: Negative Response from Master: " + String(tempval);
            Serial.println(TcpClient_SerialPrint);
#endif
        }
        else
        {
            /* Positive Response */
            /* Reset bad response counter*/
            TcpClient_Data.BadResponseCnt = 0U;
#ifdef SERIAL_PRINT_TCP
            TcpClient_SerialPrint = "TcpClient: Positive Response from Master: " + String(tempval);
            Serial.println(TcpClient_SerialPrint);
#endif
        }      
    }
    else
    {
        /* No response from server. */
        if (TCPCLIENT_CONNECTION_TIMEOUT > TcpClient_Data.BadResponseCnt)
        {
            /* Try to send again data*/
            TcpClient_Data.BadResponseCnt++;
            Rte_IRead_TCPClient_ActionFlags |= (1 << TCPCLIENT_SEND_DATA_ID);
        }
        else
        {
            /* Threshold reached -> Reset counter to wait till the next cycle. */
            /* Reset bad response counter*/
            TcpClient_Data.BadResponseCnt = 0U;
        }
#ifdef SERIAL_PRINT_TCP
        TcpClient_SerialPrint = "TcpClient: No response from server.";
        Serial.println(TcpClient_SerialPrint);
#endif
    }

    /* Stop connection to the server */
    client.stop();
    /* go babk to connecting state*/
    TcpClient_Data.State = TCPCLIENT_STATE_CONNECTING;
    /* Reset server command */
    TcpClient_Data.ServerCommand = 0U;
}

/********************************************************
* Function to handle CONNECTED state. 
* Sends Data to the Server.
********************************************************/
void tcpclient_SendData(void)
{
    String temp = "";
    uint16 temperature = 0U;
    if (client.status() == ESTABLISHED)
    {
        /* Reset both action flags */
        Rte_IRead_TCPClient_ActionFlags &= ~(1 << TCPCLIENT_SEND_DATA_ID);
        temperature = (uint16)(Rte_IRead_Temperature_ReadedTemperature.Data * TEMPERATURE_READ_TEMP_FACTOR);
        if(100U > temperature)
        {
            /* add a "0" before temp value to keep the 3 digit size */
            temp = "S" + String(BLYNK_SLAVE_ID) + "T0" + String(temperature) + "Q" + String(Rte_IRead_Temperature_ReadedTemperature.Qf);
        }
        else
        {
            /* no formatting needed */
            temp = "S" + String(BLYNK_SLAVE_ID) + "T" + String(temperature) + "Q" + String(Rte_IRead_Temperature_ReadedTemperature.Qf);
        }
        /* Send data to the Server */
        client.println(temp);

        /* Stop connection to the server */
        //client.stop();

        /* Data Sent -> go to Wait_Response state */
        TcpClient_Data.State = TCPCLIENT_STATE_WAIT_RESPONSE;

#ifdef SERIAL_PRINT_TCP_DATA_TO_SERVER
        TcpClient_SerialPrint = "TcpClient: Send data " + temp;
        Serial.println(TcpClient_SerialPrint);
#endif
    }
    else
    {
        /* Connection lost -> go to CONNECTING state */
        TcpClient_Data.State = TCPCLIENT_STATE_CONNECTING;
#ifdef SERIAL_PRINT_TCP
        TcpClient_SerialPrint = "TcpClient: Connection lost.";
        Serial.println(TcpClient_SerialPrint);
#endif
    }
}

/********************************************************
* Function to handle CONNECTING state during Server connection 
* establishing or recconnecting to Server.
********************************************************/
void tcpclient_CheckServerConnection(void)
{
    if((FALSE != (Rte_IRead_TCPClient_ActionFlags &(1 << TCPCLIENT_SEND_DATA_ID))))
    {
        if (client.connect(host, TCPCLIENT_CONNECTION_PORT))
        {
            /* Client connected -> go to Conected State */
            TcpClient_Data.State = TCPCLIENT_STATE_CONNECTED;
            /* Reset connecting timer*/
            TcpClient_Data.ConnectionTimer = 0U;
#ifdef SERIAL_PRINT_TCP
            TcpClient_SerialPrint = "TcpClient: Connected to "+ String(host);
            Serial.println(TcpClient_SerialPrint);
#endif
        }
        else
        {
            /* Client not connected -> keep CONNECTING State */
            TcpClient_Data.State = TCPCLIENT_STATE_CONNECTING;

            /* Stop the client eaven if the connection failed */
            client.stop();

            if (TCPCLIENT_CONNECTION_TIMEOUT > TcpClient_Data.ConnectionTimer)
            {
                /* Up to 5s try to connect server.*/
                TcpClient_Data.ConnectionTimer++;
            }
            else
            {
                /* Threshold reached -> Reset counter and action flags to wait till the next cycle. */
                /* Reset  action flags */
                Rte_IRead_TCPClient_ActionFlags &= ~(1 << TCPCLIENT_SEND_DATA_ID);
                /* Reset connecting timer*/
                TcpClient_Data.ConnectionTimer = 0U;
            }

#ifdef SERIAL_PRINT_TCP
            TcpClient_SerialPrint = "TcpClient: Connection to the server Failed";
            Serial.println(TcpClient_SerialPrint);
#endif
        }
    }
    else
    {
#ifdef SERIAL_PRINT_TCP
        TcpClient_SerialPrint = "TcpClient: Wait to send data.";
        //Serial.println(TcpClient_SerialPrint);
#endif  
    }
}

/********************************************************
* Function to handle Init state during WiFi connection 
* establishing or recconnecting to WiFi.
********************************************************/
void tcpclient_CheckWiFiConnection(void)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        /* WiFi not connected -> keep INIT state */
        TcpClient_Data.State = TCPCLIENT_STATE_INIT;        

    }
    else
    {
        if((TCPCLIENT_STATE_CONNECTED != TcpClient_Data.State) && (TCPCLIENT_STATE_WAIT_RESPONSE != TcpClient_Data.State))
        {
            /* WiFi connected -> go to CONNECTING state */
            TcpClient_Data.State = TCPCLIENT_STATE_CONNECTING;
        }
        else
        {
            /* do nothing */
        }
    }
}
