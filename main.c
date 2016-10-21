/*
  Interface: main.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "main.h"
#include "uart.h"
#include "switches.h"
#include "treadmill.h"
#include "serialParser.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdbool.h>

//==================//
//=== INTERRUPTS ===//
//==================//

volatile int oneSecondCounter = 0;
volatile bool oneSecondFlag = false;

ISR(TIMER0_OVF_vect)
{
  oneSecondCounter += 1;
  const int ovfPerSecond = 61; // 15625 [cnts/s] / 256 [cnts/ovf]
  if (oneSecondCounter > ovfPerSecond) {
    oneSecondFlag = true;
    oneSecondCounter = 0;
  }
}

switches_t mySwitches;
treadmill_t myTreadmill;

ISR(USART_RX_vect)
{
  unsigned char rxByte = UDR0;
  serialParser_parse(mySwitches.protocol, &myTreadmill, rxByte);
}

//============//
//=== MAIN ===//
//============//

int main()
{
  float* currentDistance_ptr = (float*)sizeof(uint8_t);
  char output[8];
  float currentDistance_km = 0;

  if (!eeprom_read_byte(0)){
    eeprom_write_float(currentDistance_ptr, currentDistance_km);
    eeprom_write_byte(0,1);
  } else currentDistance_km = eeprom_read_float(currentDistance_ptr);

  mySwitches = getSwitches();
  myTreadmill = treadmill_init(mySwitches.treadmill);
  sei(); // enable interrupts
  treadmill_resetInclination();

  switch(mySwitches.protocol) {
    default:
    case INBRAMED: uart_init(9600); break;
    case TRACKMASTER_KMPH:
    case TRACKMASTER_MPH: uart_init(4800); break;
  }

  TCCR0B |= (1 << CS02)|(1 << CS00); // start time control, CLK = 15.625kHz

  // SUPER LOOP:

  while (true) {
    if (mySwitches.treadmill != DEBUG) treadmill_update(&myTreadmill);
    if (oneSecondFlag) {
      oneSecondFlag = false;
      currentDistance_km += myTreadmill.speed_kmph/3600; // [km/h]/[s/h]
      if (currentDistance_km > myTreadmill.lubDistance_km) {
        currentDistance_km = 0;
        PORTB |= (1 << PORTB2); // switch lub on
      } else PORTB &= ~(1 << PORTB2); // switch lub off
      if (eeprom_is_ready()) eeprom_write_float(currentDistance_ptr, currentDistance_km);
    }
  }

  return 0;
}
