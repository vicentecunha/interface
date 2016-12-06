/*
  Interface: trackmaster.h
  Author: Vicente Cunha
  Date: October 2016
*/

//================//
//=== TYPEDEFS ===//
//================//

#define START_CDS_ON   0xA0 // start belt, communication disconnect stop enabled
#define START_CDS_OFF  0xA1 // start belt, communication disconnect stop disabled
#define STOP           0xA2 // stop belt
#define SET_SPEED      0xA3 // set speed to the next 4 bytes of ASCII data
#define SET_ELEVATION  0xA4 // set elevation to the next 4 bytes of ASCII data
#define SET_TIME       0xA5 // set time to the next 5 bytes of ASCII data
#define SET_PROTOCOL   0xA6 // set protocol to the next 2 bytes of ASCII data
#define SET_STAGE      0xA7 // set stage to the next 2 bytes of ASCII data
#define RESET          0xA8 // reset distance, total time, and energy to 0
#define SET_WEIGHT     0xA9 // set weight to the next 4 bytes of ASCII data
#define AUTO_STOP      0xAA // auto stop, sets speed and elevation to minimum and stops belt
#define AUTO_COOL_DOWN 0xAB // auto cool down, sets speed and elevation to minimum
#define TOGGLE_TX_ACK  0xAC // toggle transmit acknowledge data flag

  // INPUT COMMAND ACKNOWLEDGMENT

#define ACK_START_CDS_ON   0xB0
#define ACK_START_CDS_OFF  0xB1
#define ACK_STOP           0xB2
#define ACK_SET_SPEED      0xB3
#define ACK_SET_ELEVATION  0xB4
#define ACK_SET_TIME       0xB5
#define ACK_SET_PROTOCOL   0xB6
#define ACK_SET_STAGE      0xB7
#define ACK_RESET          0xB8
#define ACK_SET_WEIGHT     0xB9
#define ACK_AUTO_STOP      0xBA
#define ACK_AUTO_COOL_DOWN 0xBB
#define ACK_TOGGLE_TX_ACK  0xBC
#define DATA_OUT_OF_RANGE  0xBE // input command data out of range
#define ILLEGAL_COMMAND    0xBF // illegal command or command not recognized

  // STATUS REQUEST

#define GET_BELT_STATUS      0xC0 // transmit belt status
#define GET_ACTUAL_SPEED     0xC1 // transmit current actual speed_kmph
#define GET_ACTUAL_ELEVATION 0xC2 // transmit current actual elevation
#define GET_SPEED            0xC3 // transmit current set speed
#define GET_ELEVATION        0xC4 // transmit current set elevation
#define GET_TIME             0xC5 // transmit current time
#define GET_TOTAL_TIME       0xC6 // transmit total time
#define GET_DISTANCE         0xC7 // transmit current distance
#define GET_PROTOCOL         0xC8 // transmit current protocol
#define GET_STAGE            0xC9 // transmit current stage
#define GET_WEIGHT           0xCA // transmit current weight
#define GET_CALORIES         0xCB // transmit current calories
#define GET_TOTAL_VO2        0xCC // transmit current total VO2
#define GET_METS             0xCD // transmit current METs

  // STATUS RESPONSE

#define ACK_GET_BELT_STATUS      0xD0 // acknowledge belt status followed by 1 byte of data
#define ACK_GET_ACTUAL_SPEED     0xD1 // current belt speed follows as 4 bytes of ASCII data
#define ACK_GET_ACTUAL_ELEVATION 0xD2 // current elevation follows as 4 bytes of ASCII data
#define ACK_GET_SPEED            0xD3 // current set belt speed follows as 4 bytes of ASCII data
#define ACK_GET_ELEVATION        0xD4 // current set elevation follows as 4 bytes of ASCII data
#define ACK_GET_TIME             0xD5 // current lap time follows as 4 bytes of ASCII data
#define ACK_GET_TOTAL_TIME       0xD6 // current total time follows as 4 bytes of ASCII data
#define ACK_GET_DISTANCE         0xD7 // current distance follows as 4 bytes of ASCII data
#define ACK_GET_PROTOCOL         0xD8 // current protocol follows as 2 bytes of ASCII data
#define ACK_GET_STAGE            0xD9 // current stage follows as 2 bytes of ASCII data
#define ACK_GET_WEIGHT           0xDA // current weight follows as 4 bytes of ASCII data
#define ACK_GET_CALORIES         0xDB // current calories follows as 4 bytes of ASCII data
#define ACK_GET_TOTAL_VO2        0xDC // current total VO2 follows as 4 bytes of ASCII data
#define ACK_GET_METS             0xDD // current METs follow as 4 bytes of ASCII data

  // BELT STATUS

#define BELT_STOPPED         0x31 // belt stopped
#define BELT_STARTED_CDS_ON  0x32 // belt started communication disconnect stop enabled
#define BELT_STARTED_CDS_OFF 0x33 // belt started communication disconnect stop disabled

typedef enum speedUnit_e {
  KMPH,
  MPH
} speedUnit_e;
