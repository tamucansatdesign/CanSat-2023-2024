#include <Arduino.h>
#include "SensorFusion.h"

float step = 1;

// Initialize Kalman filter with a time step
SensorFusion::KalmanFilter kalman(step);

float targetAccX = 0;
float targetAccY = 0;
float targetAccZ = 0;

float currentAccX = 0;
float currentAccY = 0;
float currentAccZ = 0;

float trueVecX = 0;
float trueVecY = 0;
float trueVecZ = 0;

float trueState[6];

float true_longitude = 45;
float true_latitude = 45;
float true_altitude = 70;

float gps_longitude = 0;
float gps_latitude = 0;
float bmp_altitude = 0; // Starting altitude
float gps_altitude = 0; // Starting altitude
float gps_speed = 0.0;
float gps_bearing = 0.0;
float airspeed = 0.0;
float tilt_x = 0.0;
float tilt_y = 0.0;

unsigned long previousMillis = 0;  // Stores the last time the if statement was executed
long interval = 10000;

String command = "none";

void updateTargetAcc() {

  if (command == "stay") {
    targetAccX = 0;
    targetAccY = 0;
    targetAccZ = 0;
    trueVecX = 0;
    trueVecY = 0;
    trueVecZ = 0;
  }

  if (command == "east") {
    targetAccX = random(2.0, 10.0);
  }
  else if (command == "west") {
    targetAccX = random(-10.0, 2.0);
  }

  if (command == "north") {
    targetAccY = random(2.0, 10.0);
  }
  else if (command == "south") {
    targetAccY = random(-10.0, 2.0);
  }

  if (command == "up") {
    targetAccZ = random(2.0, 10.0);
  }
  else if (command == "down") {
    targetAccZ = random(-10.0, 2.0);
  }

  // targetAccX = random(-100, 100) / 10.0;
  // targetAccY = random(-100, 100) / 10.0;
  // targetAccZ = random(-100, 100) / 10.0;
}

void updateCurrentAcc() {
  if (currentAccX < targetAccX) {
    currentAccX += random(1.0, 5.0) / 10.0;
  }
  else {
    currentAccX -= random(1.0, 5.0) / 10.0;
  }

  if (currentAccY < targetAccY) {
    currentAccY += random(1.0, 5.0) / 10.0;
  }
  else {
    currentAccY -= random(1.0, 5.0) / 10.0;
  }

  if (currentAccZ < targetAccZ) {
    currentAccZ += random(1.0, 5.0) / 10.0;
  }
  else {
    currentAccZ -= random(1.0, 5.0) / 10.0;
  }
}

void updateTrue() {
  true_longitude += trueVecX * step + 0.5 * currentAccX * step * step;
  true_latitude += trueVecY * step + 0.5 * currentAccY * step * step;
  true_altitude += trueVecZ * step + 0.5 * currentAccZ * step * step;

  trueVecX += currentAccX * step;
  trueVecY += currentAccY * step;
  trueVecZ += currentAccZ * step;

  Serial.print(true_longitude);
  Serial.print(" "); Serial.print(true_latitude);
  Serial.print(" "); Serial.print(true_altitude);
  Serial.print(" "); Serial.print(trueVecX);
  Serial.print(" "); Serial.print(trueVecY);
  Serial.print(" "); Serial.println(trueVecZ);

  trueState[0] = true_longitude;
  trueState[1] = true_latitude;
  trueState[2] = true_altitude;
  trueState[3] = trueVecX;
  trueState[4] = trueVecY;
  trueState[5] = trueVecZ;
}

void getNoisySensorValues() {
  gps_longitude = true_longitude + random(-2, 2);
  gps_latitude = true_latitude + random(-2, 2);
  gps_altitude = true_altitude + random(-2, 2);
  if (random(0, 1) < 0.95) {
    bmp_altitude = true_altitude + random(10, 30);
  }
  bmp_altitude = true_altitude + random(-2, 2);
  gps_speed = sqrt(trueVecX * trueVecX + trueVecY * trueVecY + trueVecZ * trueVecZ) + random(-0.1, 0.1);
  if (currentAccY != 0) {
    gps_bearing = tan(currentAccX / currentAccY) + random(-0.1, 0.1);
  }
  else {
    gps_bearing = 90 + random(-0.1, 0.1);
  }
  airspeed = trueVecZ + random(-1, 1);
  tilt_x = random(-10, 10);
  tilt_y = random(-10, 10);

  if (command == "spike") {
    int num = random(1,5);
    switch (num) {
      case 1:
        gps_longitude = random(10000, 12000);
        break;
      case 2:
        gps_latitude = random(10000, 12000);
        break;
      case 3:
        gps_altitude = random(10000, 12000);
        break;
      case 4:
        bmp_altitude = random(10000, 12000);
        break;
    //   case 5:
    //     gps_speed = random(100, 120);
    //     break;
    //   case 6:
    //     gps_bearing = random(90, 180);
    //     break;
    //   case 7:
    //     airspeed = random(100, 120);
    //     break;
    //   case 8:
    //     tilt_x = random(90, 180);
    //     break;
    //   case 9:
    //     tilt_y = random(90, 180);
    //     break;
    }
    command = "none";
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

bool kalman_set = false;

void loop() {
  unsigned long currentTime = millis();

  if (Serial.available()) {
    command = Serial.readStringUntil("\n");
    Serial.println(command + "\n");
    delay(1000);
  }

  if (currentTime - previousMillis >= interval) {
    updateTargetAcc();
    interval = random(2, 10);
    Serial.print("Interval: "); Serial.println(interval);
  }

  if (command != "stop") {
    updateCurrentAcc();
    updateTrue();
    getNoisySensorValues();
  }

  if (!kalman_set) 
  {
    kalman.set_state(gps_longitude, gps_latitude, bmp_altitude, gps_altitude, gps_speed, gps_bearing, airspeed, tilt_x, tilt_y);
    kalman_set = true;
  }

  kalman.predict(currentAccX, currentAccY, currentAccZ);

  kalman.update(gps_longitude, gps_latitude, bmp_altitude, gps_altitude, gps_speed, gps_bearing, airspeed, tilt_x, tilt_y);

  // Serial.println("Sensor Values:");
  // Serial.print("AccX: "); Serial.println(currentAccX);
  // Serial.print("AccY: "); Serial.println(currentAccY);
  // Serial.print("AccZ: "); Serial.println(currentAccZ);
  // Serial.print("GPS Lon: "); Serial.println(gps_longitude);
  // Serial.print("GPS Lat: "); Serial.println(gps_latitude);
  // Serial.print("BMP Alt: "); Serial.println(bmp_altitude);
  // Serial.print("GPS Alt: "); Serial.println(gps_altitude);
  // Serial.print("GPS Speed: "); Serial.println(gps_speed);
  // Serial.print("GPS Angle: "); Serial.println(gps_bearing);
  // Serial.print("Airspeed: "); Serial.println(airspeed);
  // Serial.print("Tilt X: "); Serial.println(tilt_x);
  // Serial.print("Tilt Y: "); Serial.println(tilt_y);

  // // Print the updated state vector
  // Serial.println("Updated State Vector:");
  // for (int i = 0; i < 6; ++i) {
  //   Serial.print(kalman.get_state()(i));
  //   Serial.print(" ");
  // }
  // Serial.println("\nState Vector Residual:");
  // for (int i = 0; i < 6; ++i) {
  //   Serial.print(abs(kalman.get_state()(i) - trueState[i]));
  //   Serial.print(" ");
  // }
  // Serial.println();
  // Serial.println();
  Serial.println("x = ");
  for (int i = 0; i < 6; i++) {
    Serial.print(kalman.x(i)); Serial.print(" ");
  }
  Serial.println();

  Serial.println("P = ");
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      Serial.print(kalman.P(i,j)); Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("K = ");
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      Serial.print(kalman.K(i,j)); Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println();

  delay(step * 1000); // Wait before next iteration
}
