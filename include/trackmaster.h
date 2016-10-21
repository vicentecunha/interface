/*
  Interface: trackmaster.h
  Author: Vicente Cunha
  Date: October 2016
*/

//================//
//=== TYPEDEFS ===//
//================//

typedef enum trackmaster_e {

  // INPUT COMMANDS

  START_CDS_ON   = 0xA0, // start belt, communication disconnect stop enabled
  START_CDS_OFF  = 0xA1, // start belt, communication disconnect stop disabled
  STOP           = 0xA2, // stop belt
  SET_SPEED      = 0xA3, // set speed to the next 4 bytes of ASCII data
  SET_ELEVATION  = 0xA4, // set elevation to the next 4 bytes of ASCII data
  SET_TIME       = 0xA5, // set time to the next 5 bytes of ASCII data
  SET_PROTOCOL   = 0xA6, // set protocol to the next 2 bytes of ASCII data
  SET_STAGE      = 0xA7, // set stage to the next 2 bytes of ASCII data
  RESET          = 0xA8, // reset distance, total time, and energy to 0
  SET_WEIGHT     = 0xA9, // set weight to the next 4 bytes of ASCII data
  AUTO_STOP      = 0xAA, // auto stop, sets speed and elevation to minimum and stops belt
  AUTO_COOL_DOWN = 0xAB, // auto cool down, sets speed and elevation to minimum
  TOGGLE_TX_ACK  = 0xAC, // toggle transmit acknowledge data flag

  // INPUT COMMAND ACKNOWLEDGMENT

  ACK_START_CDS_ON   = 0xB0,
  ACK_START_CDS_OFF  = 0xB1,
  ACK_STOP           = 0xB2,
  ACK_SET_SPEED      = 0xB3,
  ACK_SET_ELEVATION  = 0xB4,
  ACK_SET_TIME       = 0xB5,
  ACK_SET_PROTOCOL   = 0xB6,
  ACK_SET_STAGE      = 0xB7,
  ACK_RESET          = 0xB8,
  ACK_SET_WEIGHT     = 0xB9,
  ACK_AUTO_STOP      = 0xBA,
  ACK_AUTO_COOL_DOWN = 0xBB,
  ACK_TOGGLE_TX_ACK  = 0xBC,
  DATA_OUT_OF_RANGE  = 0xBE, // input command data out of range
  ILLEGAL_COMMAND    = 0xBF, // illegal command or command not recognized

  // STATUS REQUEST

  GET_BELT_STATUS      = 0xC0, // transmit belt status
  GET_ACTUAL_SPEED     = 0xC1, // transmit current actual speed_kmph
  GET_ACTUAL_ELEVATION = 0xC2, // transmit current actual elevation
  GET_SPEED            = 0xC3, // transmit current set speed
  GET_ELEVATION        = 0xC4, // transmit current set elevation
  GET_TIME             = 0xC5, // transmit current time
  GET_TOTAL_TIME       = 0xC6, // transmit total time
  GET_DISTANCE         = 0xC7, // transmit current distance
  GET_PROTOCOL         = 0xC8, // transmit current protocol
  GET_STAGE            = 0xC9, // transmit current stage
  GET_WEIGHT           = 0xCA, // transmit current weight
  GET_CALORIES         = 0xCB, // transmit current calories
  GET_TOTAL_VO2        = 0xCC, // transmit current total VO2
  GET_METS             = 0xCD, // transmit current METs

  // STATUS RESPONSE

  ACK_GET_BELT_STATUS      = 0xD0, // acknowledge belt status followed by 1 byte of data
  ACK_GET_ACTUAL_SPEED     = 0xD1, // current belt speed follows as 4 bytes of ASCII data
  ACK_GET_ACTUAL_ELEVATION = 0xD2, // current elevation follows as 4 bytes of ASCII data
  ACK_GET_SPEED            = 0xD3, // current set belt speed follows as 4 bytes of ASCII data
  ACK_GET_ELEVATION        = 0xD4, // current set elevation follows as 4 bytes of ASCII data
  ACK_GET_TIME             = 0xD5, // current lap time follows as 4 bytes of ASCII data
  ACK_GET_TOTAL_TIME       = 0xD6, // current total time follows as 4 bytes of ASCII data
  ACK_GET_DISTANCE         = 0xD7, // current distance follows as 4 bytes of ASCII data
  ACK_GET_PROTOCOL         = 0xD8, // current protocol follows as 2 bytes of ASCII data
  ACK_GET_STAGE            = 0xD9, // current stage follows as 2 bytes of ASCII data
  ACK_GET_WEIGHT           = 0xDA, // current weight follows as 4 bytes of ASCII data
  ACK_GET_CALORIES         = 0xDB, // current calories follows as 4 bytes of ASCII data
  ACK_GET_TOTAL_VO2        = 0xDC, // current total VO2 follows as 4 bytes of ASCII data
  ACK_GET_METS             = 0xDD, // current METs follow as 4 bytes of ASCII data

  // BELT STATUS

  BELT_STOPPED         = 0x31, // belt stopped
  BELT_STARTED_CDS_ON  = 0x32, // belt started communication disconnect stop enabled
  BELT_STARTED_CDS_OFF = 0x33  // belt started communication disconnect stop disabled

} trackmaster_e;

typedef enum speedUnit_e {
  KMPH,
  MPH
} speedUnit_e;
