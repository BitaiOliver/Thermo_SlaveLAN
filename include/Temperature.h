#ifndef TEMPERATURE_H
#define TEMPERATURE_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Defines ************/

#define TEMPERATURE_HANDLE_READING_ID       0U

#define TEMPERATURE_READ_TEMP_FACTOR   10U /* Multiplication factor of the sensor data */


/**************** Varables ************/


/**************** Functions ************/
void Temperature_Init(void);
void Temperature_Main(void);



#endif