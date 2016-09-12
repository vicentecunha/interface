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

//=== MAIN ===//

int main()
{
  char output[8];

  switches_t mySwitches = getSwitches();
  treadmill_t myTreadmill = treadmill_init(mySwitches.treadmill);
  sei(); // enable interrupts
  treadmill_resetInclination();
  serialParser_t mySerialParser = serialParser_init(mySwitches.protocol);
  uart_init();

  while (1) {
    if (mySwitches.treadmill != DEBUG) treadmill_update(&myTreadmill);
    if (rxFlag) {
      rxFlag = false;
      serialParser_parse(&mySerialParser, &myTreadmill, rxByte);
      //snprintf(output, 8, "%d\r\n", encoderCounts);
      //uart_sendCstring(output);
    }
  }

  return 0;
}
