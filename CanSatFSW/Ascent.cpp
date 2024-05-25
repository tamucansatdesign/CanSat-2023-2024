#include "Common.h"
#include "Hardware.h"
#include "States.h"

namespace States
{    
  void Ascent()
  { 
    States::processCommands(1,1,1,1,1);
    Hardware::read_gps();
    Hardware::read_sensors();

    // State -> Separation: if airspeed << 3 m/s and altitude >> 2m
    if (Hardware::sensor_data.airspeed < 3 && Hardware::sensor_data.altitude > 10) {
      EE_STATE = 3;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }
    
    // Transmit 1 Hz telemetry
    if (Hardware::CX) {
      String packet = States::build_packet("Ascent", pay_states);
      Serial.println(packet);
      Hardware::write_ground_radio(packet);
    }

  }
}