//TODO interrupt handlers, pins, flags ...
#define WAKE_PIN 3 // select between 0, 1, 2, 3, 7
#define SYS_CLK 16e6
#define CNT_CLK SYS_CLK / 1024

void setup_sleep() {
  pinMode(WAKE_PIN, INPUT_PULLUP); // select between INPUT and INPUT_PULLUP
}

void wakeUp() {
  // TODO remove flag, do something
  Serial.println("Interrupt triggered");

}

// TCCR3A
// 7       6       5      4      3     2      1     0
//COM3A1 COM3A0 COM3B1 COM3B0 COM3C1 COM3C0 WGM31 WGM30
// TCCR3B
// 7    6    5   4      3    2     1    0
//CNC3 ICES3 – WGM33 WGM32 CS32 CS31 CS30

void enterIdleMode(int seconds) {
  //set the counter
  //TODO now i will set one second
  OCR3A = CNT_CLK;
  
  //set clock source = clk / 8 -> write 010 to CS32..CS30 for /8, write 101 for /1024
  //set mode 7 (up to OCR3A)
  //TODO change only the bits i'm interested in
  TCCR3A = 0;
  TCCR3B = 0b00001101;

  //enable interrupt
  TIMSK3 |= 0b00000010;
  
  //SleepMode.idle(...);
}

ISR (TIMER3_COMPA_vect)
{
  Serial.println("Interrupt!");
}

void enterPowerDownMode() {
    Serial.println("enteredF");
    attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, CHANGE); // select between CHANGE, LOW, RISING, FALLING
    
    Serial.println("Entering low power mode");
    delay(20);
    SleepMode.powerDown(ADC_OFF, BOD_OFF);
    Serial.println("External interrupt received, waking up from low power mode");
    
    detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}
