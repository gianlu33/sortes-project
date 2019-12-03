#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include <EDB.h>
#include <EEPROM.h>
#include <stdlib.h>
#include <semphr.h>
#include <queue.h>
#include <SleepMode.h>

#define TABLE_SIZE 512          // Up to 1024, corresponds to EEPROM capacity

//#define portUSE_WDTO WDTO_30MS    // Watchdog timer tick time can be increased to reduce power consumption even more (slightly), at the cost of real-time responsiveness. Default: 15MS

//LoR32u4II 868MHz or 915MHz (black board)
  #define SCK     15
  #define MISO    14
  #define MOSI    16
  #define SS      8
  #define RST     4
  #define DI0     7
  #define BAND    869100000     // Bandwidth specified for Team 06
  #define PABOOST true 

#define WAKE_PIN 3 // select between 0, 1, 2, 3, 7, to be used by the interrupt

// Define clock parameters for managing idle mode
#define SYS_CLK 8e6
#define CNT_CLK SYS_CLK / 1024

#define RESTART_GW 0    // 0 if we only want to start serial communication after waking up from power down mode, 1 if we want to restart GW comm and DB handling too

unsigned long cnt_left = 0;   // Counter for the idle mode timer (Timer3)

const char *teamHeader = "GW06";

const int counterLimit = 20;    // Number of intervals before deep sleep
const int safeWakeTime = 300;   // Wake guard for idle mode (ms)

int sleepDuration = 0;          // Will contain the pre-determined duration of the current idle sleep
int GWcounter = 0;              // Counter for keeping track of the current interval

// Flags for the power mode states
bool idleFlag = false;
bool powerDownFlag = false;

// Create handles
TaskHandle_t gatewayHandle, databaseHandle, sleepHandle, serialHandle;   // create task handles
SemaphoreHandle_t SemaphoreHndl;      // create semaphore handle for database access synchronization
QueueHandle_t logQueue;               // create queue handle for temporarily storing incoming data before writing it into the database


EDB db(&writer, &reader);             // create EEPROM database
EDB_Status dbst = EDB_OK;

struct Log {
  // Struct for storing data inside database. System is prepared to store more parameters.
  int msg;
  float data;
}logType;

void setup() {
  // Initialize the serial monitor port
  Serial.begin(9600);
  while (!Serial);
  
  initLoRa();   // Initialize LoRa comm

  setup_sleep();    // Initialize the waking conditions for sleep (interrupt pin)
  
  db.create(0, TABLE_SIZE, (unsigned int)sizeof(logType));    // create new EEPROM database
  //db.open(0);       // Uncomment for using an already stored EEPROM database, comment out db.create() in this case!

  SemaphoreHndl = xSemaphoreCreateBinary();     // initialize semaphore
  if( (SemaphoreHndl) != NULL)
    xSemaphoreGive((SemaphoreHndl));
  
  logQueue = xQueueCreate(10, sizeof(Log));     // initialize queue

  // Setting up tasks
  if(logQueue != NULL){
    // First initialize the producer-consumer tasks for the queue
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
  // Then the rest of the tasks
  xTaskCreate(
    SerialCommPC
    ,  (const portCHAR *)"SerialCommPC"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &serialHandle ); 

  delay(20);
  vTaskSuspend(serialHandle);   // Suspend the Serial comm PC task: it will only be enabled after we are finished with the GW comm
}

void loop() {
  // Empty. See tasks.ino for running task functions  
}
