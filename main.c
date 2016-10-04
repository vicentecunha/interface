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

//=== INTERRUPTS ===//

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

//=== MAIN ===//

int main()
{
  float* currentDistance_ptr = (float*)sizeof(uint8_t);
  char output[8];
  float currentDistance_km = 0;

  if (!eeprom_read_byte(0)){
    eeprom_write_float(currentDistance_ptr, currentDistance_km);
    eeprom_write_byte(0,1);
  } else currentDistance_km = eeprom_read_float(currentDistance_ptr);

  switches_t mySwitches = getSwitches();
  treadmill_t myTreadmill = treadmill_init(mySwitches.treadmill);
  sei(); // enable interrupts
  treadmill_resetInclination();
  serialParser_t mySerialParser = serialParser_init(mySwitches.protocol);
  uart_init();
  TCCR0B |= (1 << CS02)|(1 << CS00); // start time control, CLK = 15.625kHz

  while (1) {
    if (mySwitches.treadmill != DEBUG) treadmill_update(&myTreadmill);
    if (rxFlag) {
      rxFlag = false;
      serialParser_parse(&mySerialParser, &myTreadmill, rxByte);
      //snprintf(output, 8, "%d\r\n", encoderCounts);
      //uart_sendCstring(output);
    }
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
