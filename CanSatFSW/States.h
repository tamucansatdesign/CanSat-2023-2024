#pragma once
#ifndef __STATES_H__
#define __STATES_H__

// State logic
namespace States
{
  extern uint16_t EE_STATE;

  // ordered by state diagram (2023-2024: State diagram was unofficially revised)
  // void setup() : Bootup -> EE_STATE = 0
    // Initialize hardware
    // Update recovery parameters with EEPROM
    // Sync up RTC with GPS
  void Standby(Common::CanSat_States &cansat_states);         // Standby -> EE_STATE = 1
    // State -> Ascent: if altitude >> 2m
    // Transmit 1 Hz telemetry
    // Start recording of main camera (horizontal view)
  void Ascent();                                            // Ascent -> EE_STATE = 2
    // State -> Separation: if airspeed << 3 m/s and altitude >> 2m
    // Transmit 1 Hz telemetry
  void Separation(Common::CanSat_States &cansat_states);      // Separation -> EE_STATE = 3
    // Reached peak altitude: 
      // State -> Descent: if airspeed >> 0 m/s and altitude < 110m
      // Transmit 1 Hz telemetry
      // Start recording of bonus camera (aft view)
      // Deploy heat shield
  void Descent(Common::CanSat_States &cansat_states);         // Descent -> EE_STATE = 4
    // Altitude 100m:
      // State -> Landing: if airspeed < 1 m/s and altitude << 2m
      // Deploy parachute
      // Release heat shield rings
      // Move landing legs to landing deployment
  void Landing();         // Landing -> EE_STATE = 5
    // Cease telemetry
    // Stop both camera recordings
    // Activate audio beacon
  
  // helper functions
  static void processCommands(const bool enableCX, const bool enableST, const bool enableSIM, const bool enableCAL, const bool enableBCN)
  {
    String received;
    if (Hardware::read_ground_radio(received)) 
    {
      // remove whitespace and commas from received string
      received.trim();
      received.replace(",", "");

      // check if command sent with proper team id
      if (received.startsWith("CMD" + String(Common::TEAM_ID))) 
      {
        String cmd = received.substring(7,received.length());
        Hardware::lastCMD = cmd;
        Serial.println(cmd);

        // CMD,<TEAM_ID>,CX,ON|OFF
        if (enableCX && cmd.startsWith("CX")) 
        {
          if (cmd.startsWith("CXON")) 
          {
            Hardware::CX = true;
          }
          else 
          {
            Hardware::CX = false;
          }
        }

        // CMD,<TEAM_ID>,ST,<UTC_TIME>|GPS
        else if (enableST && cmd.startsWith("ST")) 
        {
          if (cmd.startsWith("STGPS"))
          {
            // Set to GPS time
            setTime(Hardware::gps_data.hour, Hardware::gps_data.minute, Hardware::gps_data.seconds, Hardware::gps_data.day, Hardware::gps_data.month, Hardware::gps_data.year);
          }
          else 
          {
            // Set to utc time
            String utc = cmd.substring(2,cmd.length());

            int hour = utc.substring(0, 2).toInt();
            int minute = utc.substring(3, 5).toInt();
            int seconds = utc.substring(6, 8).toInt();

            setTime(hour, minute, seconds, Hardware::gps_data.day, Hardware::gps_data.month, Hardware::gps_data.year);
          }
        }

        // CMD,<TEAM_ID>,SIM,<MODE>
        else if (enableSIM && cmd.startsWith("SIM")) 
        {
          if (cmd.startsWith("SIMENABLE")) 
          {
            Hardware::SIM_ENABLE = true;
          }
          else if (cmd.startsWith("SIMACTIVATE")) 
          {
            if (Hardware::SIM_ENABLE) 
            {
              Hardware::SIM_ACTIVATE = true;
            }
          }
          else if (cmd.startsWith("SIMDISABLE")) 
          {
            Hardware::SIM_ENABLE = false;
            Hardware::SIM_ACTIVATE = false;
          }
        }

        // CMD,<TEAM_ID>,SIMP,<PRESSURE>
        else if (enableSIM && cmd.startsWith("SIMP")) 
        {
          Hardware::SIM_PRESSURE = cmd.substring(4,cmd.length()).toInt();
        }

        // CMD,<TEAM_ID>,CAL 
        else if (enableCAL && cmd.startsWith("CAL")) 
        {
          Hardware::read_sensors();
          Hardware::EE_BASE_PRESSURE = Hardware::sensor_data.pressure / 100;
          // TODO: EEPROM.put(Common::BP_ADDR, Hardware::EE_BASE_PRESSURE);
        }

        // CMD,<TEAM_ID>,BCN,ON|OFF
        else if (enableBCN && cmd.startsWith("BCN"))
        {
          if (cmd.startsWith("BCNON")) 
          {
            Hardware::buzzer_on();
          }
          else 
          {
            Hardware::buzzer_off();
          }
        }
        
        // else ignore
      }
    }
  }

  static String build_packet(String state, const Common::CanSat_States &cansat_states)
  {
    // <TEAM_ID>, <MISSION_TIME>, <PACKET_COUNT>, <MODE>, <STATE>, <ALTITUDE>,
    // <AIR_SPEED>, <HS_DEPLOYED>, <PC_DEPLOYED>, <TEMPERATURE>, <VOLTAGE>,
    // <PRESSURE>, <GPS_TIME>, <GPS_ALTITUDE>, <GPS_LATITUDE>, <GPS_LONGITUDE>,
    // <GPS_SATS>, <TILT_X>, <TILT_Y>, <ROT_Z>, <CMD_ECHO>
    String packet = String(Common::TEAM_ID) + ",";
    packet += String(hour()) + ":" + String(minute()) + ":" + String(second()) + ",";
    packet += String(Hardware::EE_PACKET_COUNT) + ",";
    if (Hardware::SIM_ACTIVATE && Hardware::SIM_ENABLE)
      packet += "S,";
    else
      packet += "F,";
    packet += state + ",";
    packet += String(Hardware::sensor_data.altitude) + ","; 
    packet += String(Hardware::sensor_data.airspeed) + ",";
    packet += cansat_states.HS_DEPLOYED + ",";
    packet += cansat_states.PC_DEPLOYED + ",";
    packet += String(Hardware::sensor_data.temperature) + ",";
    packet += String(Hardware::sensor_data.vbat) + ",";
    packet += String(Hardware::sensor_data.pressure) + ",";
    packet += String(Hardware::gps_data.hours) + ":" + String(Hardware::gps_data.minutes) + ":" + String(Hardware::gps_data.seconds) + ",";
    packet += String(Hardware::gps_data.altitude) + ",";  
    packet += String(Hardware::gps_data.latitude) + ","; 
    packet += String(Hardware::gps_data.longitude) + ","; 
    packet += String(Hardware::gps_data.sats) + ",";
    packet += String(Hardware::sensor_data.tilt_x) + ",";
    packet += String(Hardware::sensor_data.tilt_y) + ",";    
    packet += String(Hardware::sensor_data.rotation_z) + ",";
    packet += Hardware::lastCMD + "\r\n";

    ++Hardware::EE_PACKET_COUNT;
    // TODO: EEPROM.put(Common::PC_ADDR, Hardware::EE_PACKET_COUNT);

    return packet;
  }

}

#endif  