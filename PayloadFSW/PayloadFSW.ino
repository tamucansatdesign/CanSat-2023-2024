#include "Common.h"
#include "Hardware.h"
#include "States.h"

Common::Payload_States pay_states;
Common::GPS_Data gps_data;
Common::Sensor_Data sensor_data;
uint16_t States::EE_STATE = 0;

void setup() {
  // Start Serial communication, if debugging
  Serial.begin(115200);
  Serial.println("Serial Connection Established");

  // Initialize all components 
  Serial.println("Beginning Hardware Initialization...");
  Hardware::init();
  Serial.println("\nAll Hardware Initialized.\n");

  // Get EEPROM values
  // EEPROM.get(Common::BA_ADDR, Hardware::EE_BASE_ALTITUDE);  
  // EEPROM.get(Common::PC_ADDR, Hardware::EE_PACKET_COUNT); 
  // EEPROM.get(Common::ST_ADDR, States::EE_STATE);   
  // EEPROM.get(Common::MS_ADDR, Hardware::EE_MISSION_TIME);
  GROUND_XBEE_SERIAL.begin(115200);
}

void loop() {
  // Hardware::read_gps(gps_data);
  // Hardware::read_sensors(sensor_data);
  // Serial.println(Hardware::build_packet(pay_states, gps_data, sensor_data));
  // Serial.println("Loop");
  delay(1000);
}

// void setup() {
//   // // Start Serial communication, if debugging
//   // Serial.begin(115200);
//   // while (!Serial); // wait for serial connection
//   // Serial.println("Serial Connection Established");

//   // Initialize all components 
//   Serial.println("Beginning Hardware Initialization...");
//   Hardware::init();
//   Serial.println("All Hardware Initialized.");

//   // Get EEPROM values
//   EEPROM.get(Common::BA_ADDR, Hardware::EE_BASE_ALTITUDE);  
//   EEPROM.get(Common::PC_ADDR, Hardware::EE_PACKET_COUNT); 
//   EEPROM.get(Common::ST_ADDR, States::EE_STATE);   
//   EEPROM.get(Common::MS_ADDR, Hardware::EE_MISSION_TIME);
// }

// void loop() {
//   // put main code here to run repeatedly
//   switch (States::EE_STATE)
//   {
//     case 1:
//       States::Initialization(pay_states);
//       break;
//     case 2:
//       States::Standby(pay_states);
//       break;
//     case 3:
//       States::Deployment(pay_states);
//       break;
//     case 4:
//       States::Landing(pay_states);
//       break;
//     default:
//       States::Initialization(pay_states);
//       break;
//   }
//   delay(Common::TELEMETRY_DELAY);
// }
