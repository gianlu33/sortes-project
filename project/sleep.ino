//TODO interrupt handlers, pins, flags ...
int wake_pin = 3; // select between 0, 1, 2, 3, 7
int system_clock = 16e6;

void setup_sleep() {
  pinMode(wake_pin, INPUT_PULLUP); // select between INPUT and INPUT_PULLUP
}

void wakeUp() {
  // TODO remove flag, do something
  Serial.println("Interrupt triggered");

}

void enterIdleMode(int seconds) {
  //TODO: set timer

  //SleepMode.idle(...);
}

void enterPowerDownMode() {
    Serial.println("enteredF");
    attachInterrupt(digitalPinToInterrupt(wake_pin), wakeUp, CHANGE); // select between CHANGE, LOW, RISING, FALLING
    
    Serial.println("Entering low power mode");
    delay(20);
    SleepMode.powerDown(ADC_OFF, BOD_OFF);
    Serial.println("External interrupt received, waking up from low power mode");
    
    detachInterrupt(digitalPinToInterrupt(wake_pin));
}
