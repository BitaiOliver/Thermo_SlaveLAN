
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>


/**************** Defines ************/


/**************** Varables ************/
/// OS ///
uint8 Rte_TaskMgr_Handler_Flags;
//------//

/// Temperature ///
Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
uint8 Rte_Temperature_ActionFlags;
//---------------//

/// Blynk ///
uint8 Rte_BlynkClient_ActionFlags;
uint8 Rte_BlynkClient_WiFiStatus;
uint8 Rte_BlynkClient_ServerConnStatus;
//---------//

/// TCP ///
uint8 Rte_TCPClient_ActionFlags;
//------//



/**************** Functions ************/
void Rte_Init(void)
{
    /// OS ///
    Rte_TaskMgr_Handler_Flags = 0U;
    //------//
    
    /// Temperature ///
    Rte_Temperature_ActionFlags = 0U;
    Rte_Temperature_ReadedTemp.Data = 0U;
    Rte_Temperature_ReadedTemp.Qf = 0U;
    //---------------//

    /// Blynk ///
    Rte_BlynkClient_ActionFlags = 0U;
    Rte_BlynkClient_WiFiStatus = 0U;
    Rte_BlynkClient_ServerConnStatus = 0U;
    //---------//

    /// TCP ///
    Rte_TCPClient_ActionFlags = 0U;
    //------//


}