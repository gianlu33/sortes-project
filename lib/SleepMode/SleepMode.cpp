#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "SleepMode.h"

#define DISABLE_BOD 0

/* TODO

1- verify if we can change BOD settings through software
  LowPower library says our device can't, infact if we enable DISABLE_BOD we get
  compilation errors

2- Modify the modes in the following way:
  A. The watchdog timer CANNOT be used -> it is used by the FreeRTOS library
     Verify if we can disable it safely during sleep mode (power down)

  B. For idle mode, maybe set here the timer which will wake up the device

  C. For power down mode, maybe set here which pin we can use to trigger an interrupt
     in order to wake up the device

  D. See if we can disable other components in order to save power:
      - BOD
      - Internal Voltage Reference
      - Watchdog timer
      - Port pins
      - On-Chip debug system

*/

// Only Pico Power devices can change BOD settings through software
#if DISABLE_BOD
#ifndef sleep_bod_disable
#define sleep_bod_disable() 										\
do { 																\
  unsigned char tempreg; 													\
  __asm__ __volatile__("in %[tempreg], %[mcucr]" "\n\t" 			\
                       "ori %[tempreg], %[bods_bodse]" "\n\t" 		\
                       "out %[mcucr], %[tempreg]" "\n\t" 			\
                       "andi %[tempreg], %[not_bodse]" "\n\t" 		\
                       "out %[mcucr], %[tempreg]" 					\
                       : [tempreg] "=&d" (tempreg) 					\
                       : [mcucr] "I" _SFR_IO_ADDR(MCUCR), 			\
                         [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)), \
                         [not_bodse] "i" (~_BV(BODSE))); 			\
} while (0)
#endif
#endif

#define	lowPowerBodOn(mode)	\
do { 						\
      set_sleep_mode(mode); \
      cli();				\
      sleep_enable();		\
      sei();				\
      sleep_cpu();			\
      sleep_disable();		\
      sei();				\
} while (0);

// Only Pico Power devices can change BOD settings through software
#if DISABLE_BOD
#define	lowPowerBodOff(mode)\
do { 						\
      set_sleep_mode(mode); \
      cli();				\
      sleep_enable();		\
			sleep_bod_disable(); \
      sei();				\
      sleep_cpu();			\
      sleep_disable();		\
      sei();				\
} while (0);
#endif

/*
Some definitions for our device
*/

// Timer 4 PRR bit is currently not defined in iom32u4.h
#ifndef PRTIM4
	#define PRTIM4 4
#endif

// Timer 4 power reduction macro is not defined currently in power.h
#ifndef power_timer4_disable
	#define power_timer4_disable()	(PRR1 |= (uint8_t)(1 << PRTIM4))
#endif

#ifndef power_timer4_enable
	#define power_timer4_enable()		(PRR1 &= (uint8_t)~(1 << PRTIM4))
#endif

/*******************************************************************************
* Name: idle
* Description: Putting ATmega32U4 into idle state. Please make sure you
*			   understand the implication and result of disabling module.
*			   Take note that Timer 2 is not available and USART0 is replaced
* 			   with USART1 on ATmega32U4.
*
* Argument  	Description
* =========  	===========
*
* 1. adc		ADC module disable control:
*				(a) ADC_OFF - Turn off ADC module
*				(b) ADC_ON - Leave ADC module in its default state
*
* 2. timer4		Timer 4 module disable control:
*				(a) TIMER4_OFF - Turn off Timer 4 module
*				(b) TIMER4_ON - Leave Timer 4 module in its default state
*
* 3. timer3		Timer 3 module disable control:
*				(a) TIMER3_OFF - Turn off Timer 3 module
*				(b) TIMER3_ON - Leave Timer 3 module in its default state
*
* 4. timer1		Timer 1 module disable control:
*				(a) TIMER1_OFF - Turn off Timer 1 module
*				(b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 5. timer0		Timer 0 module disable control:
*				(a) TIMER0_OFF - Turn off Timer 0 module
*				(b) TIMER0_ON - Leave Timer 0 module in its default state
*
* 6. spi		SPI module disable control:
*				(a) SPI_OFF - Turn off SPI module
*				(b) SPI_ON - Leave SPI module in its default state
*
* 7. usart1		USART1 module disable control:
*				(a) USART1_OFF - Turn off USART1  module
*				(b) USART1_ON - Leave USART1 module in its default state
*
* 8. twi		TWI module disable control:
*				(a) TWI_OFF - Turn off TWI module
*				(b) TWI_ON - Leave TWI module in its default state
*
* 9.usb		USB module disable control:
*				(a) USB_OFF - Turn off USB module
*				(b) USB_ON - Leave USB module in its default state
*******************************************************************************/
void	SleepModeClass::idle(adc_t adc,
							timer4_t timer4, timer3_t timer3,
							timer1_t timer1, timer0_t timer0,
							spi_t spi, usart1_t usart1,	twi_t twi, usb_t usb)
{
	if (adc == ADC_OFF)
	{
		ADCSRA &= ~(1 << ADEN);
		power_adc_disable();
	}

	if (timer4 == TIMER4_OFF)	power_timer4_disable();
	if (timer3 == TIMER3_OFF)	power_timer3_disable();
	if (timer1 == TIMER1_OFF)	power_timer1_disable();
	if (timer0 == TIMER0_OFF)	power_timer0_disable();
	if (spi == SPI_OFF)			power_spi_disable();
	if (usart1 == USART1_OFF)	power_usart1_disable();
	if (twi == TWI_OFF)			power_twi_disable();
	if (usb == USB_OFF)			power_usb_disable();

	lowPowerBodOn(SLEEP_MODE_IDLE);

	if (adc == ADC_OFF)
	{
		power_adc_enable();
		ADCSRA |= (1 << ADEN);
	}

	if (timer4 == TIMER4_OFF)	power_timer4_enable();
	if (timer3 == TIMER3_OFF)	power_timer3_enable();
	if (timer1 == TIMER1_OFF)	power_timer1_enable();
	if (timer0 == TIMER0_OFF)	power_timer0_enable();
	if (spi == SPI_OFF)			power_spi_enable();
	if (usart1 == USART1_OFF)	power_usart1_enable();
	if (twi == TWI_OFF)			power_twi_enable();
	if (usb == USB_OFF)			power_usb_enable();
}

/*******************************************************************************
* Name: powerDown
* Description: Putting microcontroller into power down state. This is
*			   the lowest current consumption state. Use this together with
*			   external pin interrupt to wake up through external event
*			   triggering (example: RTC clockout pin, SD card detect pin).
*
* Argument  	Description
* =========  	===========
*
* 1. adc		ADC module disable control. Turning off the ADC module is
*				basically removing the purpose of this low power mode.
*				(a) ADC_OFF - Turn off ADC module
*				(b) ADC_ON - Leave ADC module in its default state
*
* 2. bod		Brown Out Detector (BOD) module disable control:
*				(a) BOD_OFF - Turn off BOD module
*				(b) BOD_ON - Leave BOD module in its default state
*
*******************************************************************************/
void	SleepModeClass::powerDown(adc_t adc, bod_t bod)
{
	if (adc == ADC_OFF)	ADCSRA &= ~(1 << ADEN);

	if (bod == BOD_OFF)
	{
		#if DISABLE_BOD
			lowPowerBodOff(SLEEP_MODE_PWR_DOWN);
		#else
			lowPowerBodOn(SLEEP_MODE_PWR_DOWN);
		#endif
	}
	else
	{
		lowPowerBodOn(SLEEP_MODE_PWR_DOWN);
	}

	if (adc == ADC_OFF) ADCSRA |= (1 << ADEN);
}

SleepModeClass SleepMode;
