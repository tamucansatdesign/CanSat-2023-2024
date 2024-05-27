#include "States.h"

namespace States
{  
  uint16_t EE_STATE = 0;
  
  void Standby()
  { 
    Hardware::processCommands(1,1,1,1,1);
    Hardware::read_sensors();

    // State -> Ascent: if altitude >> 2m
    if (Hardware::sensor_data.altitude > 2) {
      EE_STATE = 2;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

    // Start recording of main camera (horizontal view)
    Hardware::main_cam.update_camera(true);
    
    // Initialize payload states
    Hardware::cansat_states.HS_DEPLOYED = 'N';
    Hardware::cansat_states.HS_RELEASED = 'N';
    Hardware::cansat_states.PC_DEPLOYED = 'N';
    
  }
}