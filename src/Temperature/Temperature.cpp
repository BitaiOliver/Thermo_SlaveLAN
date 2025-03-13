#include <OneWire.h>
#include <DallasTemperature.h>
#include <Temperature.h>

#include "Temperature_Priv.h"
#include "CompilerSwitch.h"

/* Global Variables */

Temperature_Data_Type Temperature_Data;


OneWire oneWire(TEMPERATURE_SENSOR_BUS_PIN);
DallasTemperature sensors(&oneWire);


String Temperature_PrintString;

uint8 Temperature_OneTimeCounter = 1U;
String Temperature_SerialPrint = "";


/* Functions */
/********************************************************
* Temperature Init function. 
********************************************************/
void Temperature_Init(void)
{
#ifdef SERIAL_PRINT_TEMP
  Temperature_SerialPrint = "Temperature: Init Start.";
  Serial.println(Temperature_SerialPrint);
#endif

    Temperature_PrintString = "";

    Rte_IRead_Temperature_ActionFlags = 0U;
    Rte_IWrite_Temperature_ReadedTemperature.Data = 0U;
    Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_INIT;

    Temperature_Data.ReadingTimer = 1U; /* initialize with 1 to manage start offset */
    Temperature_Data.State = TEMPERATURE_STATE_WAIT;
    Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_1;
    Temperature_Data.NbOfReadingRetry = 0U;

    pinMode(TEMPERATURE_SENSOR_VCC_PIN, OUTPUT);
    digitalWrite(TEMPERATURE_SENSOR_VCC_PIN, LOW);

    /* start one wire */
    sensors.begin();
#ifdef SERIAL_PRINT_TEMP
  Temperature_SerialPrint = "Temperature: Init Stop.";
  Serial.println(Temperature_SerialPrint);
#endif
}

/********************************************************
* Temperature Main function. 
********************************************************/
void Temperature_Main(void)
{
    if (0U < Temperature_OneTimeCounter)
    {
        Temperature_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_TEMP
        Temperature_SerialPrint = "Temperature: Loop Start.";
        Serial.println(Temperature_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    if(FALSE != (Rte_IRead_Temperature_ActionFlags &(1 << TEMPERATURE_HANDLE_READING_ID)))
    {
        /* Reset handle reading flag */
        Rte_IRead_Temperature_ActionFlags &= ~(1 << TEMPERATURE_HANDLE_READING_ID);
        /* Increment reading timer */
        Temperature_Data.ReadingTimer++;
    }
    else
    {
        /* code */
    }

    switch (Temperature_Data.State)
    {
    case TEMPERATURE_STATE_WAIT:
    {
        if (TEMPERATURE_VCC_TEMP_THRSH <= Temperature_Data.ReadingTimer)
        {
            /* Turn on Temp Sensor.*/
            digitalWrite(TEMPERATURE_SENSOR_VCC_PIN, HIGH);

        }
        else
        {
            /* code */
        }

        if (TEMPERATURE_READ_TEMP_THRSH <= Temperature_Data.ReadingTimer)
        {
            /* Reset reading timer */
            Temperature_Data.ReadingTimer = 0U;
            Temperature_Data.State = TEMPERATURE_STATE_READ;
        }
        else
        {
            /* code */
        }
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Wait to flag ";
            //Serial.println(Temperature_PrintString);
#endif

        break;
    }
    case TEMPERATURE_STATE_READ:
    {
        temperature_ReadTemperature();
        break;
    }
    
    default:
        break;
    }

}

/********************************************************
* Read temperature handler function. 
********************************************************/
void temperature_ReadTemperature()
{
    switch (Temperature_Data.ReadingState)
    {
    case TEMPERATURE_READING_STEP_1:
    {
        /* Read temperature */
        temperature_ReadSensor(TEMPERATURE_READING_STEP_1);
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Read temp step 1 ";
            //Serial.println(Temperature_PrintString);
#endif

        break;
    }
    case TEMPERATURE_READING_STEP_2:
    {
        /* Read temperature */
        temperature_ReadSensor(TEMPERATURE_READING_STEP_2);
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Read temp step 2 ";
            //Serial.println(Temperature_PrintString);
#endif
        break;
    }
    case TEMPERATURE_READING_STEP_3:
    {
        temperature_ComputeTemperature();
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Read temp step 3 ";
            //.println(Temperature_PrintString);
#endif
        break;
    }
    
    default:
        break;
    }
}

/********************************************************
* Read sensor error free handler function. 
********************************************************/
void temperature_ReadSensor(uint8 step)
{
    float temp = 0;
    temp = temperature_GetSensorData();
    if ((TEMPERATURE_MIN_TEMP <= temp) && (TEMPERATURE_MAX_TEMP >= temp))
    {
        /* Temperature valid -> save reading */
        Temperature_Data.CurrentReading[step].Data = temp; 
        Temperature_Data.CurrentReading[step].Qf = TEMPERATURE_QF_GOOD;

        /* Reset retry counter */
        Temperature_Data.NbOfReadingRetry = 0U;

        /* Jump to the next state */
        Temperature_Data.ReadingState = step +1; 
    }
    else
    {
        /* Failed to read sensor or negative value */
        Temperature_Data.CurrentReading[step].Qf = TEMPERATURE_QF_BAD; 
        /* Increment nuber of retry counter */
        Temperature_Data.NbOfReadingRetry++;

        if (TEMPERATURE_NB_OF_RETRY <= Temperature_Data.NbOfReadingRetry)
        {
            /* Reset retry counter */
            Temperature_Data.NbOfReadingRetry = 0U;
            /* Max retry reached -> Jump to the next state */
            Temperature_Data.ReadingState = step +1; 
        }
        else
        {
            /* code */
        }   
    }
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Temp " + String(step);
            Temperature_PrintString += ": " + String(Temperature_Data.CurrentReading[step].Data);
            Temperature_PrintString += " Qf: " + String(Temperature_Data.CurrentReading[step].Qf);
            Serial.println(Temperature_PrintString);
#endif
}

/********************************************************
* Read sensor row data. 
********************************************************/
float temperature_GetSensorData()
{
    //uint16 Temperature = 0;
    float Temperature = 0;
    sensors.requestTemperatures();  
    //Temperature = (uint16)(sensors.getTempCByIndex(0) * TEMPERATURE_READ_TEMP_FACTOR);
    Temperature = sensors.getTempCByIndex(0);
#ifdef SERIAL_PRINT_TEMP
    Temperature_PrintString = "Temperature: Sensor Data: " + String(Temperature);
    Serial.println(Temperature_PrintString);
#endif
    return Temperature;
}

/********************************************************
* Compute final temperature value. 
********************************************************/
void temperature_ComputeTemperature()
{
    float temp = 0U;

    /* Set reading status back to step 1 and Main status back to wait */
    Temperature_Data.State = TEMPERATURE_STATE_WAIT;
    Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_1;
    /* Turn on Temp Sensor.*/
    digitalWrite(TEMPERATURE_SENSOR_VCC_PIN, LOW);

    /* Compute the temperature from the 2 readings */
    if (TEMPERATURE_QF_GOOD == Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Qf)
    {
        if (TEMPERATURE_QF_GOOD == Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_2].Qf)
        {
            /* Both reading are good */
            Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_GOOD;
            temp = (Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Data + Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_2].Data);
            Rte_IWrite_Temperature_ReadedTemperature.Data = temp / TEMPERATURE_NUMBER_OF_READINGS;
        }
        else
        {
            /* First reading is good */
            Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_FAILING;
            temp = (Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Data + Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_2].Data);
            Rte_IWrite_Temperature_ReadedTemperature.Data = temp / TEMPERATURE_NUMBER_OF_READINGS;
        }
    }
    else
    {
        if (TEMPERATURE_QF_GOOD == Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_2].Qf)
        {
            /* Second reading is good */
            Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_FAILING;
            temp = (Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Data + Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_2].Data);
            Rte_IWrite_Temperature_ReadedTemperature.Data = temp / TEMPERATURE_NUMBER_OF_READINGS;
        }
        else
        {
            /* No reading reading is good, no need to modify the actual value. */
            Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_BAD;
        }
    }
}