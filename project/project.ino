#include <Arduino_FreeRTOS.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  //TODO set parameters...

  // Now set up two tasks to run independently.
  xTaskCreate(
    GatewayComm
    ,  (const portCHAR *)"GatewayComm"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    // Now set up two tasks to run independently.
  xTaskCreate(
    SerialCommPC
    ,  (const portCHAR *)"SerialCommPC"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    // Now set up two tasks to run independently.
  xTaskCreate(
    DatabaseHandler
    ,  (const portCHAR *)"DatabaseHandler"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}

void loop() {}
