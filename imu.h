#ifndef LONGHORN_LIBRARY_2024_IMU_H
#define LONGHORN_LIBRARY_2024_IMU_H

#include "eeprom.h"

#ifndef XYZ_STRUCT
#define XYZ_STRUCT
typedef struct xyz {
  float x;
  float y;
  float z;
} xyz;
#endif

static xyz accelData;
static xyz gyroData;

/**
 * Initialize IMU.
 */
void imu_init();

/**
 * Calibrate all 3 axes using gravity as a ground truth.
 */
void imu_calibrate();

/**
 * Get adjusted acceleration XYZ.
 */
void imu_getAccel(xyz *accel);

/**
 * Get rotational velocity XYZ.
 */
void imu_getGyro(xyz *gyro);

/**
 * Called many times per second.
 * Send SPI commands to the IMU and check for responses.
 * Update accelData and gyroData accordingly.
 */
void imu_periodic();

#endif //LONGHORN_LIBRARY_2024_IMU_H
