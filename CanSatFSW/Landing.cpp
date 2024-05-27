#include "States.h"

namespace States
{    
  void Landing()
  {
    Hardware::processCommands(1,1,1,1,1);
    Hardware::read_sensors();

    // Cease telemetry
    Hardware::CX = false;

    // Stop both camera recordings
    Hardware::main_cam.update_camera(false);
    Hardware::bonus_cam.update_camera(false);
    
    // Activate audio beacon
    Hardware::buzzer_on();
    
  }
}