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
      Log logTemp = onReceive(LoRa.parsePacket());
      sleepDuration = logTemp.msg;
      // sleep for duration

      
      //TODO
      //Serial.println(tempMsg);
      //vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

void SerialCommPC( void *pvParameters) {
  (void) pvParameters;
    int inputMsg = 0;
    while(1) {
      while(Serial.available() > 0){
        inputMsg = Serial.parseInt();
          switch (inputMsg)
            case 1:
            readRecord(db.count()-1);
            break;
            case 2:
            readAllRec();
            break;
            case 3:
            //lowPower
            Serial.println("Entering low power mode")
            break;
            default:
            break;  
      }
  }
}

void DatabaseHandler( void *pvParameters) {
  (void) pvParameters;

    while(1) {
      //Serial.println("Hello DatabaseHandler");
      //TODO
      vTaskDelay( 250 / portTICK_PERIOD_MS);

  }
}
