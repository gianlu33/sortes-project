// Sleep functions

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
  
  //Serial.println("Sleeping");
  //delay(20);
  
  idleMode();
}

void setCounter() {
  // Handling counter values (and overfflow)
  if(cnt_left > 65535) { // maximum value that can be stored
    OCR3A = 65535;
    cnt_left -= 65535;
  }
  else {
    OCR3A = cnt_left;
    cnt_left = 0;
  }
}

ISR (TIMER3_COMPA_vect){ 
  // Interrupt handler function (triggered by Timer3, in idle mode)
  if(cnt_left != 0) {
    //Serial.println("Temp wake");
    //delay(20);
    setCounter();   // Reset counter in case of overflow (duration >8), and go back to sleep
  }
  else {
    // disable interrupt
    bitClear(TIMSK3, 1);
    enableModules();
    resumeTasks();
    idleFlag = false;
    //Serial.println("Waking up");
    //delay(20);
  }
}

void idleMode() {
  // Disable tasks, modules and enter idle sleep mode
  stopTasks();
  disableModules();
  while(idleFlag){
    SleepMode.enterIdleSleep();
  }               
  //Serial.println("Waking up");
  //delay(20);
}

void disableModules() {
    // Function for disabling modules
    SleepMode.disableModules(ADC_OFF, TIMER4_OFF, TIMER3_ON, TIMER1_OFF, TIMER0_OFF,
                     SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
                     // Set USB_ON to keep Serial comm with PC turned on during GW operation!
}

void enableModules() {
    // Function for enabling modules
    SleepMode.enableModules(ADC_ON, TIMER4_ON, TIMER3_ON, TIMER1_ON, TIMER0_ON,
                     SPI_ON, USART1_ON, TWI_ON, USB_ON);
}

// POWER DOWN MODE

void enterPowerDownMode() {
    attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, LOW); // Attach interrupt to WAKE_PIN
    Serial.println("Entering power down mode");   // print for visual confirmation
    delay(20);
    
    stopTasks();
    disableUSB();
    
    while(powerDownFlag) {
      SleepMode.enterPowerDown(ADC_OFF, BOD_OFF);
    }

    enableUSB();
    Serial.println("Wake up from power down mode");

#if RESTART_GW
    resumeTasks();      // Optionally, we can resume all tasks to restart GW comm from the beginning
    GWcounter = 0;
#else
    vTaskResume(serialHandle);    // Resume serial communication for receiving commands
#endif
    
    detachInterrupt(digitalPinToInterrupt(WAKE_PIN));   // Detach the interrupt
}

void wakeUp() {
  // Set the flags upon waking up by an interrupt, so that we will not resume sleeping
  powerDownFlag = false;
  idleFlag = false;
}

void disableUSB() {
  // Disable the USB prior going to sleep
  USBCON |= _BV(FRZCLK);  //freeze USB clock
  PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
  USBCON &= ~_BV(USBE);   // disable USB
}

void enableUSB() {
  // Enable the USB and the Serial port after waking up.
  // Effects visible and usable only if the user restarts the serial monitor on the PC
  USBDevice.attach();
  Serial.begin(9600);
  while(!Serial);
}
