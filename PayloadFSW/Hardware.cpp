#include "Hardware.h"

namespace Hardware
{
  bool SIM_ACTIVATE = false;  
  bool SIM_ENABLE = false;
  int SIM_PRESSURE = 0;

  bool CX = false;

  float EE_BASE_ALTITUDE = 0;
  uint16_t EE_PACKET_COUNT = 0;
  String EE_MISSION_TIME = "00:00:00.00";

  int lastCheck = 4;                        // TODO: Why set equal to 4?
  String lastCMD = "None";

  elapsedMillis cameraHold = 0;
  bool cameraRecording = false;
  bool firstCameraCall = true;

  Adafruit_BMP3XX bmp;
  Adafruit_BNO08x bno;
  Adafruit_GPS gps;
  bfs::Ms4525do airspeed;
  Servo para_servo;
  Servo hs_servo;
  
  // Core function of the Boot-up/Initialization states. 
  // Performs set up for all sensors and other components, and does anything
  // that needs to be done before Standby state can be entered.
  void init()
  {
    // Starting I2C connections
    Serial.println("\nOpening I2C Connections...");
    BMP_WIRE->begin();   // BMP388 
    BNO_WIRE->begin();  // BNO085
    AIRSPEED_WIRE->begin();  // Airspeed
    // AIRSPEED_WIRE.setClock(400000);

    // call BMP388 setup function
    setup_BMP();

    // call BNO085 setup
    setup_BNO();

    // call airspeed setup
    setup_airspeed();

    // call GPS setup
    setup_GPS();

  }

  // setup XBee Pro S2C radio
  void setup_XBee() 
  {
    Serial.println("\nInitializing XBee...");

    GROUND_XBEE_SERIAL.begin(Common::SERIAL_BAUD);

    Serial.println("Found XBee successfully");
  }

  // setup BMP388 sensor
  void setup_BMP()
  {
    Serial.println("\nInitializing BMP388...");
    
    // initialize BMP388 object
    Adafruit_BMP3XX bmp;

    // Check if BMP388 is connected
    if (!bmp.begin_I2C(0x77, BMP_WIRE)) { // 0x77 is the assigned I2C address for the BMP388
      Serial.println("Could not find BMP388, check wiring! womp womp.");
      // while (1) { delay(10); } // Halt program if BMP388 is not found
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

  // setup BNO085 sensor
  void setup_BNO()
  {
    Serial.println("\nInitializing BNO085...");
    
    // init BNO object
    Adafruit_BNO08x bno(Common::BNO08X_RESET);

    // Attempt init of bno I2C connection
    if (!bno.begin_I2C(0x4A, BNO_WIRE)) {
      // I2C connection failed, log error and halt
      Serial.println("Could not find BNO085, check wiring! boo womp.");
      // while (1) { delay(10); }
    }
    else {
      Serial.println("Found BNO085 successfully.");
    }
  }

  // setup airspeed sensor (MS4525DO)
  void setup_airspeed() 
  {
    Serial.println("\nInitializing Airspeed Sensor...");

    // Declare an MS4525DO Object
    bfs::Ms4525do airspeed;

    // Setup the I2C address of Ox28, on bus 0, with a -1 to +1 PSI range
    airspeed.Config(AIRSPEED_WIRE, 0x28, 1.0f, -1.0f);
    
    // Starting the communication with the pressure transducer */
    if (!airspeed.Begin()) {
      Serial.println("Error communicating with sensor"); 
      // while(1){ delay(10); };
    }
    else {
      Serial.println("Found Airspeed Sensor successfully.");
    }
  }

  // setup GPS sensor (Ultimate GPS Breakout)
  void setup_GPS()
  {
    Serial.println("\nInitializing GPS...");

    // begin UART connection
    GPS_SERIAL.begin(Common::SERIAL_BAUD);

    // instantiate GPS object
    Adafruit_GPS gps(&GPS_SERIAL);

    Serial.println("Found GPS successfully.");
  }

  // read data from Teensy, BMP, BNO, and Airspeed sensors
  void read_sensors(Common::Sensor_Data &data)
  { 
    // read Teensy: vbat (voltage)
    data.vbat = ((analogRead(Common::VOLTAGE_PIN) / 1023.0) * 4.2) + 0.35;

    // read BMP: altitude, temperature, pressure
    bmp.performReading();
    if (EE_BASE_ALTITUDE != -100) {
        // TODO: should SEA_LEVEL be used when EE_BASE_ALTITUDE is already offsetting value
        data.altitude = bmp.readAltitude(Common::SEA_LEVEL) - EE_BASE_ALTITUDE;
    }
    else {
      data.altitude = bmp.readAltitude(Common::SEA_LEVEL);
    }
    data.temperature = bmp.temperature;
    data.pressure = bmp.pressure;

    // TODO: read BNO: tilt, rotation_speed

    // read Airspeed sensor: airspeed
    airspeed.Read();
    data.airspeed = bfs::Ias_mps(airspeed.pres_pa());
  }

  // read data from GPS
  void read_gps(Common::GPS_Data &data)
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
      lastCheck = gps.milliseconds + millis();
    }

    data.hours = gps.hour;
    data.minutes = gps.minute;
    data.seconds = gps.seconds;
    data.milliseconds = gps.milliseconds;
    data.latitude = gps.latitude;
    data.longitude = gps.longitude;
    data.altitude = gps.altitude;
    data.sats = (byte)(unsigned int)gps.satellites;  // We do this double conversion to avoid signing issues
  }

  // XBee operation
  void write_ground_radio(const String &data) 
  {
    GROUND_XBEE_SERIAL.println(data);
    // telemetry = SD.open("Flight_1043.csv", FILE_WRITE);
    // telemetry.print(data);
    // telemetry.close();
  }

  bool read_ground_radio(String &data)
  {
    if (GROUND_XBEE_SERIAL.available())
    {
      data = GROUND_XBEE_SERIAL.readStringUntil("\r\n");
      return true;
    } 
    else {
      return false;
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
  void update_camera(bool record)
  {
    if (record && !cameraRecording)
    {
      if (firstCameraCall)
      {
        cameraHold = 0;
        firstCameraCall = false;
      }

      start_recording();
    } else if (!record && cameraRecording)
    {
      if (firstCameraCall)
      {
        cameraHold = 0;
        firstCameraCall = false;
      }

      stop_recording();
    }
  }

  void start_recording()
  {
    if (cameraHold < 150)
    {
      digitalWrite(Common::CAMERA_PIN, LOW);
    } else
    {
      digitalWrite(Common::CAMERA_PIN, HIGH);
      cameraRecording = true;
      firstCameraCall = true;
    }
  }

  void stop_recording()
  {
    if (cameraHold < 550)
    {
      digitalWrite(Common::CAMERA_PIN, LOW);
    } else
    {
      digitalWrite(Common::CAMERA_PIN, HIGH);
      cameraRecording = false;
      firstCameraCall = true;
    }
  }

  
}