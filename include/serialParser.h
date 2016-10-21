/*
  Interface: serialParser.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _serialParser_h_
#define _serialParser_h_

#include "switches.h"
#include "treadmill.h"

//================//
//=== TYPEDEFS ===//
//================//

typedef enum states_e {
  IDLE,
  PARSING_SPEED,
  PARSING_INCLINATION,
  PARSING_TIME,
  PARSING_PROTOCOL,
  PARSING_STAGE,
  PARSING_WEIGHT
} states_e;

//========================//
//=== PUBLIC FUNCTIONS ===//
//========================//

void serialParser_parse(protocol_e, treadmill_t*, unsigned char);

#endif
