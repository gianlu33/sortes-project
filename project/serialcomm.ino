void SerialInstructionHandlerPC(){
  int inputMsg = 0;
      while(Serial.available() > 0){
        //Serial.println("Enter command (1 - read last record, 2 - read all records, 3 - enter ultra low power mode):");
        //Serial.println("runs");
        inputMsg = Serial.parseInt();
          switch (inputMsg){
            case 1:
              // Read last record
              if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
                readRecord(db.count());
                xSemaphoreGive(SemaphoreHndl);
              }  
            break;
            case 2:
              // Read all records
              if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
                readAllRec();
                xSemaphoreGive(SemaphoreHndl);
              }
            break;
            case 3:
              // Enter ultra low power mode
              enterPowerDownMode();
              //enterIdleMode(10);
              //SleepMode.idle(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
                     //SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
              
            break;
            default:
            break;  
          }
      }
}
