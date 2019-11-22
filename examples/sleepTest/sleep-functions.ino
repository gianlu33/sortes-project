#include <LowPower.h>

// Use pin 2 as wake up pin
const int wakeUpPin = 2;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void powerDown() {
      LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);  
}

void idle(int seconds) {
    //TODO setting timer 0
    
    //LowPower.idle(ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON, 
      //          SPI_OFF, USART0_OFF, TWI_OFF);
}
