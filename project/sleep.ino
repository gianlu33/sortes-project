#define WAKE_PIN 3 // select between 0, 1, 2, 3, 7
#define SYS_CLK 8e6
#define CNT_CLK SYS_CLK / 1024

bool is_idle = false; // to check if we have to go to sleep again if the interrupt is triggered
bool idle_finished = true; // to ensure that enterIdleMode() (the one without parameters) does not go to sleep if we have already finished
unsigned long cnt_left = 0;

//TODO wake up device if i send a command on the serial interface (maybe it is already working..)

void setup_sleep() {
  //pinMode(WAKE_PIN, INPUT_PULLUP);
}

// IDLE MODE

void enterIdleMode(int seconds) {
  //set the counter
  cnt_left = CNT_CLK * seconds - (CNT_CLK * safeWakeTime) / 1000;

  //set clock source clk / 1024 -> write 101 in CS32..CS30
  //set mode 7 (up to OCR3A)
  TCCR3A = 0;
  TCCR3B = 0b00001101;
  
  // clear the counter
  TCNT3 = 0; 

  // set the compare register
  setCounter(); 

  // clear pending ints
  bitSet(TIFR3,OCF3A);
  
  //enable interrupt
  bitSet(TIMSK3, 1);

  is_idle = true;
  idle_finished = false;
  
  Serial.println("Entering idle mode");
  delay(20);
  
  idleMode();
}

// Call this one if you were already sleeping, but it arrives a serial communication and you serve it, then you want to go to sleep again
// Note that the timer is already set, we don't have to do anything except for going to sleep
// If in the meanwhile the counter has reached 0, we don't have to sleep
void enterIdleMode() {
  if(!idle_finished) {
    is_idle = true;
    Serial.println("Entering idle mode");
    delay(20);
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
  if(cnt_left != 0) {     // maybe >= 0?
    //Serial.println("Temp wake");
    //delay(20);
    setCounter();

    // what happens if this routine is triggered when i'm reading from serial port? I can't go to sleep while i'm doing something else
    // When this occurs, idle must be set to false manually, do all the operations and then call enterIdleMode()
    //if(is_idle) {
      //idleMode();
    //}
  }
  else {
    // disable interrupt
      bitClear(TIMSK3, 1);
      disableIdleMode();
      resumeTasks();
      is_idle = false;
      idleFlag = false;
      idle_finished = true;
      Serial.println("Sleep finished");
      delay(20);
  }
}

void idleMode() {
   // stopping tasks
  stopTasks();

  //portSUPPRESS_TICKS_AND_SLEEP(cnt_left * 1024);
  enableIdleMode();
  while(idleFlag){
    SleepMode.enterIdleSleep();
    //SleepMode.idle(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
    //                 SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
                     //Serial.println("a");
                     //delay(5);
  }
  //vTaskDelay( 20 / portTICK_PERIOD_MS);
  //disableIdleMode();
  //resumeTasks();
                    
  //Serial.println("wake up");
}

// FAKE IDLE MODE
void enableIdleMode() {
    SleepMode.disableModules(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
                     SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
                     // USB_ON to turn on Serial during GW operation!
}

void disableIdleMode() {
    SleepMode.enableModules(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
                     SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
                     // USB_ON to turn on Serial during GW operation!
}

// POWER DOWN MODE

void enterPowerDownMode() {
    attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, LOW); // select between CHANGE, LOW, RISING, FALLING
    Serial.println("Entering power down mode");
    delay(20);
    stopTasks();
    SleepMode.powerDown(ADC_OFF, BOD_OFF);
    Serial.println("Wake up from power down mode");
    //detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
    vTaskResume(serialHandle);
    //vTaskResume(databaseHandle);
    detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}

void wakeUp() {
  //TODO verify if there is something to do
  Serial.println("Interrupt triggered");
  //resumeTasks();      // Optionally, we can resume all tasks to restart GW comm from the beginning
  //GWcounter = 0;

}
/*
void enterPowerDown(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  portENTER_CRITICAL();
  sleep_enable();
  #if defined(BODS) && defined(BODSE)
  sleep_bod_disable();
  #endif
  portEXIT_CRITICAL();
  sleep_cpu();
  sleep_reset();
}
*/
