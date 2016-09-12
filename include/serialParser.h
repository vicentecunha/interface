/*
  Interface: serialParser.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _serialParser_h_
#define _serialParser_h_

#include "switches.h"
#include "treadmill.h"

//=== TYPEDEFS ===//

typedef enum states_e {
  IDLE,
  PARSING_SPEED,
  PARSING_INCLINATION
} states_e;

typedef struct serialParser_t {
  protocol_e protocol;
  states_e state;
} serialParser_t;

//=== PUBLIC FUNCTIONS ===//

serialParser_t serialParser_init(protocol_e);
void serialParser_parse(serialParser_t*, treadmill_t*, unsigned char);

#endif
