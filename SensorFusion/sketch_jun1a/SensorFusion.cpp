#include "SensorFusion.h"

namespace SensorFusion 
{
  KalmanFilter::KalmanFilter(float dt) : dt(dt)
  {
    // temporary state vector [longitude latitude altitude vx vy vz]
    x.Fill(0);

    // constant state transition matrix
    A = {1, 0, 0, dt, 0,  0,
         0, 1, 0, 0,  dt, 0,
         0, 0, 1, 0,  0,  dt,
         0, 0, 0, 1,  0,  0,
         0, 0, 0, 0,  1,  0,
         0, 0, 0, 0,  0,  1};

    // constant control matrix
    float val1 = 0.5 * dt * dt;
    B = {val1, 0,    0,
         0,    val1, 0,
         0,    0,    val1,
         dt,   0,    0,
         0,    dt,   0,
         0,    0,    dt};

    // initial error covariance matrix
    // P = {0.02, 0,    0, 0,    0,    0,
    //      0,    0.02, 0, 0,    0,    0,
    //      0,    0,    0, 0,    0,    0,
    //      0,    0,    0, 0.01, 0,    0,
    //      0,    0,    0, 0,    0.01, 0,
    //      0,    0,    0, 0,    0,    0.01};
    P = {1, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0,
         0, 0, 0, 1, 0, 0,
         0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1};

    // constant process noise covariance matrix
    Q = B * ~B * 50.0f * 50.0f;
    // Q.Fill(0);

    // constant measurement-state mapping vector
    C = {1, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0,
         0, 0, 0, 1, 0, 0,
         0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1};

    // initial measurement noise covariance matrix
    R = {sigma_longitude * sigma_longitude, 0, 0, 0, 0, 0,
         0, sigma_latitude * sigma_latitude, 0, 0, 0, 0,
         0, 0, sigma_altitude * sigma_altitude, 0, 0, 0,
         0, 0, 0, sigma_velocity_x * sigma_velocity_x , 0, 0,
         0, 0, 0, 0, sigma_velocity_y * sigma_velocity_y, 0,
         0, 0, 0, 0, 0, sigma_velocity_z * sigma_velocity_z};

    // constant identity matrix
    I = {1, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0,
         0, 0, 0, 1, 0, 0,
         0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1};
  }

  void KalmanFilter::predict(float bno_acceleration_x, float bno_acceleration_y, float bno_acceleration_z)
  {
    // calculate control inputs from sensor values
    u = {
        bno_acceleration_x,
        bno_acceleration_y,
        bno_acceleration_z
    };

    // calculate predicted state vector result
    x_ = A * x + B * u;

    // calculate predicted error covariance matrix
    P_ = A * P * ~A + Q;
  }

  void KalmanFilter::update(float gps_longitude, float gps_latitude, float bmp_altitude, float gps_altitude, float gps_speed, float gps_bearing, float airspeed, float tilt_x, float tilt_y) 
  {
    // calculate Kalman gain
    K = P_ * ~C * BLA::Inverse(C * P_ * ~C + R);

    // calculated measured states from sensor values
    /*
    y = [longitude,
         latitude,
         uncertainty-weighted altitude,
         velocity in x-direction,
         velocity in y-direction,
         velocity in z-direction]
    */ 
    y = {gps_longitude,
         gps_latitude,
         (bmp_altitude / (sigma_bmp_altitude * sigma_bmp_altitude) + gps_altitude / (sigma_gps_altitude * sigma_gps_altitude)) / (1.0 / (sigma_bmp_altitude * sigma_bmp_altitude) + 1.0 / (sigma_gps_altitude * sigma_gps_altitude)),
         gps_speed * sin(gps_bearing),
         gps_speed * cos(gps_bearing),
         airspeed * cos(tilt_x) * cos(tilt_y)};

    // calculate updated state vector
    x = x_ + K * (y - C * x_);

    // calculate updated process covariance matrix
    P = (I - K * C) * P_;
  }

  BLA::Matrix<6,1> KalmanFilter::get_state() 
  {
    return x;
  }

  void KalmanFilter::set_state(float gps_longitude, float gps_latitude, float bmp_altitude, float gps_altitude, float gps_speed, float gps_bearing, float airspeed, float tilt_x, float tilt_y) 
  {
    // calculated state to set from sensor values 
    x = {gps_longitude,
         gps_latitude,
         (bmp_altitude / (sigma_bmp_altitude * sigma_bmp_altitude) + gps_altitude / (sigma_gps_altitude * sigma_gps_altitude)) / (1.0 / (sigma_bmp_altitude * sigma_bmp_altitude) + 1.0 / (sigma_gps_altitude * sigma_gps_altitude)), /* uncertainty-weighted average */
         gps_speed * sin(gps_bearing),
         gps_speed * cos(gps_bearing),
         airspeed * cos(tilt_x) * cos(tilt_y)};
  }

}
