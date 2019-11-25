void SerialInstructionHandlerPC(){
  int inputMsg = 0;
      while(Serial.available() > 0){
        //Serial.println("Enter command (1 - read last record, 2 - read all records, 3 - enter ultra low power mode):");
        Serial.println("runs");
        inputMsg = Serial.parseInt();
          switch (inputMsg){
            case 1:
              readRecord(db.count());
            break;
            case 2:
              readAllRec();
            break;
            case 3:
              enterPowerDownMode();
            break;
            default:
            break;  
          }
      }
}
