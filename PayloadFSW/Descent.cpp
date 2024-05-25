#include "Common.h"
#include "Hardware.h"
#include "States.h"

namespace States
{    
  void Descent(Common::Payload_States &pay_states)
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
      String packet = States::build_packet("Descent", pay_states);
      Serial.println(packet);
      Hardware::write_ground_radio(packet);
    }

    // Deploy parachute
    Hardware::para_servo.write(180);
    pay_states.PC_DEPLOYED = 'C';

    // TODO: Release heat shield rings
    //       Move landing legs to landing configuration
    pay_states.HS_RELEASED = 'R';

  }
}