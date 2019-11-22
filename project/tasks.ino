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
      String tempMsg = onReceive(LoRa.parsePacket());
      //TODO
      //Serial.println(tempMsg);
      //vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

void SerialCommPC( void *pvParameters) {
  (void) pvParameters;

    while(1) {
      //Serial.println("Hello SerialCommPC");
      //TODO
      vTaskDelay( 500 / portTICK_PERIOD_MS);
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
