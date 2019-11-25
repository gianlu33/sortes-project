void SerialInstructionHandlerPC(){
  int inputMsg = 0;
      while(Serial.available() > 0){
        inputMsg = Serial.parseInt();
          switch (inputMsg)
            case 1:
            readRecord(db.count());
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
