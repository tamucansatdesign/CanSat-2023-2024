#pragma once
#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "Common.h"

#include <Wire.h>             // I2C protocol
#include <Adafruit_BMP3XX.h>  // BMP388 
#include <Adafruit_BNO08x.h>  // BNO085 
#include "ms4525do.h"         // airspeed sensor 
#include "airdata.h"          // converting differential pressure from airspeed sensor to indicated airspeed
#include <Adafruit_GPS.h>     // GPS
#include <EEPROM.h>
#include <Servo.h>
#include <SD.h>

// Covers initialization of sensors/radios, and any functions that utilize this hardware
namespace Hardware
{
  extern bool SIM_ENABLE;                      // SIM ENABLE -> true ; SIM DISABLE -> false
  extern bool SIM_ACTIVATE;                    // SIM ENABLE and SIM ACTIVATE -> true ; SIM DISABLE -> false
  extern int SIM_PRESSURE;

  extern bool CX;                              // CX on -> true ; CX off -> false

  // Variables preceded by "EE_" are values that write/read from EEPROM
  extern float EE_BASE_PRESSURE;               // CAL -> base_pressure ; else -> -1
  extern uint16_t EE_PACKET_COUNT;

  extern int lastCheck;
  extern String lastCMD;

  const int chipSelect = BUILTIN_SDCARD;
  static File telemetry;

  extern Adafruit_BMP3XX bmp;
  extern Adafruit_BNO08x bno;
  extern Adafruit_GPS gps;
  extern bfs::Ms4525do airspeed;
  extern class Camera main_cam;
  extern class Camera bonus_cam;
  extern Servo hs_servo;
  extern Servo para_servo;

  extern Common::Sensor_Data sensor_data;
  extern Common::GPS_Data gps_data;
  
  void init();

  // Teensy 4.1, BMP388, BNO085, and airspeed (MS4525DO) sensors operation
  void setup_BMP();
  void setup_BNO();
  void setup_airspeed();
  void read_sensors();

  // Ultimate GPS Breakout operation
  void setup_GPS();
  void read_gps();
    
  // XBee Pro S2C operation
  void setup_XBee();
  void write_ground_radio(const String &data);
  bool read_ground_radio(String &data);

  // Buzzer operation
  void buzzer_on();
  void buzzer_off();

  // Camera operation
  class Camera 
  {
    public: 
      Camera(const byte camera_pin);
      void update_camera(bool record);
      bool is_recording();

    private:
      const byte camera_pin;
      elapsedMillis cameraHold;
      bool cameraRecording;
      bool firstCameraCall;

      void start_recording();
      void stop_recording();
  };

}

#endif