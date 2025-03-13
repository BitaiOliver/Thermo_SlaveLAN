#ifndef TEMPERATURE_PRIV_H
#define TEMPERATURE_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>


/**************** Defines ************/
#define TEMPERATURE_READ_TEMP_THRSH    6U /* 50s read. +1 is needed to start offset */
#define TEMPERATURE_VCC_TEMP_THRSH     5U /* 50s read. +1 is needed to start offset */


#define TEMPERATURE_SENSOR_BUS_PIN      4U  /*D02*/
#define TEMPERATURE_SENSOR_VCC_PIN      14U /*D05*/


/* Main function states */
#define TEMPERATURE_STATE_WAIT         1U 
#define TEMPERATURE_STATE_READ         2U 


/* Reading states */
#define TEMPERATURE_READING_STEP_1     0U 
#define TEMPERATURE_READING_STEP_2     1U 
#define TEMPERATURE_READING_STEP_3     2U 

#define TEMPERATURE_NUMBER_OF_READINGS 2U 

#define TEMPERATURE_MIN_TEMP            0U 
#define TEMPERATURE_MAX_TEMP            80U 


#define TEMPERATURE_NB_OF_RETRY         3U

/**************** Varables ************/

typedef struct Temperature_Data_Type_Tag
{
    uint8 ReadingTimer;    
    uint8 State;
    uint8 ReadingState;
    uint8 NbOfReadingRetry;
    Rte_Temperature_Reading_Type CurrentReading[TEMPERATURE_NUMBER_OF_READINGS];
}Temperature_Data_Type;

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IWrite_Temperature_ReadedTemperature    Rte_Temperature_ReadedTemp

extern uint8 Rte_Temperature_ActionFlags;
#define Rte_IRead_Temperature_ActionFlags    Rte_Temperature_ActionFlags
/**************** Functions ************/
void temperature_ReadTemperature();
void temperature_ReadSensor(uint8 step);
void temperature_ComputeTemperature();
float temperature_GetSensorData();



#endif