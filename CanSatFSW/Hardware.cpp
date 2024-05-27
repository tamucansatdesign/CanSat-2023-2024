#include "Hardware.h"
#include "States.h"

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
  Servo pc_servo;
  Servo hs_servo;
  
  Camera main_cam(Common::CAMERA_PIN);
  Camera bonus_cam(Common::BONUS_CAMERA_PIN);

  Common::CanSat_States cansat_states;
  Common::Sensor_Data sensor_data;
  Common::GPS_Data gps_data;

  Threads::Mutex general_mtx;
  Threads::Mutex sensor_mtx;
  Threads::Mutex gps_mtx;
  Threads::Mutex states_mtx;
    
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

  void read_gps_loop() 
  {
    char c = gps.read();
    if (gps.newNMEAreceived()) 
    {
      if (gps.parse(gps.lastNMEA()))
      {
        gps_mtx.lock();
        setTime(gps.hour, gps.minute, gps.seconds, gps.day, gps.month, gps.year);
        gps_data.hours = gps.hour;
        gps_data.minutes = gps.minute;
        gps_data.seconds = gps.seconds;
        gps_data.milliseconds = gps.milliseconds;
        gps_data.latitude = gps.latitude;
        gps_data.longitude = gps.longitude;
        gps_data.altitude = gps.altitude;
        gps_data.sats = (byte)(unsigned int)gps.satellites;  // We do this double conversion to avoid signing issues
        gps_mtx.unlock();
      }
    }
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

  void write_ground_radio_loop() 
  {
    // Transmit 1 Hz telemetry
    while (1) 
    {
      general_mtx.lock();

      if (CX) {
        String packet;

        sensor_mtx.lock();
        gps_mtx.lock();
        states_mtx.lock();

        switch (States::EE_STATE) 
        {
          case 1:
            packet = build_packet("Standby");
            break;
          case 2:
            packet = build_packet("Ascent");
            break;
          case 3:
            packet = build_packet("Separation");
            break;
          case 4:
            packet = build_packet("Descent");
            break;    
          case 5:
            packet = build_packet("Landing");
            break;
          default:
            packet = build_packet("Standby");
            break;
        }

        Serial.println(packet);
        write_ground_radio(packet);

        sensor_mtx.unlock();
        gps_mtx.unlock();
        states_mtx.unlock();
      }

      general_mtx.unlock();

      delay(Common::TELEMETRY_DELAY);
    }
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

  // Servo operation  
  void deploy_hs_loop() 
  {
    servo_mtx.lock()
    for (int loop = 0; loop < 10; loop++)
    {
      hs_servo.write(180);
    }
    servo_mtx.unlock();
    cansat_states.HS_DEPLOYED = 'P';
  }

  void deploy_pc_loop()
  {
    servo_mtx.lock();
    pc_servo.write(180);
    servo_mtx.unlock();
    cansat_states.PC_DEPLOYED = 'C';
  }

  void release_hs_loop() 
  {
    servo_mtx.lock();
    for (int loop = 0; loop < 3; loop++)
    {
      hs_servo.write(180);
    }
    servo_mtx.unlock();
    cansat_states.HS_RELEASED = 'R';
  }

  // helper functions
  void processCommands(const bool enableCX, const bool enableST, const bool enableSIM, const bool enableCAL, const bool enableBCN)
  {
    String received;
    if (read_ground_radio(received)) 
    {
      // remove whitespace and commas from received string
      received.trim();
      received.replace(",", "");

      // check if command sent with proper team id
      if (received.startsWith("CMD" + String(Common::TEAM_ID))) 
      {
        String cmd = received.substring(7,received.length());
        lastCMD = cmd;
        Serial.println(cmd);

        // CMD,<TEAM_ID>,CX,ON|OFF
        if (enableCX && cmd.startsWith("CX")) 
        {
          if (cmd.startsWith("CXON")) 
          {
            CX = true;
          }
          else 
          {
            CX = false;
          }
        }

        // CMD,<TEAM_ID>,ST,<UTC_TIME>|GPS
        else if (enableST && cmd.startsWith("ST")) 
        {
          if (cmd.startsWith("STGPS"))
          {
            // Set to GPS time
            setTime(gps_data.hours, gps_data.minutes, gps_data.seconds, day(), month(), year());
          }
          else 
          {
            // Set to utc time
            String utc = cmd.substring(2,cmd.length());

            int hours = utc.substring(0, 2).toInt();
            int minutes = utc.substring(3, 5).toInt();
            int seconds = utc.substring(6, 8).toInt();

            setTime(hours, minutes, seconds, day(), month(), year());
          }
        }

        // CMD,<TEAM_ID>,SIM,<MODE>
        else if (enableSIM && cmd.startsWith("SIM")) 
        {
          if (cmd.startsWith("SIMENABLE")) 
          {
            SIM_ENABLE = true;
          }
          else if (cmd.startsWith("SIMACTIVATE")) 
          {
            if (SIM_ENABLE) 
            {
              SIM_ACTIVATE = true;
            }
          }
          else if (cmd.startsWith("SIMDISABLE")) 
          {
            SIM_ENABLE = false;
            SIM_ACTIVATE = false;
          }
        }

        // CMD,<TEAM_ID>,SIMP,<PRESSURE>
        else if (enableSIM && cmd.startsWith("SIMP")) 
        {
          SIM_PRESSURE = cmd.substring(4,cmd.length()).toInt();
        }

        // CMD,<TEAM_ID>,CAL 
        else if (enableCAL && cmd.startsWith("CAL")) 
        {
          read_sensors();
          EE_BASE_PRESSURE = sensor_data.pressure / 100;
          // TODO: EEPROM.put(Common::BP_ADDR, EE_BASE_PRESSURE);
        }

        // CMD,<TEAM_ID>,BCN,ON|OFF
        else if (enableBCN && cmd.startsWith("BCN"))
        {
          if (cmd.startsWith("BCNON")) 
          {
            buzzer_on();
          }
          else 
          {
            buzzer_off();
          }
        }
        
        // else ignore
      }
    }
  }

  String build_packet(String state)
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
    packet += state + ",";
    packet += String(sensor_data.altitude) + ","; 
    packet += String(sensor_data.airspeed) + ",";
    packet += cansat_states.HS_DEPLOYED + ",";
    packet += cansat_states.PC_DEPLOYED + ",";
    packet += String(sensor_data.temperature) + ",";
    packet += String(sensor_data.vbat) + ",";
    packet += String(sensor_data.pressure) + ",";
    packet += String(gps_data.hours) + ":" + String(gps_data.minutes) + ":" + String(gps_data.seconds) + ",";
    packet += String(gps_data.altitude) + ",";  
    packet += String(gps_data.latitude) + ","; 
    packet += String(gps_data.longitude) + ","; 
    packet += String(gps_data.sats) + ",";
    packet += String(sensor_data.tilt_x) + ",";
    packet += String(sensor_data.tilt_y) + ",";    
    packet += String(sensor_data.rotation_z) + ",";
    packet += lastCMD + "\r\n";

    ++EE_PACKET_COUNT;
    // TODO: EEPROM.put(Common::PC_ADDR, EE_PACKET_COUNT);

    return packet;
  }

}