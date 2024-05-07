#pragma once
#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "Common.h"
#include "States.h"

#include <Wire.h>             // I2C protocol
#include <Adafruit_BMP3XX.h>  // BMP388 
#include <Adafruit_BNO08x.h>  // BNO085 
#include "ms4525do.h"         // airspeed sensor 
#include "airdata.h"          // converting differential pressure from airspeed sensor to indicated airspeed
#include <Adafruit_GPS.h>     // GPS
#include <EEPROM.h>
#include <Servo.h>
// #include <SD.h>

// Covers initialization of sensors/radios, and any functions that utilize this hardware
namespace Hardware
{
  // Variables preceded by "EE_" are values that write/read from EEPROM
  extern bool SIM_ACTIVATE;
  extern bool SIM_ENABLE;
  extern int SIM_PRESSURE;

  extern bool CX;                              // CX on = true ; CX off = false

  extern float EE_BASE_ALTITUDE;               // CAL = base_altitude ; else = -100
  extern uint16_t EE_PACKET_COUNT;
  extern String EE_MISSION_TIME;

  extern int lastCheck;
  extern String lastCMD;

  extern elapsedMillis cameraHold;
  extern bool cameraRecording;
  extern bool firstCameraCall;

  // const int chipSelect = BUILTIN_SDCARD;
  // static File telemetry;

  extern Adafruit_BMP3XX bmp;
  extern Adafruit_BNO08x bno;
  extern Adafruit_GPS gps;
  extern bfs::Ms4525do airspeed;
  extern Servo para_servo;
  extern Servo hs_servo;
  
  void init();

  // hardware setup functions
  void setup_XBee();
  void setup_BMP();
  void setup_BNO();
  void setup_airspeed();
  void setup_GPS();

  // TODO: sensors data reading functions
  void read_sensors(Common::Sensor_Data &data);
  void read_gps(Common::GPS_Data &data);
    
  // XBee read/write radio functions
  void write_ground_radio(const String &data);
  bool read_ground_radio(String &data);

  // buzzer functions
  void buzzer_on();
  void buzzer_off();

  // camera functions
  void update_camera(bool record);
  void start_recording();
  void stop_recording();

  static String build_packet(const Common::Payload_States &pay_states, const Common::GPS_Data &gps, const Common::Sensor_Data &sensors)
  {
    // <TEAM_ID>, <MISSION_TIME>, <PACKET_COUNT>, <MODE>, <STATE>, <ALTITUDE>,
    // <AIR_SPEED>, <HS_DEPLOYED>, <PC_DEPLOYED>, <TEMPERATURE>, <VOLTAGE>,
    // <PRESSURE>, <GPS_TIME>, <GPS_ALTITUDE>, <GPS_LATITUDE>, <GPS_LONGITUDE>,
    // <GPS_SATS>, <TILT_X>, <TILT_Y>, <ROT_Z>, <CMD_ECHO>
    String packet = String(Common::TEAM_ID) + ",";
    packet += String(hour()) + ":" + String(minute()) + ":" + String(second()) + ",";
    packet += String(EE_PACKET_COUNT) + ",";
    if (SIM_ACTIVATE && SIM_ENABLE)
      packet += "S,";
    else
      packet += "F,";
    packet += States::EE_STATE + ",";
    packet += String(sensors.altitude) + ","; 
    packet += String(sensors.airspeed) + ",";
    packet += pay_states.HS_DEPLOYED + ",";
    packet += pay_states.PC_DEPLOYED + ",";
    packet += String(sensors.temperature) + ",";
    packet += String(sensors.vbat) + ",";
    packet += String(sensors.pressure) + ",";
    packet += String(gps.hours) + ":" + String(gps.minutes) + ":" + String(gps.seconds) + ",";
    packet += String(gps.altitude) + ",";  
    packet += String(gps.latitude) + ","; 
    packet += String(gps.longitude) + ","; 
    packet += String(gps.sats) + ",";
    packet += String(sensors.tilt[0]) + ",";
    packet += String(sensors.tilt[1]) + ",";    
    packet += String(sensors.rotation_speed) + ",";
    packet += lastCMD + "\r\n";

    return packet;
  }

}

#endif