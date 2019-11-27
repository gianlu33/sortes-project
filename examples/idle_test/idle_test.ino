#include <SleepMode.h>

int val = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  while(Serial.available() > 0){
    val = Serial.parseInt();
    Serial.println(val);

/*
      digitalWrite(LED_BUILTIN, LOW);
      SleepMode.idle(ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, TIMER0_OFF,
                     SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);

      digitalWrite(LED_BUILTIN, HIGH);
*/
  }
}
