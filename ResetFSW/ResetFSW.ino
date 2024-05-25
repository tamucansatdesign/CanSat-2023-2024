#include <EEPROM.h>
#include <Servo.h>

// Variable addresses for EEPROM reading and writing
const uint16_t BP_ADDR = 0;  // base (ground-level) pressure used for calibration ; address space: 0-4 (float = 4 bytes)
const uint16_t PC_ADDR = 4;  // packet count                                      ; address space: 4-6 (uint16_t = 2 bytes)
const uint16_t ST_ADDR = 6;  // states                                            ; address space: 6-8 (uint16_t = 2 bytes)

const byte HS_SERVO_PIN = 45;     // Servo 1 PWM pin
const byte PARA_SERVO_PIN = 44;   // Servo 2 PWM pin

// Used to reset EEPROM values to default values, and position any servo motors to their original position
void setup() {
  Serial.begin(115200);
  Serial.println("Serial Connection Established.");
  
  Serial.println("\nResetting EEPROM values to default values");

  float EE_BASE_PRESSURE = -1;             // 4 bytes
  uint16_t EE_PACKET_COUNT = 0;            // 2 bytes
  uint16_t EE_STATE = 0;                   // 2 bytes

  Serial.println("\nWriting values: ");
  EEPROM.put(BP_ADDR, EE_BASE_PRESSURE);
  EEPROM.put(PC_ADDR, EE_PACKET_COUNT);
  EEPROM.put(ST_ADDR, EE_STATE);
  Serial.println("EE_BASE_PRESSURE: " + String(EE_BASE_PRESSURE));
  Serial.println("EE_PACKET_COUNT: " + String(EE_PACKET_COUNT));
  Serial.println("EE_STATE: " + String(EE_STATE));

  EE_BASE_PRESSURE = 1;
  EE_PACKET_COUNT = 1;
  EE_STATE = 1;

  Serial.println("\nReading values: ");
  EEPROM.get(BP_ADDR, EE_BASE_PRESSURE);
  EEPROM.get(PC_ADDR, EE_PACKET_COUNT);
  EEPROM.get(ST_ADDR, EE_STATE);
  Serial.println("EE_BASE_PRESSURE: " + String(EE_BASE_PRESSURE));
  Serial.println("EE_PACKET_COUNT: " + String(EE_PACKET_COUNT));
  Serial.println("EE_STATE: " + String(EE_STATE));

  Serial.println("\nPositioning servo motors to their original position");

  Servo hs_servo;
  Servo para_servo;

  hs_servo.attach(HS_SERVO_PIN);
  para_servo.attach(PARA_SERVO_PIN);

  for (int i = 0; i < 10; i++) {
    hs_servo.write(-180);
  }
  para_servo.write(0);

}

void loop() {}