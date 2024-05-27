#pragma once
#ifndef __STATES_H__
#define __STATES_H__

#include "Common.h"
#include "Hardware.h"

// State logic
namespace States
{
  extern uint16_t EE_STATE;

  // ordered by state diagram (2023-2024: State diagram was unofficially revised)
  // void setup()      // Bootup -> EE_STATE = 0
    // Initialize hardware
    // Update recovery parameters with EEPROM
    // Sync up RTC with GPS
  void Standby();      // Standby -> EE_STATE = 1
    // State -> Ascent: if altitude >> 2m
    // Transmit 1 Hz telemetry
    // Start recording of main camera (horizontal view)
  void Ascent();       // Ascent -> EE_STATE = 2
    // State -> Separation: if airspeed << 3 m/s and altitude >> 2m
    // Transmit 1 Hz telemetry
  void Separation();   // Separation -> EE_STATE = 3
    // Reached peak altitude: 
      // State -> Descent: if airspeed >> 0 m/s and altitude < 110m
      // Transmit 1 Hz telemetry
      // Start recording of bonus camera (aft view)
      // Deploy heat shield
  void Descent();      // Descent -> EE_STATE = 4
    // Altitude 100m:
      // State -> Landing: if airspeed < 1 m/s and altitude << 2m
      // Deploy parachute
      // Release heat shield rings
      // Move landing legs to landing deployment
  void Landing();      // Landing -> EE_STATE = 5
    // Cease telemetry
    // Stop both camera recordings
    // Activate audio beacon

}

#endif  