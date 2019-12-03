// Gateway communication handler functions

struct Log onReceive(int packetSize) {
  Log logTemp;
  logTemp.msg = 0;
  logTemp.data = 0;
  if (packetSize == 0) return logTemp;  // if there's no packet, return empty log
  
  char header[4];

  // Packet might not be sent with individual writes! We need to determine the format of the message:
  // Can we read it in bytes, or only the whole packet at once with a single read?

  bool headerMismatchFlag = false; // Flag for signaling when the header is not equal to the expected team header
  // read packet header bytes:
  for (int i = 0; i<4; i++){
    char readchar = (char)LoRa.read();
    header[i]=readchar;
    if(readchar != teamHeader[i]){
      headerMismatchFlag = true;
    }
  }

  if(headerMismatchFlag) return logTemp;  // Return empty log if the header was mismatched
    
  // Read the content byte of the message
  byte tD = LoRa.read();
  int timeData = tD-48;   // ASCII conversion to integer
  float temperatureData = getTemp();  // measure temperature
  
  // Save data to a temp log struct
  logTemp.msg = timeData;
  logTemp.data = temperatureData;

  return logTemp; // Return the log
}

void sendData(float data){
  LoRa.beginPacket();   // Begin LoRa packet
  LoRa.write(data);     // Write data: the measured temperature value, which is a float
  LoRa.endPacket();     // End LoRa packet
}
