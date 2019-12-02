#define PIN_MODE INPUT_PULLUP
#define INT_MODE CHANGE

void setPins() {
  pinMode(0, PIN_MODE);
  pinMode(1, PIN_MODE);
  pinMode(2, PIN_MODE);
  pinMode(3, PIN_MODE);
}

void attachInterrupts() {
  attachInterrupt(0, wakeUp, INT_MODE);
  attachInterrupt(1, wakeUp, INT_MODE);
  attachInterrupt(2, wakeUp, INT_MODE);
  attachInterrupt(3, wakeUp, INT_MODE);
}

void detachInterrupts() {
  detachInterrupt(0);
  detachInterrupt(1);
  detachInterrupt(2);
  detachInterrupt(3);
}
