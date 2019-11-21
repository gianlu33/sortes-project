#include <EDB.h>
#include <Arduino_FreeRTOS.h>
#include <SleepMode.h>

void vApplicationIdleHook( void );

// Use pin 2 as wake up pin
const int wakeUpPin = 2;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}


void setup() {
  // put your setup code here, to run once:
    // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

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

void loop() {
  // put your main code here, to run repeatedly:

}

void vApplicationIdleHook( void ) {
      // Allow wake up pin to trigger interrupt on low.
   // attachInterrupt(0, wakeUp, LOW);

    Serial.write("Going to sleep\n");
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    //LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
    //LowPower.powerStandby(SLEEP_4S, ADC_OFF, BOD_OFF); 

    Serial.write("Waking up\n");

    // Disable external pin interrupt on wake up pin.
    //detachInterrupt(0); 
}
