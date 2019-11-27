/*
void SleepTask(void *pvParameters) {
  (void) pvParameters;
    
  while(1) {
      if(powerDownFlag) {
        Serial.println("Power down mode");
        // go to power down mode
        enterPowerDownMode();
        powerDownFlag = false;
      }
      if(idleFlag) {
        Serial.println("idle mode");
        // go to idle mode
        enterIdleMode(sleepDuration);
        idleFlag = false;
      }

      vTaskDelay( 20 / portTICK_PERIOD_MS);
  }
}
*/

void vApplicationIdleHook(void) {
      if(powerDownFlag) {
        // go to power down mode
        enterPowerDownMode();
        powerDownFlag = false;
        idleFlag = false;
      }
      else if(idleFlag) {
        // go to idle mode
        enterIdleMode(sleepDuration);
        idleFlag = false;
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
        if(GWcounter >= counterLimit){
          powerDownFlag = true;
        }
        vTaskDelay( (logTemp.msg*1000-safeWakeTime*2) / portTICK_PERIOD_MS);
      }
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
        
        xSemaphoreGive(SemaphoreHndl);
        if(GWcounter < 20){
            idleFlag = true;
        }
        else{
            idleFlag = false;
        }
      }
      
      //TODO
      vTaskDelay( 20 / portTICK_PERIOD_MS);
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
