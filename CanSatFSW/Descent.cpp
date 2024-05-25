#include "Common.h"
#include "Hardware.h"
#include "States.h"

namespace States
{    
  void Descent(Common::CanSat_States &cansat_states)
  { 
    States::processCommands(1,1,1,1,1);
    Hardware::read_gps();
    Hardware::read_sensors();

    // State -> Landing: if airspeed < 1 m/s and altitude << 2m
    if (Hardware::sensor_data.airspeed < 1 && Hardware::sensor_data.altitude < 2) {
      EE_STATE = 5;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

    // Transmit 1 Hz telemetry
    if (Hardware::CX) {
      String packet = States::build_packet("Descent", cansat_states);
      Serial.println(packet);
      Hardware::write_ground_radio(packet);
    }

    // Deploy parachute
    Hardware::para_servo.write(180);
    cansat_states.PC_DEPLOYED = 'C';

    // TODO: Release heat shield rings
    //       Move landing legs to landing configuration
    cansat_states.HS_RELEASED = 'R';

  }
}