/*
  Indirect Calorimeter: uart.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _uart_h_
#define _uart_h_

#include "main.h"

#define BAUD 9600UL
#define MYUBRR ((F_CPU/(16*BAUD))-1)

//=== PUBLIC FUNCTIONS ===//

void uart_init();
void uart_sendChar(char);
void uart_sendCstring(char*);
void uart_sendN(char*, int);

//=== FLAGS ===//

extern volatile unsigned char rxByte;
extern volatile bool rxFlag;

#endif
