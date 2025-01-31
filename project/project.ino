#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include <EDB.h>
#include <EEPROM.h>
#include <stdlib.h>
#include <semphr.h>
#include <queue.h>

#define TABLE_SIZE 512          // Up to 1024

#define portUSE_WDTO WDTO_30MS

const int safeWakeTime = 300;

//LoR32u4II 868MHz or 915MHz (black board)
  #define SCK     15
  #define MISO    14
  #define MOSI    16
  #define SS      8
  #define RST     4
  #define DI0     7
  #define BAND    869100000
  #define PABOOST true 

#define RESTART_GW 0

const int teamNum = 06;

const int counterLimit = 20;

int GWcounter = 0;

bool idleFlag = false;
bool powerDownFlag = false;

int sleepDuration = 0;

TaskHandle_t gatewayHandle, databaseHandle, sleepHandle, serialHandle;

SemaphoreHandle_t SemaphoreHndl;

EDB db(&writer, &reader);
EDB_Status dbst = EDB_OK;

struct Log {
  // Struct for storing data inside database. System is prepared to store more parameters.
  int msg;
  float data;
}logType;

QueueHandle_t logQueue;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();

  setup_sleep();
  
  db.create(0, TABLE_SIZE, (unsigned int)sizeof(logType));
  //db.open(0);       // Uncomment for using an already stored EEPROM database, comment out db.create() in this case!

  SemaphoreHndl = xSemaphoreCreateBinary();
  if( (SemaphoreHndl) != NULL)
    xSemaphoreGive((SemaphoreHndl));
  
  logQueue = xQueueCreate(10, sizeof(Log));

  // Setting up tasks
  if(logQueue != NULL){
    
    xTaskCreate(
      GatewayComm
      ,  (const portCHAR *)"GatewayComm"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  &gatewayHandle );
      
    xTaskCreate(
      DatabaseHandler
      ,  (const portCHAR *)"DatabaseHandler"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  &databaseHandle );
  }
  xTaskCreate(
    SerialCommPC
    ,  (const portCHAR *)"SerialCommPC"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &serialHandle ); 
}

void loop() {
  // Empty. See tasks.ino for running functions  
}
