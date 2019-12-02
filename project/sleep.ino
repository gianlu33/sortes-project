#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define WAKE_PIN 3 // select between 0, 1, 2, 3, 7
#define SYS_CLK 8e6
#define CNT_CLK SYS_CLK / 1024

unsigned long cnt_left = 0;

void setup_sleep() {
  pinMode(WAKE_PIN, INPUT_PULLUP);
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
  
  Serial.println("Entering idle mode");
  delay(20);
  
  idleMode();
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
    //Serial.println("Temp wake");
    //delay(20);
    setCounter();
  }
  else {
    // disable interrupt
      bitClear(TIMSK3, 1);
      idleFlag = false;
  }
}

void idleMode() {
   // stopping tasks
  stopTasks();

  disableModules();
  while(idleFlag){
    sleep_idle();
  }

  Serial.println("wake up");
  
  enableModules();
  resumeTasks();          
}

// POWER DOWN MODE

void enterPowerDownMode() {
    attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, LOW); // select between CHANGE, LOW, RISING, FALLING
    Serial.println("Entering power down mode");
    delay(20);
    
    stopTasks();
    disableUSB();
    
    while(powerDownFlag) {
      sleep_powerDown();
    }

    enableUSB();
    Serial.println("Wake up from power down mode");

#if RESTART_GW
    resumeTasks();      // Optionally, we can resume all tasks to restart GW comm from the beginning
    GWcounter = 0;
#else
    vTaskResume(serialHandle);
#endif
    
    detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}

void wakeUp() {
  powerDownFlag = false;
  idleFlag = false;
}


// Functions to enable / disable modules and to actually go to sleep

void disableUSB() {
  // disable the USB prior going to sleep
  USBCON |= _BV(FRZCLK);  //freeze USB clock
  PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
  USBCON &= ~_BV(USBE);   // disable USB
}

void enableUSB() {
  USBDevice.attach(); // keep this
  Serial.begin(9600);
  while(!Serial);
}

void disableModules() {
  ADCSRA &= ~(1 << ADEN);
  power_adc_disable();
    
  //power_timer3_disable(); // We need this!
  power_timer1_disable();
  power_timer0_disable();
  power_spi_disable();
  power_usart1_disable();
  power_twi_disable();
//power_usb_disable();  // USB_ON to turn on Serial during GW operation!
}

void enableModules() {
  power_adc_enable();
  ADCSRA |= (1 << ADEN);

  //power_timer3_enable(); // We need this!
  power_timer1_enable();
  power_timer0_enable();
  power_spi_enable();
  power_usart1_enable();
  power_twi_enable();
  //power_usb_enable(); // USB_ON to turn on Serial during GW operation!
}

void sleep_powerDown() {
  ADCSRA &= ~(1 << ADEN);

  sleep(SLEEP_MODE_PWR_DOWN);

  ADCSRA |= (1 << ADEN);
}

void sleep_idle() {
  sleep(SLEEP_MODE_IDLE);
}

void sleep(int mode){
  set_sleep_mode(mode);
  cli();
  sleep_enable();
  sei();
  sleep_cpu(); // go to sleep here
  sleep_disable(); // wake up here
  sei();
}
