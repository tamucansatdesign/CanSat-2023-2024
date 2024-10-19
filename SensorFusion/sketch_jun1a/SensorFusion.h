#include <BasicLinearAlgebra.h>
#include <cmath>

// TODO:
//  - Initial state vector makes a difference (offset)???

// Covers any classes and functions that will fuse hardware data to get more accurate results
namespace SensorFusion
{
  class KalmanFilter 
  {
    public:
      KalmanFilter(float dt);

      void predict(float bno_acceleration_x, float bno_acceleration_y, float bno_acceleration_z);

      void update(float gps_longitude, float gps_latitude, float bmp_altitude, float gps_altitude, float gps_speed, float gps_heading, float airspeed, float tilt_x, float tilt_y);

      BLA::Matrix<6,1> get_state();

      void set_state(float gps_longitude, float gps_latitude, float bmp_altitude, float gps_altitude, float gps_speed, float gps_heading, float airspeed, float tilt_x, float tilt_y);

    private:
      const float dt;                                         // timestep

      static const int state_dim = 6;                         // dimensions of states
      static const int measure_dim = 6;                       // dimensions of measurements
      static const int control_dim = 3;                       // dimensions of controls

      const float sigma_gps_altitude = 1.8;                   // Ultimate GPS Breakout position uncertainty in m (from datasheet)
      const float sigma_bmp_altitude = 0.5;                   // BMP388 altitude uncertainty in m (from datasheet)

      const float sigma_longitude = 1.8;                      // Ultimate GPS Breakout position uncertainty in m (from datasheet)
      const float sigma_latitude = 1.8;                       // Ultimate GPS Breakout position uncertainty in m (from datasheet)
      const float sigma_altitude = 0.481758954815;            // Ultimate GPS Breakout-BMP388 propagated altitude uncertainty in m (from datasheets)
      const float sigma_velocity_x = 0.1;                     // Ultimate GPS Breakout velocity uncertainty in m/s (from datasheet)
      const float sigma_velocity_y = 0.1;                     // Ultimate GPS Breakout velocity uncertainty in m/s (from datasheet)
      const float sigma_velocity_z = 0.1;                     // (ESTIMATED) MS4525DO-BNO085 propagated velocity uncertainty in m/s (from datasheets)

    public:
      BLA::Matrix<state_dim, 1> x_;                           // predicted state vector result from predict step
      BLA::Matrix<state_dim, state_dim> P_;                   // predicted error covariance matrix result from predict step

      BLA::Matrix<state_dim, 1> x;                            // state vector (x)
      BLA::Matrix<state_dim, state_dim> A;                    // constant state transition matrix (A)
      BLA::Matrix<state_dim, control_dim> B;                  // constant control matrix (B)
      BLA::Matrix<control_dim, 1> u;                          // control input vector (u)
      BLA::Matrix<state_dim, state_dim> P;                    // error covariance matrix (P)
      BLA::Matrix<state_dim, state_dim> Q;                    // constant process noise covariance matrix (Q)
      BLA::Matrix<state_dim, measure_dim> K;                  // Kalman gain vector (K)
      BLA::Matrix<measure_dim, state_dim> C;                  // measurement-state mapping vector (C)
      BLA::Matrix<measure_dim, measure_dim> R;                // constant measurement noise covariance matrix (R)
      BLA::Matrix<measure_dim, 1> y;                          // measurement vector (y)
      BLA::Matrix<state_dim, state_dim> I;                    // constant identity matrix (I)
  };
}
