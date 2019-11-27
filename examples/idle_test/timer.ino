/*#define SYS_CLK 16e6
#define CNT_CLK SYS_CLK / 1024

unsigned int cnt_left = 0;

void setTimer(int seconds){
    cnt_left = CNT_CLK * seconds;   // I think this might be wrong, as the clock is continuously running
  
  //set clock source clk / 1024 -> write 101 in CS32..CS30
  //set mode 7 (up to OCR3A)
  //TODO change only the bits i'm interested in
  //TODO this can be moved in the "setup" phase
  TCCR3A = 0;
  TCCR3B = 0b00001101;

  
  //TODO check the order of these instructions
  // clear the counter
  TCNT3 = 0; 

  // set the compare register
  setCounter(); 
  
  //enable interrupt
  bitSet(TIMSK3, 1);
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
    setCounter();

    // what happens if this routine is triggered when i'm reading from serial port? I can't go to sleep while i'm doing something else
    // When this occurs, idle must be set to false manually, do all the operations and then call enterIdleMode()
    if(is_idle) {
      idleMode();
    }
  }
  else {
    // disable interrupt
      bitClear(TIMSK3, 1);
      is_idle = false;
      idle_finished = true;
  }
}
*/
