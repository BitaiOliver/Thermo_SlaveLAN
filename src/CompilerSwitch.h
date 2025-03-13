#ifndef COMPILER_SWITCH_H
#define COMPILER_SWITCH_H 1
/* This header shall be included in everi software component.
    This header includes defines which are used as compiler switches. */

#define SERIAL_PRINT_TCP_DATA_TO_SERVER         1U /* If this define exist serial prints are working for TcpClient module */

#define SERIAL_PRINT_MAIN                       1U /* If this define exist serial prints are working for TcpClient module */
#define SERIAL_PRINT_TASKMGR                    1U /* If this define exist serial prints are working for TcpClient module */
#define SERIAL_PRINT_TCP                        1U /* If this define exist serial prints are working for TcpClient module */
#define SERIAL_PRINT_TEMP                       1U /* If this define exist serial prints are working for Temperature module */
#define SERIAL_PRINT_BLYNK                      1U /* If this define exist serial prints are working for BlynkClient module */


/* Some other defines used by all the modules */
#define TRUE                    1U
#define FALSE                   0U

#define TEMPERATURE_QF_INIT             0U
#define TEMPERATURE_QF_GOOD             1U
#define TEMPERATURE_QF_FAILING          2U
#define TEMPERATURE_QF_BAD              3U


#define WIFI_STATUS_INIT                0U
#define WIFI_STATUS_CONNECTED           1U
#define WIFI_STATUS_NOT_CONNECTED       2U

#define BLYNK_SERVER_STATUS_INIT                0U
#define BLYNK_SERVER_STATUS_CONNECTED           1U
#define BLYNK_SERVER_STATUS_NOT_CONNECTED       2U

/***************** Configuration *****************/
const char Blynk_WiFi_Name[] = "N***i";          /* add your WiFi name (SSID) */   
const char Blynk_WiFi_Password[] = "t***3";      /* add your WiFi password */

/*
#define BLYNK_SLAVE_ID                  1U      // Slave IDs start from 1 
*/

#define BLYNK_SLAVE_ID                  2U      // Slave IDs start from 1 

/*
#define BLYNK_SLAVE_ID                  3U      // Slave IDs start from 1 
*/

/*************************************************/
#endif