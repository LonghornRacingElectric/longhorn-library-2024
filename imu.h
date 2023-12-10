#ifndef LONGHORN_LIBRARY_2024_IMU_H
#define LONGHORN_LIBRARY_2024_IMU_H

#include "eeprom.h"

typedef struct xyz {
    float x;
    float y;
    float z;
} xyz;

xyz accelData;
xyz gyroData;

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
void imu_getAccel(xyz* vec);

/**
 * Get rotational velocity XYZ.
 */
void imu_getGyro(xyz* vec);

#endif //LONGHORN_LIBRARY_2024_IMU_H
