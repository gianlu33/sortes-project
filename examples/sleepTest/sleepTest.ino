#include <Arduino_FreeRTOS.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    // Configure wake up pin as input.
    // This will consumes few uA of current.
    //pinMode(wakeUpPin, INPUT); 
}

void loop() {}
