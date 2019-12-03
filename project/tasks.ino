//#define N_TICKS 2

void vApplicationIdleHook(void) {
  // Idle task function, runs when no other tasks are executing
  // Used for going to sleep whenever the flags signal that we should be sleeping
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
  // Task handling communication with the gateway
  (void) pvParameters;
  
  Log logTemp;  // Temporary log structure
  while(1) {
      logTemp = onReceive(LoRa.parsePacket());    // Read data
      if (logTemp.msg != 0){                      // Check if message (sleeptime) is valid
        sleepDuration = logTemp.msg;      
        sendData(logTemp.data);                   // Send the measured temperature back to the gateway as acknowledgment
        GWcounter++;                              // Increase gateway interval counter
        xQueueSend(logQueue, &logTemp, portMAX_DELAY);  // Send logTemp to the queue  
        Serial.println(logTemp.msg);                // Print the received message for visual user confirmation
        vTaskDelay( (logTemp.msg*1000-safeWakeTime*2) / portTICK_PERIOD_MS);  // Delay (suspend execution) the task until slightly before the next interval, so the other tasks can run
      }
  }
}

void SerialCommPC( void *pvParameters) {
  // Task handling serial communication with the PC (user inputs)
  (void) pvParameters;  
    while(1) {
      SerialInstructionHandlerPC();   // Call the instruction handler function
      vTaskDelay( 20 / portTICK_PERIOD_MS);   // Delay the task so the others can run
  }
}

void DatabaseHandler( void *pvParameters) {
  // Task handling filling up the database
  (void) pvParameters;

    while(1) {    
      Log logTemp;
      if(xQueueReceive(logQueue, &logTemp, portMAX_DELAY) == pdPASS){
        // If the queue holds a new data, take the semaphore, and append the record to the DB
        if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
          addRecord(logTemp);
        }
        
        xSemaphoreGive(SemaphoreHndl);      // Give the semaphore back
        if(GWcounter < counterLimit){
            idleFlag = true;        // If the data was saved, we can go to idle sleep
        }
        else{
            idleFlag = false;      // If the set amount of intervals is reached, go to power down instead
            powerDownFlag = true;      // Use this for automatically entering PowerDown after 20 intervals, wake up with external interrupt to only enable Serial queries, not GW 
            //switchGWtoSerMode();     // Use this for not setting PowerDown automatically after 20 intervals, only from Serial manually
        }
      }
      
      //vTaskDelay(N_TICKS);
      vTaskDelay( 20 / portTICK_PERIOD_MS);
  }
}

void stopTasks() {
  // Function for suspending all tasks
  vTaskSuspend(gatewayHandle);
  //vTaskSuspend(serialHandle);
  vTaskSuspend(databaseHandle);
}

void resumeTasks() {
  // Function for resuming all tasks
  vTaskResume(gatewayHandle);
  //vTaskResume(serialHandle);
  vTaskResume(databaseHandle);
}

void switchGWtoSerMode(){
  // Function to call after 20 intervals, if we don't want to automatically enter Power Down mode. Keeps the serial handler task open.
  Serial.println("GW end, Ser start");
  stopTasks();
  vTaskResume(serialHandle);
}
