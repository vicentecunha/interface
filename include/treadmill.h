/*
  Interface: treadmill.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _treadmill_h_
#define _treadmill_h_

#include "main.h"
#include "switches.h"

#define CLK (F_CPU/1024)
#define RELOAD_PERIOD(ovfPeriod_s) (0x10000UL - CLK*ovfPeriod_s);
#define RELOAD_FREQ(ovfFreq_Hz) (0x10000UL - CLK/ovfFreq_Hz);

//=== TYPEDEFS ===//

typedef struct treadmill_t {
  float speed_kmph;
  float inclination_pt;
  float targetInclination_pt;
  float maxSpeed_kmph;
  float maxInclination_pt;
  int lubDistance_km;
  int maxEncoderCounts;
} treadmill_t;

//=== PUBLIC FUNCTIONS ===//

treadmill_t treadmill_init(treadmill_e treadmill);
void treadmill_resetInclination();
void treadmill_update(treadmill_t* myTreadmill);

//=== FLAGS ===//

extern volatile int encoderCounts;
extern volatile bool intFlag;
extern volatile int reloadValue;

#endif
