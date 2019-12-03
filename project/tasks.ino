//#define N_TICKS 2

void vApplicationIdleHook(void) {
      if(powerDownFlag) {
        // go to power down mode
        enterPowerDownMode();
      }
      else if(idleFlag) {
        // go to idle mode
        enterIdleMode(sleepDuration);
      }
}

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
        vTaskDelay( (logTemp.msg*1000-safeWakeTime*2) / portTICK_PERIOD_MS);
      }
  }
}

void SerialCommPC( void *pvParameters) {
  (void) pvParameters;  
    while(1) {
      SerialInstructionHandlerPC();
      //vTaskDelay(N_TICKS);
      vTaskDelay( 65 / portTICK_PERIOD_MS);
  }
}

void DatabaseHandler( void *pvParameters) {
  (void) pvParameters;

    while(1) {    
      Log logTemp;
      if(xQueueReceive(logQueue, &logTemp, portMAX_DELAY) == pdPASS){
        if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
          addRecord(logTemp);
        }
        
        xSemaphoreGive(SemaphoreHndl);
        if(GWcounter < counterLimit){
            idleFlag = true;
        }
        else{
            idleFlag = false;
            powerDownFlag = true;  // Use this for automatic PowerDown setting after 20 intervals, wake up with external interrupt to only enable Serial queries, not GW 
            //switchGWtoSerMode();     // Use this for not setting PowerDown automatically after 20 intervals, only from Serial manually
        }
      }
      
      //vTaskDelay(N_TICKS);
      vTaskDelay( 65 / portTICK_PERIOD_MS);
  }
}

void stopTasks() {
  vTaskSuspend(gatewayHandle);
  vTaskSuspend(serialHandle);
  vTaskSuspend(databaseHandle);
}

void resumeTasks() {
  vTaskResume(gatewayHandle);
  vTaskResume(serialHandle);
  vTaskResume(databaseHandle);
}

void switchGWtoSerMode(){
  Serial.println("GW end, Ser start");
  vTaskSuspend(gatewayHandle);   
  vTaskSuspend(databaseHandle);
}
