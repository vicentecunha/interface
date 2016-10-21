/*
  Interface: switches.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _switches_h_
#define _switches_h_

//=== TYPEDEFS ===//

typedef enum treadmill_e {
	MAX_SPEED_16_KMPH = 0x00,
	DEBUG             = 0xF0
} treadmill_e;

typedef enum protocol_e {
	INBRAMED         = 0x00,
	TRACKMASTER_KMPH = 0x01,
	TRACKMASTER_MPH  = 0x02
} protocol_e;

typedef struct switches_t {
	treadmill_e treadmill;
	protocol_e protocol;
} switches_t;

//=== PUBLIC FUNCTIONS ===//

switches_t getSwitches();

#endif
