/*
  Indirect Calorimeter: uart.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _uart_h_
#define _uart_h_

#include "main.h"
#include <stdbool.h>

#define MYUBRR_4800 ((F_CPU/(16*4800UL))-1)
#define MYUBRR_9600 ((F_CPU/(16*9600UL))-1)

//=== PUBLIC FUNCTIONS ===//

void uart_init(int);
void uart_sendChar(char);
void uart_sendCstring(char*);
void uart_sendN(char*, int);

//=== FLAGS ===//

extern volatile unsigned char rxByte;
extern volatile bool rxFlag;

#endif
