void initLoRa(){
  LoRa.setPins(SS,RST,DI0);// set CS, reset, IRQ pin
  if (!LoRa.begin(BAND,PABOOST)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
}
