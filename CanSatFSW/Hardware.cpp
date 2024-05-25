#include "Hardware.h"

namespace Hardware
{
  bool SIM_ACTIVATE = false;  
  bool SIM_ENABLE = false;
  int SIM_PRESSURE = 0;

  bool CX = false;

  float EE_BASE_PRESSURE = -1;
  uint16_t EE_PACKET_COUNT = 0;

  String lastCMD = "None";

  Adafruit_BMP3XX bmp;
  Adafruit_BNO08x bno;
  Adafruit_GPS gps(&GPS_SERIAL);
  bfs::Ms4525do airspeed;
  Camera main_cam(Common::CAMERA_PIN);
  Camera bonus_cam(Common::BONUS_CAMERA_PIN);
  Servo para_servo;
  Servo hs_servo;

  Common::Sensor_Data sensor_data;
  Common::GPS_Data gps_data;
  
  // Core function of the Boot-up states. 
  // Performs set up for all sensors and other components, and does anything
  // that needs to be done before Standby state can be entered.
  void init()
  {
    Serial.println("\nBeginning Hardware Initialization...");

    // Starting I2C connections
    Serial.println("\nInitializing I2C connections...");
    BMP_WIRE->begin();   // BMP388 
    BMP_WIRE->setClock(Common::I2C_CLOCK_SPEED);
    BNO_WIRE->begin();  // BNO085
    BNO_WIRE->setClock(Common::I2C_CLOCK_SPEED);
    AIRSPEED_WIRE->begin();  // Airspeed
    AIRSPEED_WIRE->setClock(Common::I2C_CLOCK_SPEED);
    Serial.println("Found I2C connections successfully.");

    // Connect servo pins
    hs_servo.attach(Common::HS_SERVO_PIN);
    para_servo.attach(Common::PARA_SERVO_PIN);

    // SD card initialization
    SD.begin(chipSelect);

    // Call individual hardware setup functions
    setup_BMP();
    setup_BNO();
    setup_airspeed();
    setup_XBee();
    setup_GPS();

    Serial.println("\nAll Hardware Initialized.\n");
  }

  // Teensy 4.1, BMP388, BNO085, and airspeed (MS4525DO) sensors operation
  void setup_BMP()
  {
    Serial.println("\nInitializing BMP388...");
    
    // Check if BMP388 is connected
    if (!bmp.begin_I2C(0x77, BMP_WIRE)) { // 0x77 is the assigned I2C address for the BMP388
      Serial.println("Could not find BMP388, check wiring! womp womp :(");
      while (1) { delay(10); } // Halt program if BMP388 is not found
    }
    else {
      Serial.println("Found BMP388 successfully.");
    }

    // Misc. BMP388 Initialization - see Adafruit docs for specifics
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  }

  void setup_BNO()
  {
    Serial.println("\nInitializing BNO085...");
    
    // Attempt init of bno I2C connection
    if (!bno.begin_I2C(0x4A, BNO_WIRE)) {
      // I2C connection failed, log error and halt
      Serial.println("Could not find BNO085, check wiring! womp womp :(");
      while (1) { delay(10); }
    }
    else {
      Serial.println("Found BNO085 successfully.");
    }

    // Attempt to setup desired bno reports
    Serial.println("Setting BNO085 desired reports...");
    if (!bno.enableReport(SH2_ARVR_STABILIZED_RV, 5000)) {
      Serial.println("Could not enable stabilized rotation vector report");
    }
    else {
      Serial.println("Set BNO085 desired reports successfully.");
    }
  }

  void setup_airspeed() 
  {
    Serial.println("\nInitializing Airspeed Sensor...");

    // Setup the I2C address of Ox28, on bus 0, with a -1 to +1 PSI range
    airspeed.Config(AIRSPEED_WIRE, 0x28, 1.0f, -1.0f);
    
    // Starting the communication with the pressure transducer */
    if (!airspeed.Begin()) {
      Serial.println("Could not find Airspeed Sensor, check wiring! womp womp :("); 
      while(1){ delay(10); };
    }
    else {
      Serial.println("Found Airspeed Sensor successfully.");
    }
  }

  void read_sensors()   // read data from Teensy, BMP, BNO, and Airspeed sensors
  {  
    // read Teensy: vbat (voltage)
    sensor_data.vbat = ((analogRead(Common::VOLTAGE_PIN) / 1023.0) * 4.2) + 0.35;

    // read BMP: pressure, temperature, altitude
    if (bmp.performReading())
    {
      if (SIM_ENABLE && SIM_ACTIVATE) {
        sensor_data.pressure = SIM_PRESSURE;
        sensor_data.altitude = 44330.0 * (1.0 - pow((SIM_PRESSURE / 100.0) / Common::SEA_LEVEL, 0.1903)); // formula is used directly from BMP388 Arduino library instead of readAltitude() to account for simulated pressure instead of true pressure
      }
      else {
        sensor_data.pressure = bmp.pressure;
        sensor_data.temperature = bmp.temperature;

        if (EE_BASE_PRESSURE != -1) { // if pressure calibrated
            sensor_data.altitude = bmp.readAltitude(EE_BASE_PRESSURE);
        }
        else { // if pressure not calibrated
          sensor_data.altitude = bmp.readAltitude(Common::SEA_LEVEL);
        }
      }
    }

    // TODO: read BNO: tilt, rotation_speed
    sh2_SensorValue_t sensorValue;
    if (bno.getSensorEvent(&sensorValue)) 
    {
      switch (sensorValue.sensorId)
      {
        case SH2_ARVR_STABILIZED_RV:
          // Extract quaternion components
          float qr = sensorValue.un.arvrStabilizedRV.real;
          float qi = sensorValue.un.arvrStabilizedRV.i;
          float qj = sensorValue.un.arvrStabilizedRV.j;
          float qk = sensorValue.un.arvrStabilizedRV.k;

          // Convert quaternion to Euler angles
          float sqr = sq(qr);
          float sqi = sq(qi);
          float sqj = sq(qj);
          float sqk = sq(qk);

          // float yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr)) * RAD_TO_DEG;
          float pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr)) * RAD_TO_DEG;
          float roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr)) * RAD_TO_DEG;

          // Calculate TILT_X and TILT_Y (angles of X and Y axes with respect to Z axis)
          sensor_data.tilt_y = pitch;   // TILT_Y is pitch
          sensor_data.tilt_x = roll;  // TILT_X is roll

          // Calculate ROT_Z (rotation rate around Z axis)
          sensor_data.rotation_z = sensorValue.un.gyroscope.z * RAD_TO_DEG; // Convert from radians/sec to degrees/sec
          break;
      }
    }

    // read Airspeed sensor: airspeed
    if (airspeed.Read()) 
    {
      sensor_data.airspeed = bfs::Ias_mps(airspeed.pres_pa());
    }
  }

  // Ultimate GPS Breakout operation
  void setup_GPS()
  {
    Serial.println("\nInitializing GPS...");

    // Check init of GPS UART connection
    if (!gps.begin(9600)) {
      // UART connection failed, log error and halt
      Serial.println("Could not find GPS, check wiring! womp womp :(");
      while (1) { delay(10); }
    }
    else {
      Serial.println("Found GPS successfully.");
    }

    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // GPS.sendCommand(PGCMD_ANTENNA);

    // while (1) {
    //   while (!gps.newNMEAreceived())
    //   {
    //     char c = gps.read();
    //   }
    //   gps.parse(gps.lastNMEA());

    //   if (gps.fix) {
    //     Serial.println("GPS fixed onto satellites successfully.");
    //     break;
    //   }
    //   Serial.println("Looking for GPS fix onto satellites...");
    //   delay(5000);
    // }
  }

  void read_gps()
  {
    bool newData = false;

    while (!gps.newNMEAreceived())
    {
      char c = gps.read();
    }
    newData = gps.parse(gps.lastNMEA());

    if (newData)
    {
      //Serial.println(String(gps.hour) + ":" + String(gps.minute) + ":" + String(gps.seconds) + "." + String(gps.milliseconds));
      setTime(gps.hour, gps.minute, gps.seconds, gps.day, gps.month, gps.year);
    }

    gps_data.hours = gps.hour;
    gps_data.minutes = gps.minute;
    gps_data.seconds = gps.seconds;
    gps_data.milliseconds = gps.milliseconds;
    gps_data.latitude = gps.latitude;
    gps_data.longitude = gps.longitude;
    gps_data.altitude = gps.altitude;
    gps_data.sats = (byte)(unsigned int)gps.satellites;  // We do this double conversion to avoid signing issues
  }

  // XBee Pro S2C operation
  void setup_XBee() 
  {
    Serial.println("\nInitializing XBee...");

    GROUND_XBEE_SERIAL.begin(Common::SERIAL_BAUD);

    // Check init of XBee UART connection
    if (!GROUND_XBEE_SERIAL) {
      // UART connection failed, log error and halt
      Serial.println("Could not find XBee, check wiring! womp womp :(");
      while (1) { delay(10); }
    }
    else {
      Serial.println("Found XBee successfully.");
    }
  }

  void write_ground_radio(const String &data) 
  {
    GROUND_XBEE_SERIAL.println(data);
    
    telemetry = SD.open("Flight_2054.csv", FILE_WRITE);
    telemetry.println(data);
    telemetry.close();
  }

  bool read_ground_radio(String &data)
  {
    String buffer = "";  // Buffer to hold incoming data

    while (GROUND_XBEE_SERIAL.available())
    {
        char c = GROUND_XBEE_SERIAL.read();
        buffer += c;

        // Check for the end of a message
        if (c == '\n' && buffer.endsWith("\r\n"))
        {
            // Remove the delimiter from the buffer and return the message
            data = buffer.substring(0, buffer.length() - 2);  // Remove \r\n
            return true;
        }
    }

    // No complete message received yet
    return false;
  }

  // Buzzer operation 
  void buzzer_on()
  {
    analogWriteFrequency(Common::AUDIO_BEACON_PIN, 4000);
    analogWrite(Common::AUDIO_BEACON_PIN, 128);
  }

  void buzzer_off()
  {
    analogWriteFrequency(Common::AUDIO_BEACON_PIN, 0);
    analogWrite(Common::AUDIO_BEACON_PIN, 0);
  }

  // Camera operation
  Camera::Camera(const byte camera_pin) : camera_pin(camera_pin), cameraHold(0), cameraRecording(false), firstCameraCall(true) {}

  void Camera::update_camera(bool record)
  {
      if (record && !cameraRecording)
      {
        if (firstCameraCall)
        {
            cameraHold = 0;
            firstCameraCall = false;
        }
        start_recording();
      } 
      else if (!record && cameraRecording)
      {
        if (firstCameraCall)
        {
            cameraHold = 0;
            firstCameraCall = false;
        }
        stop_recording();
      }
  }

  bool Camera::is_recording()
  {
    return cameraRecording;
  }

  void Camera::start_recording()
  {
      if (cameraHold < 150)
      {
        digitalWrite(camera_pin, LOW);
      } 
      else
      {
        digitalWrite(camera_pin, HIGH);
        cameraRecording = true;
        firstCameraCall = true;
      }
  }

  void Camera::stop_recording()
  {
      if (cameraHold < 550)
      {
        digitalWrite(camera_pin, LOW);
      }
      else
      {
        digitalWrite(camera_pin, HIGH);
        cameraRecording = false;
        firstCameraCall = true;
      }
  }

}