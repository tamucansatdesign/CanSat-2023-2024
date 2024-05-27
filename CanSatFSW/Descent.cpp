#include "States.h"

namespace States
{    
  void Descent()
  { 
    Hardware::processCommands(1,1,1,1,1);
    Hardware::read_sensors();

    // State -> Landing: if airspeed < 1 m/s and altitude << 2m
    if ((Hardware::sensor_data.airspeed < 1 || (Hardware::SIM_ENABLE && Hardware::SIM_ENABLE)) && Hardware::sensor_data.altitude < 2) {
      EE_STATE = 5;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

    // Deploy parachute
    threads.addThread(Hardware::deploy_pc_loop);

    // Release heat shield rings
    // Move landing legs to landing configuration
    threads.addThread(Hardware::release_hs_loop);
    
  }
}