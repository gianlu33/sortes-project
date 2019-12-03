// Initializer functions

void initLoRa(){
  LoRa.setPins(SS,RST,DI0);             // set CS, reset, IRQ pin
  if (!LoRa.begin(BAND,PABOOST)) {      // initialize ratio at 915 MHz and the defined bandwidth
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
}

void setup_sleep() {
  // Configure the mode of the wake input pin
  pinMode(WAKE_PIN, INPUT_PULLUP);
}
