/*
  Interface: switches.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "switches.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

switches_t getSwitches() {
  // Enable pull-ups:
  PORTC |= (0x0F);
  PORTD |= (0xF0);

  _delay_ms(100);

  unsigned char dipSwitch = 0;
  dipSwitch |= (PINC & (1 << PINC3)) ? 0 : (1 << 0);
  dipSwitch |= (PINC & (1 << PINC2)) ? 0 : (1 << 1);
  dipSwitch |= (PINC & (1 << PINC1)) ? 0 : (1 << 2);
//  dipSwitch |= (PINC & (1 << PINC0)) ? 0 : (1 << 3);
  dipSwitch &= ~(1 << 3);
  dipSwitch |= (PIND & (1 << PIND7)) ? 0 : (1 << 4);
  dipSwitch |= (PIND & (1 << PIND6)) ? 0 : (1 << 5);
  dipSwitch |= (PIND & (1 << PIND5)) ? 0 : (1 << 6);
  dipSwitch |= (PIND & (1 << PIND4)) ? 0 : (1 << 7);

  switches_t mySwitches;
  mySwitches.treadmill = dipSwitch & 0xF0;
  mySwitches.protocol  = dipSwitch & 0x0F;

  return mySwitches;
}
