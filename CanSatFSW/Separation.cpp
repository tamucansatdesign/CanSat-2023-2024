#include "States.h"

namespace States
{    
  void Separation()
  { 
    Hardware::processCommands(1,1,1,1,1);
    Hardware::read_sensors();

    // State -> Descent: if airspeed >> 0 m/s and altitude < 110m
    if ((Hardware::sensor_data.airspeed > 0 || (Hardware::SIM_ENABLE && Hardware::SIM_ENABLE))) && Hardware::sensor_data.altitude < 110) {
      EE_STATE = 4;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

    // Start recording of bonus camera (aft view)
    Hardware::bonus_cam.update_camera(true);

    // Deploy heat shield
    threads.addThread(Hardware::deploy_hs_loop);
    
  }
}