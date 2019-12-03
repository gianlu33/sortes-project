// Serial communication (user input) handler functions

void SerialInstructionHandlerPC(){
  // Function for handling serial communication with the PC (user inputs through the serial monitor)
  int inputMsg = 0;
      while(Serial.available() > 0){
        //Serial.println("Enter command (1 - read last record, 2 - read all records, 3 - enter ultra low power mode):");
        //Serial.println("runs");
        inputMsg = Serial.parseInt();
          switch (inputMsg){
            case 1:
              // Read last record from the EEPROM database
              if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
                readRecord(db.count());
                xSemaphoreGive(SemaphoreHndl);
              }  
            break;
            case 2:
              // Read all records from the EEPROM database
              if(xSemaphoreTake(SemaphoreHndl, (TickType_t) 5) == pdTRUE){
                readAllRec();
                xSemaphoreGive(SemaphoreHndl);
              }
            break;
            case 3:
              // Enter ultra low power mode
              powerDownFlag = true;
            break;
            default:
            break;  
          }
      }
}
