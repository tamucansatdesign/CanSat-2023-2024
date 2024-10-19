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
        P.Fill(0);

        // constant process noise covariance matrix
        Q = B * ~B * 10.0f * 10.0f;
        // Q.Fill(0);

        // constant measurement-state mapping vector
        C = {1, 0, 0, 0, 0, 0,
             0, 1, 0, 0, 0, 0,
             0, 0, 1, 0, 0, 0,
             0, 0, 0, 1, 0, 0,
             0, 0, 0, 0, 1, 0,
             0, 0, 0, 0, 0, 1};

        // initial measurement noise covariance matrix
        R = {30 * 30, 0,      0,      0,      0,      0,
             0,      30 * 30, 0,      0,      0,      0,
             0,      0,      30 * 30, 0,      0,      0,
             0,      0,      0,      30 * 30, 0,      0,
             0,      0,      0,      0,      30 * 30, 0,
             0,      0,      0,      0,      0,      30 * 30};
        // R.Fill(0);

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

    void KalmanFilter::update(float gps_longitude, float gps_latitude, float bmp_altitude, float gps_altitude, float gps_speed, float gps_angle, float airspeed, float tilt_x, float tilt_y) 
    {
        // calculate Kalman gain
        K = P_ * ~C * BLA::Inverse(C * P_ * ~C + R);

        // calculated measured states from sensor values 
        y = {gps_longitude,
             gps_latitude,
            (bmp_altitude / (sigma_bmp_altitude * sigma_bmp_altitude) + gps_altitude / (sigma_gps_position * sigma_gps_position)) / (1.0 / (sigma_bmp_altitude * sigma_bmp_altitude) + 1.0 / (sigma_gps_position * sigma_gps_position)),
             gps_speed * sin(gps_angle),
             gps_speed * cos(gps_angle),
             airspeed * cos(tilt_x) * cos(tilt_y)};

        // calculate updated state vector
        x = x_ + K * (y - C * x_);

        // calculate updated process covariance matrix
        P = (I - K * C) * P_;
    }

    BLA::Matrix KalmanFilter::get_state() 
    {
        return x;
    }
}
