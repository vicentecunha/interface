/*
  Interface: treadmill.h
  Author: Vicente Cunha
  Date: September 2016
*/

#ifndef _treadmill_h_
#define _treadmill_h_

#include "main.h"
#include "switches.h"
#include <stdbool.h>

#define CLK (F_CPU)
#define RELOAD_PERIOD(ovfPeriod_s) (0x10000UL - CLK*ovfPeriod_s);
#define RELOAD_FREQ(ovfFreq_Hz) (0x10000UL - CLK/ovfFreq_Hz);
#define MIN_FREQ_Hz 200
#define MAX_FREQ_Hz 3000

//================//
//=== TYPEDEFS ===//
//================//

typedef struct treadmill_t {
  double speed_kmph;
  double inclination_pt;
  double targetInclination_pt;
  double maxSpeed_kmph;
  double maxInclination_pt;
  int maxEncoderCounts;
  int lubDistance_km;
  bool enableBelt;
  bool cds;
} treadmill_t;

//========================//
//=== PUBLIC FUNCTIONS ===//
//========================//

treadmill_t treadmill_init(treadmill_e treadmill);
void treadmill_resetInclination();
void treadmill_update(treadmill_t* myTreadmill);

//=============//
//=== FLAGS ===//
//=============//

extern volatile int encoderCounts;
extern volatile bool intFlag;
extern volatile long reloadValue;

#endif
