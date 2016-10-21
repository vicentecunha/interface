/*
  Interface: uart.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//========================//
//=== PUBLIC FUNCTIONS ===//
//========================//

// Configure uart baudrate and enable transmitter, receiver and rx interrupt.

void uart_init(int baudrate)
{
  switch (baudrate) {
    case 4800: UBRR0 = (int)MYUBRR_4800; break;
    default:
    case 9600: UBRR0 = (int)MYUBRR_9600; break;
  }
  UCSR0B |= (1 << RXCIE0)|(1 << TXEN0)|(1 << RXEN0);
}

// Wait for transmitter buffer to be ready, then send char.

void uart_sendChar(char c)
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

// Sends a cstring (string terminated by a '\0' character).

void uart_sendCstring(char* buf)
{
  int i = 0;
  while (buf[i] != 0) {
    uart_sendChar(buf[i++]);
  }
}

// Sends n bytes.

void uart_sendN(char* buf, int n)
{
  for (int i = 0; i < n; i++) {
    uart_sendChar(buf[i]);
  }
}
