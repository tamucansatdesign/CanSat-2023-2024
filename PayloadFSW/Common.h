#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>

// TODO: update the serial wire values to match actual configuration
#define GPS_SERIAL Serial7
#define GROUND_XBEE_SERIAL Serial8
#define BMP_WIRE (&Wire1)
#define BNO_WIRE (&Wire2)   // Works
#define AIRSPEED_WIRE (&Wire)

// Header containing all constants and hardware identities
namespace Common
{
  const int SERIAL_BAUD = 115200; // Serial communication rate in bit/s

  const float SEA_LEVEL = 1014.6f; // update this before launch
  const uint16_t TEAM_ID = 2054;
  const unsigned long TELEMETRY_DELAY = 1000; // 1 Hz

  // TODO: update the pin values to match actual configuration
  const byte VOLTAGE_PIN = 38;
  const byte PARA_SERVO_PIN = 22;
  const byte HS_SERVO_PIN = 23;
  const byte CAMERA_PIN = 3;
  const byte AUDIO_BEACON_PIN = 6;

  const byte BNO08X_RESET = -1;

  // TODO: Variable addresses for EEPROM reading and writing
  const uint16_t BA_ADDR = 0;  // base altitude used for calibration
  const uint16_t PC_ADDR = 4;  // packet count 
  const uint16_t ST_ADDR = 6;  // states
  const uint16_t MS_ADDR = 17; // mission time

  struct Payload_States
  {
    char HS_DEPLOYED; // heat shield deployment
    char HS_RELEASED; // heat shield released
    char PC_DEPLOYED; // parachute deployment
  };

  struct GPS_Data
  {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;
    float latitude; 
    float longitude;
    float altitude;
    byte sats;
  };

  struct Sensor_Data
  {
    float vbat; // Teensy voltage sensor
    float altitude; // BMP388 pressure-calculated
    float temperature; // BMP388
    float pressure; // BMP388
    float tilt[2]; // BNO 085: 
    float rotation_speed; // BNO 085: rotation speed
    float airspeed; // pitot tube airspeed sensor
  };
}

#endif