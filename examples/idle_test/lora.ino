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
