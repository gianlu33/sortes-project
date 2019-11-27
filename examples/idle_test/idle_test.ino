#include <SleepMode.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  while(Serial.available() > 0){
      Serial.parseInt();
      
      digitalWrite(LED_BUILTIN, LOW);
      setTimer(20);
      idleMode();
      digitalWrite(LED_BUILTIN, HIGH);

  }
}
