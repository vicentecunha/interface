/*
  Interface: serialParser.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "serialParser.h"

//=== PRIVATE FUNCTIONS ===//

void inbramed(serialParser_t* mySerialParser, treadmill_t* myTreadmill,
              unsigned char c)
{
  float auxFloat;

  switch (mySerialParser->state) {
    case IDLE:
    if (c == 0xAA) {
      myTreadmill->speed_kmph = 0;
      myTreadmill->targetInclination_pt = 0;
    } else if (c == 0xA0) mySerialParser->state = PARSING_SPEED;
    break;

    case PARSING_SPEED:
    mySerialParser->state = PARSING_INCLINATION;
    auxFloat = c/15.;
    if (auxFloat <= myTreadmill->maxSpeed_kmph && auxFloat >= 0)
      myTreadmill->speed_kmph = auxFloat;
    break;

    case PARSING_INCLINATION:
    mySerialParser->state = IDLE;
    auxFloat = c/2.;
    if (auxFloat <= myTreadmill->maxInclination_pt && auxFloat >= 0)
      myTreadmill->targetInclination_pt = auxFloat;
    break;
  }
}

//=== PUBLIC FUNCTIONS ===//

serialParser_t serialParser_init(protocol_e protocol)
{
  serialParser_t mySerialParser;
  mySerialParser.protocol = protocol;
  mySerialParser.state = IDLE;
  return mySerialParser;
}

void serialParser_parse(serialParser_t* mySerialParser,
                        treadmill_t* myTreadmill, unsigned char c)
{
  switch (mySerialParser->protocol) {
    default: inbramed(mySerialParser, myTreadmill, c);
  }
}
