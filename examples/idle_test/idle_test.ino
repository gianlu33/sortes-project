#include <SleepMode.h>
#include <LoRa.h>

int sleepTime = 0;
int val;
int cnt =0;

const int WAKE_PIN = 2;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(WAKE_PIN, INPUT);
}

void loop() {
  
  while(Serial.available() > 0){
      sleepTime = Serial.parseInt();
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Going to sleep");
      delay(20);
      attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, LOW);
      setTimer(sleepTime);
      //idleMode();
      powerDownMode();
      delay(20);
      Serial.println("Woke");
      delay(20);
      digitalWrite(LED_BUILTIN, HIGH);

  }
  
  /*val = onReceive(LoRa.parsePacket());
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
  }*/
}

void wakeUp(){
  Serial.println("IR");
  detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}
