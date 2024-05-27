#include "Common.h"
#include "Hardware.h"
#include "States.h"

// IMPROVEMENTS:
// when processor resets request update from GSC: cansat_states, lastCMD, cameras operating?
      // servo_mtx???
// BMP388 ground-level calibration
// Running average to account for spikes

// TODO:
// Fix BNO085 tilt and rotation angles
// Test EEPROM values 
// Test mission time and set time command 
// Test audio buzzer 

// void setup() {
//   // Start Serial communication, if debugging
//   Serial.begin(Common::SERIAL_BAUD);
//   Serial.println("Serial Connection Established.");

//   // Initialize all components  
//   Hardware::init();

//   // Hardware::main_cam.update_camera(true);
// }

// void loop() {
//   States::processCommands(1,1,1,1,1);

//   Hardware::read_gps();
//   Hardware::read_sensors();

//   if (Hardware::CX) {
//     String packet = States::build_packet("Test", cansat_states);
//     Serial.println(packet);
//     Hardware::write_ground_radio(packet);
//   }

//   delay(Common::TELEMETRY_DELAY);

//   if (Hardware::SIM_ACTIVATE) {
//     // Hardware::main_cam.update_camera(false);
//     Hardware::buzzer_on();
//     Serial.println("buzzer on");
//   }
//   else {
//     Hardware::buzzer_off();
//     Serial.println("buzzer off");
//   }
// }

void setup() {
  Serial.begin(Common::SERIAL_BAUD);
  Serial.println("Serial Connection Established.");

  // Initialize hardware
  Hardware::init();

  // Update recovery parameters with EEPROM
  EEPROM.get(Common::BP_ADDR, Hardware::EE_BASE_PRESSURE);
  EEPROM.get(Common::PC_ADDR, Hardware::EE_PACKET_COUNT);
  EEPROM.get(Common::ST_ADDR, States::EE_STATE);

  // Begin detached infinite threads
  threads.addThread(Hardware::read_gps_loop);
  threads.addThread(Hardware::write_ground_radio_loop);

  // Sync up RTC with GPS
  Hardware::gps_mtx.lock();
  setTime(Hardware::gps_data.hours, Hardware::gps_data.minutes, Hardware::gps_data.seconds, day(), month(), year());
  Hardware::gps_mtx.unlock();
}

void loop() {
  // Loop through each state 
  Hardware::general_mtx.lock();
  Hardware::sensor_mtx.lock();
  Hardware::states_mtx.lock();
  switch (States::EE_STATE)
  { 
    case 1:
      States::Standby();
      break;
    case 2:
      States::Ascent();
      break;
    case 3:
      States::Separation();
      break;
    case 4:
      States::Descent();
      break;    
    case 5:
      States::Landing();
      break;
    default:
      States::Standby();
      break;
  }
  Hardware::general_mtx.unlock();
  Hardware::sensor_mtx.unlock();
  Hardware::states_mtx.unlock();
}