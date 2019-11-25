//TODO functions

struct Log onReceive(int packetSize) {
  Log logTemp;
  logTemp.msg = 0;
  logTemp.data = 0;
  if (packetSize == 0) return logTemp;  // if there's no packet, return
  
  String header = "";

  // Packet might not be sent with individual writes! We need to determine the format of the message:
  // Can we read it in bytes, or only the whole packet at once with a single read?
  
  // read packet header bytes:
  for (int i = 0; i<4; i++){
    header += (char)LoRa.read();
  }
  
  /*
  // If the msg is encoded as characters
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  int timeData = incoming.toInt();
  */

  // If the msg is sent as a single byte (or maybe int?)
  byte tD = LoRa.read();
  int timeData = tD-48;   // ASCII conversion to an integer
  //Serial.println(timeData);
  float temperatureData = getTemp();

  logTemp.msg = timeData;
  logTemp.data = temperatureData;

  return logTemp;
}

void sendData(float d){
  LoRa.beginPacket();
  LoRa.write(d);
  LoRa.endPacket();
}
