/*
  Interface: serialParser.c
  Author: Vicente Cunha
  Date: September 2016
*/

#include "serialParser.h"
#include "trackmaster.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>

//=========================//
//=== PRIVATE FUNCTIONS ===//
//=========================//

float mphToKmph(float x)
{
  return x*1.60934;
}

float kmphToMph(float x)
{
  return x*0.621371;
}

void inbramed(treadmill_t* myTreadmill, unsigned char c)
{
  float auxFloat;
  static states_e state = IDLE;

  switch (state) {
    default:
    case IDLE:
    if (c == 0xAA) {
      myTreadmill->speed_kmph = 0;
      myTreadmill->targetInclination_pt = 0;
      myTreadmill->enableBelt = false;
    } else if (c == 0xA0) state = PARSING_SPEED;
    break;

    case PARSING_SPEED:
    state = PARSING_INCLINATION;
    auxFloat = c/15.;
    if (auxFloat <= myTreadmill->maxSpeed_kmph && auxFloat >= 0) {
      myTreadmill->speed_kmph = auxFloat;
      myTreadmill->enableBelt = true;
    }
    break;

    case PARSING_INCLINATION:
    state = IDLE;
    auxFloat = c/2.;
    if (auxFloat <= myTreadmill->maxInclination_pt && auxFloat >= 0)
      myTreadmill->targetInclination_pt = auxFloat;
    break;
  }
}

void trackmaster(treadmill_t* myTreadmill, unsigned char c, speedUnit_e speedUnit)
{
  static int i = 0;
  static states_e state = IDLE;
  static char rxbuf[8] = {0}, txbuf[8] = {0};
  static bool txAck = false;
  float auxFloat = 0;

  switch (state) {
    default:
    case IDLE:
    switch (c) {
      case START_CDS_ON:
      uart_sendChar(ACK_START_CDS_ON);
      myTreadmill->enableBelt = true;
      myTreadmill->cds = true;
      if (txAck) uart_sendChar(START_CDS_ON);
      break;

      case START_CDS_OFF:
      uart_sendChar(ACK_START_CDS_OFF);
      myTreadmill->enableBelt = true;
      myTreadmill->cds = false;
      if (txAck) uart_sendChar(START_CDS_OFF);
      break;

      case STOP:
      uart_sendChar(ACK_STOP);
      myTreadmill->enableBelt = false;
      if (txAck) uart_sendChar(STOP);
      break;

      case SET_SPEED:
      state = PARSING_SPEED;
      break;

      case SET_ELEVATION:
      state = PARSING_INCLINATION;
      break;

      case SET_TIME:
      state = PARSING_TIME;
      break;

      case SET_PROTOCOL:
      state = PARSING_PROTOCOL;
      break;

      case SET_STAGE:
      state = PARSING_STAGE;
      break;

      case RESET:
      uart_sendChar(ACK_RESET);
      if (txAck) uart_sendChar(RESET);
      break;

      case SET_WEIGHT:
      state = PARSING_WEIGHT;
      break;

      case AUTO_STOP:
      uart_sendChar(ACK_AUTO_STOP);
      myTreadmill->speed_kmph = 0;
      myTreadmill->targetInclination_pt = 0;
      myTreadmill->enableBelt = false;
      if (txAck) uart_sendChar(AUTO_STOP);
      break;

      case AUTO_COOL_DOWN:
      uart_sendChar(ACK_AUTO_COOL_DOWN);
      myTreadmill->speed_kmph = 0;
      myTreadmill->targetInclination_pt = 0;
      if (txAck) uart_sendChar(AUTO_COOL_DOWN);
      break;

      case TOGGLE_TX_ACK:
      uart_sendChar(ACK_TOGGLE_TX_ACK);
      txAck = !txAck;
      if (txAck) uart_sendChar(TOGGLE_TX_ACK);
      break;

      case GET_BELT_STATUS:
      uart_sendChar(ACK_GET_BELT_STATUS);
      if (!myTreadmill->enableBelt) uart_sendChar(BELT_STOPPED);
      else if (myTreadmill->cds)    uart_sendChar(BELT_STARTED_CDS_ON);
      else                          uart_sendChar(BELT_STARTED_CDS_OFF);
      if (txAck) uart_sendChar(GET_BELT_STATUS);
      break;

      case GET_ACTUAL_SPEED:
      uart_sendChar(ACK_GET_ACTUAL_SPEED);
      auxFloat = myTreadmill->speed_kmph;
      if (speedUnit == MPH) auxFloat = kmphToMph(auxFloat);
      snprintf(txbuf, 8, "%04d", (int)(10*auxFloat));
      uart_sendCstring(txbuf);
      if (txAck) uart_sendChar(GET_ACTUAL_SPEED);
      break;

      case GET_ACTUAL_ELEVATION:
      uart_sendChar(ACK_GET_ACTUAL_ELEVATION);
      snprintf(txbuf, 8, "%04d", (int)(10*(myTreadmill->inclination_pt)));
      uart_sendCstring(txbuf);
      if (txAck) uart_sendChar(GET_ACTUAL_ELEVATION);
      break;

      case GET_SPEED:
      uart_sendChar(ACK_GET_SPEED);
      auxFloat = myTreadmill->speed_kmph;
      if (speedUnit == MPH) auxFloat = kmphToMph(auxFloat);
      snprintf(txbuf, 8, "%04d", (int)(10*auxFloat));
      uart_sendCstring(txbuf);
      if (txAck) uart_sendChar(GET_SPEED);
      break;

      case GET_ELEVATION:
      uart_sendChar(ACK_GET_ELEVATION);
      snprintf(txbuf, 8, "%04d", (int)(10*(myTreadmill->targetInclination_pt)));
      uart_sendCstring(txbuf);
      if (txAck) uart_sendChar(GET_ELEVATION);
      break;

      case GET_TIME:
      uart_sendChar(ACK_GET_TIME);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_TIME);
      break;

      case GET_TOTAL_TIME:
      uart_sendChar(ACK_GET_TOTAL_TIME);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_TOTAL_TIME);
      break;

      case GET_DISTANCE:
      uart_sendChar(ACK_GET_DISTANCE);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_DISTANCE);
      break;

      case GET_PROTOCOL:
      uart_sendChar(ACK_GET_PROTOCOL);
      for (int i = 0; i < 2; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_PROTOCOL);
      break;

      case GET_STAGE:
      uart_sendChar(ACK_GET_STAGE);
      for (int i = 0; i < 2; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_STAGE);
      break;

      case GET_WEIGHT:
      uart_sendChar(ACK_GET_WEIGHT);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_WEIGHT);
      break;

      case GET_CALORIES:
      uart_sendChar(ACK_GET_CALORIES);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_CALORIES);
      break;

      case GET_TOTAL_VO2:
      uart_sendChar(ACK_GET_TOTAL_VO2);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_TOTAL_VO2);
      break;

      case GET_METS:
      uart_sendChar(ACK_GET_METS);
      for (int i = 0; i < 4; i++) uart_sendChar(0);
      if (txAck) uart_sendChar(GET_METS);
      break;

      default:
      uart_sendChar(ILLEGAL_COMMAND);
      if (txAck) uart_sendChar(c);
      break;
    }
    break;

    case PARSING_SPEED:
    rxbuf[i] = c;
    if (++i > 3) {
      state = IDLE;
      i = 0;
      auxFloat = strtoul(rxbuf, NULL, 10)/10.;
      if (speedUnit == MPH) auxFloat = mphToKmph(auxFloat);
      if (auxFloat >= 0 && auxFloat <= myTreadmill->maxSpeed_kmph) {
        myTreadmill->speed_kmph = auxFloat;
        uart_sendChar(ACK_SET_SPEED);
      } else uart_sendChar(DATA_OUT_OF_RANGE);
      if (txAck) {
        uart_sendChar(SET_SPEED);
        for (int i = 0; i < 4; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;

    case PARSING_INCLINATION:
    rxbuf[i] = c;
    if (++i > 3) {
      state = IDLE;
      i = 0;
      auxFloat = strtoul(rxbuf, NULL, 10)/10.;
      if (auxFloat >= 0 && auxFloat <= myTreadmill->maxInclination_pt) {
        myTreadmill->targetInclination_pt = auxFloat;
        uart_sendChar(ACK_SET_ELEVATION);
      } else uart_sendChar(DATA_OUT_OF_RANGE);
      if (txAck) {
        uart_sendChar(SET_ELEVATION);
        for (int i = 0; i < 4; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;

    case PARSING_TIME:
    rxbuf[i] = c;
    if (++i > 3) {
      state = IDLE;
      i = 0;
      uart_sendChar(ACK_SET_TIME);
      if (txAck) {
        uart_sendChar(SET_TIME);
        for (int i = 0; i < 4; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;

    case PARSING_PROTOCOL:
    rxbuf[i] = c;
    if (++i > 1) {
      state = IDLE;
      i = 0;
      uart_sendChar(ACK_SET_PROTOCOL);
      if (txAck) {
        uart_sendChar(SET_PROTOCOL);
        for (int i = 0; i < 2; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;

    case PARSING_STAGE:
    rxbuf[i] = c;
    if (++i > 1) {
      state = IDLE;
      i = 0;
      uart_sendChar(ACK_SET_STAGE);
      if (txAck) {
        uart_sendChar(SET_STAGE);
        for (int i = 0; i < 2; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;

    case PARSING_WEIGHT:
    rxbuf[i] = c;
    if (++i > 3) {
      state = IDLE;
      i = 0;
      uart_sendChar(ACK_SET_WEIGHT);
      if (txAck) {
        uart_sendChar(SET_WEIGHT);
        for (int i = 0; i < 4; i++) uart_sendChar(rxbuf[i]);
      }
    }
    break;
  }
}

//========================//
//=== PUBLIC FUNCTIONS ===//
//========================//

void serialParser_parse(protocol_e protocol, treadmill_t* myTreadmill,
                        unsigned char c)
{
  switch (protocol) {
    case INBRAMED: inbramed(myTreadmill, c); break;
    case TRACKMASTER_KMPH: trackmaster(myTreadmill, c, KMPH); break;
    case TRACKMASTER_MPH: trackmaster(myTreadmill, c, MPH); break;
  }
}
