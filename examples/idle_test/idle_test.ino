#include <SleepMode.h>
#include <LoRa.h>

int sleepTime = 0;
int val;
int cnt =0;

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
    sendData(val);
      digitalWrite(LED_BUILTIN, LOW);
      setTimer(val - 1);
      idleMode();
      digitalWrite(LED_BUILTIN, HIGH);
      
    if (cnt > 5) {
      Serial.println("Power down");
      delay(20);
      powerDownMode();
    }
      cnt++;
  }
}
