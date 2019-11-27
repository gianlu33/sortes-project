//LoR32u4II 868MHz or 915MHz (black board)
  #define SCK     15
  #define MISO    14
  #define MOSI    16
  #define SS      8
  #define RST     4
  #define DI0     7
  #define BAND    869100000  // 915E6
  #define PABOOST true 

 void initLoRa(){
  LoRa.setPins(SS,RST,DI0);// set CS, reset, IRQ pin
  if (!LoRa.begin(BAND,PABOOST)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
}

int onReceive(int packetSize) {
  if (packetSize == 0) return -1;  // if there's no packet, return
  
  String header = "";

  // Packet might not be sent with individual writes! We need to determine the format of the message:
  // Can we read it in bytes, or only the whole packet at once with a single read?
  
  // read packet header bytes:
  for (int i = 0; i<4; i++){
    header += (char)LoRa.read();
  }

  // Read the content byte of the message
  byte tD = LoRa.read();
  int timeData = tD-48;   // ASCII conversion to integer
  //Serial.println(timeData);

  return timeData;
}

void sendData(float data){
  LoRa.beginPacket();
  LoRa.write(data);
  LoRa.endPacket();
}
