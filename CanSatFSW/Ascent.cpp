#include "States.h"

namespace States
{    
  void Ascent()
  { 
    Hardware::processCommands(1,1,1,1,1);
    Hardware::read_sensors();

    // State -> Separation: if airspeed << 3 m/s and altitude >> 2m
    if ((Hardware::sensor_data.airspeed < 3 || (Hardware::SIM_ENABLE && Hardware::SIM_ENABLE)) && Hardware::sensor_data.altitude > 10) {
      EE_STATE = 3;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

  }
}