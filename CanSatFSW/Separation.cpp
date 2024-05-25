#include "Common.h"
#include "Hardware.h"
#include "States.h"

namespace States
{    
  void Separation(Common::CanSat_States &cansat_states)
  { 
    States::processCommands(1,1,1,1,1);
    Hardware::read_gps();
    Hardware::read_sensors();

    // State -> Descent: if airspeed >> 0 m/s and altitude < 110m
    if (Hardware::sensor_data.airspeed > 0 && Hardware::sensor_data.altitude < 110) {
      EE_STATE = 4;
      // TODO: EEPROM.put(Common::ST_ADDR, EE_STATE);
    }

    // Transmit 1 Hz telemetry
    if (Hardware::CX) {
      String packet = States::build_packet("Separation", cansat_states);
      Serial.println(packet);
      Hardware::write_ground_radio(packet);
    }

    // Start recording of bonus camera (aft view)
    Hardware::bonus_cam.update_camera(true);

    // TODO: Deploy heat shield
  
    cansat_states.HS_DEPLOYED = 'P';
  }
}