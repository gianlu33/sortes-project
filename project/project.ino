#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include <EDB.h>

//LoR32u4II 868MHz or 915MHz (black board)
  #define SCK     15
  #define MISO    14
  #define MOSI    16
  #define SS      8
  #define RST     4
  #define DI0     7
  #define BAND    869100000  // 915E6
  #define PABOOST true 

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();

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
