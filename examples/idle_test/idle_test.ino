#include <SleepMode.h>
#include <LoRa.h>

int sleepTime = 0;
int val;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  /*
  while(Serial.available() > 0){
      sleepTime = Serial.parseInt();
      
      digitalWrite(LED_BUILTIN, LOW);
      setTimer(sleepTime);
      idleMode();
      digitalWrite(LED_BUILTIN, HIGH);

  }
  */
  val = onReceive(LoRa.parsePacket());
  if(val != -1) {
    Serial.println(val);
    delay(20);
      digitalWrite(LED_BUILTIN, LOW);
      setTimer(20);
      idleMode();
      digitalWrite(LED_BUILTIN, HIGH);
  }
}
