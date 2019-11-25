#define WAKE_PIN 3 // select between 0, 1, 2, 3, 7
#define SYS_CLK 16e6
#define CNT_CLK SYS_CLK / 1024

bool isIdle = false;
unsigned int cnt_left = 0;

//TODO wake up device if i send a command on the serial interface

void setup_sleep() {
  pinMode(WAKE_PIN, INPUT_PULLUP);
}

void wakeUp() {
  //TODO verify if there is something to do
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
  cnt_left = CNT_CLK * seconds - safeWakeTime;
  
  setCounter();

  //set clock source clk / 1024 -> write 101 in CS32..CS30
  //set mode 7 (up to OCR3A)
  //TODO change only the bits i'm interested in
  //TODO this can be moved in the "setup" phase
  TCCR3A = 0;
  TCCR3B = 0b00001101;
  
  //enable interrupt
  bitSet(TIMSK3, 1);

  isIdle = true;
  idleMode();
}

// Call this one if we were already sleeping, but it arrives a serial communication and you serve it, then you want to go to sleep again
// Note that the timer is already set, we don't have to do anything except for going to sleep
// If in the meanwhile the counter has reached 0, we don't have to sleep
void enterIdleMode() {
  if(cnt_left != 0) {
    isIdle = true;
    idleMode();
  }
}

void setCounter() {
  if(cnt_left > 65535) { // maximum value that can be stored
    OCR3A = 65535;
    cnt_left -= 65535;
  }
  else {
    OCR3A = cnt_left;
    cnt_left = 0;
  }
}

ISR (TIMER3_COMPA_vect)
{
  if(cnt_left != 0) {
    setCounter();

    // what happens if this routine is triggered when i'm reading from serial port? I can't go to sleep while i'm doing something else
    // When this occurs, idle must be set to false manually, do all the operations and then call enterIdleMode()
    if(isIdle) {
      idleMode();
    }
  }
  else {
    // disable interrupt
      bitClear(TIMSK3, 1);
      isIdle = false;
  }
}

void idleMode() {
  //TODO verify if it's all correct
  SleepMode.idle(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
                     SPI_OFF, USART1_ON, TWI_OFF, USB_OFF);
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
