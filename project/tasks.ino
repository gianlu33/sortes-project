/*void vApplicationIdleHook( void ) {
      // Allow wake up pin to trigger interrupt on low.
   // attachInterrupt(0, wakeUp, LOW);
   
    //TODO going to sleep
      Serial.println("Sleep");
      //TODO
    
    // Disable external pin interrupt on wake up pin.
    //detachInterrupt(0); 
}*/

void GatewayComm( void *pvParameters) {
  (void) pvParameters;
  
  while(1) {
      Log logTemp;
      logTemp = onReceive(LoRa.parsePacket());
      if (logTemp.msg != 0){
        sleepDuration = logTemp.msg;
        sendData(logTemp.data);
      }

      // Put longtemp to queue
      
      // sleep for duration
      
      
      //TODO
      //Serial.println(tempMsg);
      //vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

void SerialCommPC( void *pvParameters) {
  (void) pvParameters;  
    while(1) {
      SerialInstructionHandlerPC();
  }
}

void DatabaseHandler( void *pvParameters) {
  (void) pvParameters;

    while(1) {
      //Serial.println("Hello DatabaseHandler");
      
      if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
        // Access queue and save the data
        addRecord(logType);
        xSemaphoreGive(SemaphoreHndl);
      }
      //TODO
      vTaskDelay( 250 / portTICK_PERIOD_MS);

  }
}
