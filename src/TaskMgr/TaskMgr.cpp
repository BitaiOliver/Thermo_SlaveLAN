#include <TaskMgr.h>
#include <Temperature.h>
#include <BlynkClient.h>
#include <TCPClient.h>

#include "TaskMgr_Priv.h"
#include "CompilerSwitch.h"

/* Global shared variables */
uint8 TaskMgr_InRunPrintCounter = 0U;
uint8 TaskMgr_OneTimeCounter = 1U;
String TaskMgr_SerialPrint = "";


void TaskMgr_Init(void)
{
#ifdef SERIAL_PRINT_TASKMGR
  TaskMgr_SerialPrint = "TaskMgr: Init Start.";
  Serial.println(TaskMgr_SerialPrint);
#endif

    /// OS ///
    /* Initialize timer handlers flag */
    Rte_IRead_TaskMgr_Handler_Flags = 0U;
    /* Call Rte_Init() */
    Rte_Init();
    //------//

    /// Temperature ///
    Temperature_Init();
    //---------------//

    /// TCP ///
    TCPClient_Init();
    //-------//

    /* Blynk init lasts much time so keep it in the last position. */
    /// Blynk ///
    BlynkClient_Init();
    //---------//

#ifdef SERIAL_PRINT_TASKMGR
  TaskMgr_SerialPrint = "TaskMgr: Init Stop.";
  Serial.println(TaskMgr_SerialPrint);
#endif

}

void TaskMgr_Main(void)
{
    if (0U < TaskMgr_OneTimeCounter)
    {
        TaskMgr_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_TASKMGR
        TaskMgr_SerialPrint = "TaskMgr: Loop Start.";
        Serial.println(TaskMgr_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    /* The order of the tasks are decreasing because rearly executed tasks set flags 
        for often executed tasks */
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_1M_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_1M_ID);
        /* Call handler */
        Handle_1m_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_30S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_30S_ID);
        /* Call handler */
        Handle_30s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_10S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_10S_ID);
        /* Call handler */
        Handle_10s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_5S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_5S_ID);
        /* Call handler */
        Handle_5s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_1S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_1S_ID);
        /* Call handler */
        Handle_1s_Task();
    }
    else
    {
        /* code */
    }
}

void Handle_1s_Task(void)
{
    Temperature_Main();
    BlynkClient_Main();
    TCPClient_Main();

    switch (TaskMgr_InRunPrintCounter)
    {
    case 0:
    {
        TaskMgr_InRunPrintCounter = 1U;
#ifdef SERIAL_PRINT_TASKMGR
        TaskMgr_SerialPrint = "TaskMgr: Running 1s.";
        Serial.println(TaskMgr_SerialPrint);
#endif
        break;
    }
    case 1:
    {
        TaskMgr_InRunPrintCounter = 2U;
#ifdef SERIAL_PRINT_TASKMGR
        TaskMgr_SerialPrint = "TaskMgr: Run: ";
        Serial.print(TaskMgr_SerialPrint);
#endif
        break;
    }
    case 2:
    {
        TaskMgr_InRunPrintCounter = 3U;
#ifdef SERIAL_PRINT_TASKMGR
        TaskMgr_SerialPrint = " W: " + String(Rte_IRead_TaskMgr_WifiStatus) + " B: " + String(Rte_IRead_TaskMgr_ServerConnStatus);
        Serial.print(TaskMgr_SerialPrint);
#endif
        break;
    }
    case 3:
    {
        TaskMgr_InRunPrintCounter = 4U;
#ifdef SERIAL_PRINT_TASKMGR
        //TaskMgr_SerialPrint = " .";
        TaskMgr_SerialPrint = " W: " + String(Rte_IRead_TaskMgr_WifiStatus) + " B: " + String(Rte_IRead_TaskMgr_ServerConnStatus);
        Serial.print(TaskMgr_SerialPrint);
#endif
        break;
    }
    case 4:
    {
        TaskMgr_InRunPrintCounter = 5U;
#ifdef SERIAL_PRINT_TASKMGR
        //TaskMgr_SerialPrint = " .";
        TaskMgr_SerialPrint = " W: " + String(Rte_IRead_TaskMgr_WifiStatus) + " B: " + String(Rte_IRead_TaskMgr_ServerConnStatus);
        Serial.print(TaskMgr_SerialPrint);
#endif
        break;
    }
    case 5:
    {
        TaskMgr_InRunPrintCounter = 1U;
#ifdef SERIAL_PRINT_TASKMGR
        //TaskMgr_SerialPrint = " .";
        TaskMgr_SerialPrint = " W: " + String(Rte_IRead_TaskMgr_WifiStatus) + " B: " + String(Rte_IRead_TaskMgr_ServerConnStatus);
        Serial.println(TaskMgr_SerialPrint);
#endif
        break;
    }

    
    default:
        break;
    }

}
void Handle_5s_Task(void)
{
}
void Handle_10s_Task(void)
{
    /* Set flag for Temperature to handle reading */
    Rte_IWrite_TaskMgr_Temperature_ActionFlags |= (1 << TEMPERATURE_HANDLE_READING_ID);
}
void Handle_30s_Task(void)
{  
}
void Handle_1m_Task(void)
{
    /* Set flag for Blynk to send data to server */
    Rte_IWrite_TaskMgr_BlynkClient_ActionFlags |= (1 << BLYNKCLIENT_SEND_DATA_ID);
    /* Set flag for TCP to send data to server */
    Rte_IRead_TcpClient_Handler_Flags |= (1 << TCPCLIENT_SEND_DATA_ID);
}