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
  Log logTemp;
  
  while(1) {
      logTemp = onReceive(LoRa.parsePacket());
      if (logTemp.msg != 0){
        sleepDuration = logTemp.msg;
        sendData(logTemp.data);
        // Increase gateway interval counter
        GWcounter++;
        // Put logTemp to queue  
        xQueueSend(logQueue, &logTemp, portMAX_DELAY);
        Serial.println(logTemp.msg);
        if(GWcounter >= 20){
          deepSleepFlag = 1;
        }
        vTaskDelay( (logTemp.msg*1000-safeWakeTime*2) / portTICK_PERIOD_MS);
      }
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
      vTaskDelay( 20 / portTICK_PERIOD_MS);
  }
}

void DatabaseHandler( void *pvParameters) {
  (void) pvParameters;

    while(1) {
      //Serial.println("Hello DatabaseHandler");
      
      
      Log logTemp;
      if(xQueueReceive(logQueue, &logTemp, portMAX_DELAY) == pdPASS){
        if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
          addRecord(logTemp);
        }
      }
      xSemaphoreGive(SemaphoreHndl);
      goToSleepFlag = 1;
      
      //TODO
      vTaskDelay( 20 / portTICK_PERIOD_MS);

  }
}
