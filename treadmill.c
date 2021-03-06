/*
  Interface: treadmill.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "treadmill.h"
#include <avr/interrupt.h>
#include <util/delay.h>

//==================//
//=== INTERRUPTS ===//
//==================//

volatile int encoderCounts = 0;
volatile bool intFlag = false;
volatile long reloadValue = 0;

ISR(INT0_vect)
{
  intFlag = true;
  if (PIND & (1 << PIND3)) encoderCounts -= 1;
  else encoderCounts += 1;
}

ISR(INT1_vect)
{
  intFlag = true;
}

ISR(TIMER1_OVF_vect)
{
  PORTC ^= (1 << PORTC4); // toggle speed control pin
  TCNT1 = reloadValue;
}

//=========================//
//=== PRIVATE FUNCTIONS ===//
//=========================//

double countsToPt(int counts, int maxCounts, double maxInclination_pt)
{
  return (counts*maxInclination_pt)/maxCounts;
}

double kmphToHz(double speed_kmph, double maxSpeed_kmph,
                int minFrequency_Hz, int maxFrequency_Hz)
{
  return 2*((maxFrequency_Hz - minFrequency_Hz)*speed_kmph/maxSpeed_kmph + minFrequency_Hz);
}

void lowerInclination()
{
  PORTB &= ~(1 << PORTB0);
  PORTB |=  (1 << PORTB1);
}

void raiseInclination()
{
  PORTB &= ~(1 << PORTB1);
  PORTB |=  (1 << PORTB0);
}

void stabilizeInclination()
{
  PORTB &= ~((1 << PORTB1)|(1 << PORTB0));
}

//========================//
//=== PUBLIC FUNCTIONS ===//
//========================//

treadmill_t treadmill_init(treadmill_e treadmill)
{
  DDRB |= (1 << DDB0)|(1 << DDB1)|(1 << DDB2); // OUTPUTS UP / DOWN / LUB
  PORTB &= ~((1 << PORTB2)|(1 << PORTB1)|(1 << PORTB0));
  PORTB |= (1 << PORTB3); // silicone led pull-up
  DDRC |= (1 << DDC5)|(1 << DDC4); // INVERTER SIGNAL
  PORTC |= (1 << PORTC5);
  PORTD |= (1 << PORTD2)|(1 << PORTD3); // ENCODER PULL-UPS
  EICRA |= (1 << ISC11)|(1 << ISC10)|(1 << ISC01)|(1 << ISC00); // rising edge
  EIMSK |= (1 << INT1)|(1 << INT0); // enable external interrupts
  TIMSK1 |= (1 << TOIE1); // enable timer OVF interrupt
  TCCR1B |= (1 << CS10); // start speed timer, prescaler = 8

  treadmill_t myTreadmill;
  myTreadmill.speed_kmph = 0;
  myTreadmill.inclination_pt = 0;
  myTreadmill.targetInclination_pt = 0;
  myTreadmill.maxInclination_pt = 26;
  myTreadmill.maxEncoderCounts = 4000;
  myTreadmill.lubDistance_km = 25;
  myTreadmill.enableBelt = false;
  myTreadmill.cds = false;
  switch (treadmill) {
    default:
    case MAX_SPEED_16_KMPH: myTreadmill.maxSpeed_kmph = 16; break;
  }
  return myTreadmill;
}

void treadmill_resetInclination()
{
  lowerInclination();
  do {
    intFlag = false;
    _delay_ms(100);
  } while (intFlag);
  encoderCounts = 0;
  stabilizeInclination();
}

void treadmill_update(treadmill_t* myTreadmill)
{
  if (myTreadmill->enableBelt) {
    reloadValue = RELOAD_FREQ(kmphToHz(myTreadmill->speed_kmph, myTreadmill->maxSpeed_kmph, MIN_FREQ_Hz, MAX_FREQ_Hz));
  } else {
    reloadValue = RELOAD_PERIOD(0);
  }

  myTreadmill->inclination_pt = countsToPt(encoderCounts,
    myTreadmill->maxEncoderCounts, myTreadmill->maxInclination_pt);
  const double tolerance = 0.05;
  if (myTreadmill->targetInclination_pt <= 0 && myTreadmill->inclination_pt > tolerance)
    treadmill_resetInclination();
  else if (myTreadmill->inclination_pt > myTreadmill->targetInclination_pt + tolerance)
    lowerInclination();
  else if (myTreadmill->inclination_pt < myTreadmill->targetInclination_pt - tolerance)
    raiseInclination();
  else stabilizeInclination();
}
