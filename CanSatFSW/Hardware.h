#pragma once
#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "Common.h"
// #include "States.h"

#include <Wire.h>             // I2C protocol
#include <Adafruit_BMP3XX.h>  // BMP388 
#include <Adafruit_BNO08x.h>  // BNO085 
#include "ms4525do.h"         // MS4525DO (airspeed sensor)
#include "airdata.h"          // converting differential pressure from airspeed sensor to indicated airspeed
#include <Adafruit_GPS.h>     // Ultimate GPS Breakout (GPS)
#include <TeensyThreads.h>    // Multithreading
#include <EEPROM.h>
#include <Servo.h>
#include <SD.h>

// Covers initialization and any functions that utilize the hardware
namespace Hardware
{
  extern bool SIM_ENABLE;                      // SIM ENABLE -> true ; SIM DISABLE -> false
  extern bool SIM_ACTIVATE;                    // SIM ENABLE and SIM ACTIVATE -> true ; SIM DISABLE -> false
  extern int SIM_PRESSURE;

  extern bool CX;                              // CX on -> true ; CX off -> false

  // Variables preceded by "EE_" are values that write/read from EEPROM
  extern float EE_BASE_PRESSURE;               // CAL -> base_pressure ; else -> -1
  extern uint16_t EE_PACKET_COUNT;

  extern String lastCMD;

  const int chipSelect = BUILTIN_SDCARD;
  static File telemetry;

  extern Adafruit_BMP3XX bmp;
  extern Adafruit_BNO08x bno;
  extern Adafruit_GPS gps;
  extern bfs::Ms4525do airspeed;
  extern Servo hs_servo;
  extern Servo pc_servo;

  extern class Camera main_cam;
  extern class Camera bonus_cam;

  extern Common::CanSat_States cansat_states;
  extern Common::Sensor_Data sensor_data;
  extern Common::GPS_Data gps_data;

  // Mutexes for their defined shared resources 
  extern Threads::Mutex general_mtx;           // shared resources: SIM_ACTIVATE, SIM_ENABLE, SIM_PRESSURE, CX, EE_BASE_PRESSURE, EE_PACKET_COUNT, lastCMD 
    // used in:
      // read_sensors()
      // write_ground_radio_loop()
  extern Threads::Mutex sensor_mtx;            // shared resources: bmp, bno, airspeed, sensor_data
    // used in:
      // read_sensors()
      // build_packet()
  extern Threads::Mutex gps_mtx;               // shared resources: gps, gps_data, GPS_SERIAL
    // used in:
      // read_gps_loop()
      // processCommands()
      // build_packet()
  extern Threads::Mutex radio_mtx;             // shared resources: GROUND_XBEE_SERIAL
    // used in:
      // read_ground_radio()
      // write_ground_radio()
  extern Threads::Mutex states_mtx;            // shared resources: cansat_states, EE_STATE
    // used in:
      // loop()
      // deploy_hs_loop()
      // deploy_pc_loop()
      // release_hs_loop()
      // build_packet()
  extern Threads::Mutex servo_mtx;             // shared resources: hs_servo, para_servo
    // used in:
      // deploy_hs_loop()
      // deploy_pc_loop()
      // release_hs_loop()

  void init();

  // Teensy 4.1, BMP388, BNO085, and airspeed (MS4525DO) sensors operation
  void setup_BMP();
  void setup_BNO();
  void setup_airspeed();
  void read_sensors();

  // Ultimate GPS Breakout operation
  void setup_GPS();
  void read_gps_loop();                        // infinite detached loop

  // XBee Pro S2C operation
  void setup_XBee();
  void write_ground_radio(const String &data);
  bool read_ground_radio(String &data);
  void write_ground_radio_loop();             // infinite detached loop (1 Hz intervals)

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

  // Servo operation
  void deploy_hs_loop();                      // finite detached loop
  void deploy_pc_loop();                      // finite detached loop
  void release_hs_loop();                     // finite detached loop

  // helper functions
  void processCommands(const bool enableCX, const bool enableST, const bool enableSIM, const bool enableCAL, const bool enableBCN);
  String build_packet(String state);

}

#endif