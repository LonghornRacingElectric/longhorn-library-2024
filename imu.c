#include "imu.h"
#include <stdint.h>
#include "main.h"
#include "spi.h"
#include <stdbool.h>

static uint8_t data[6];

static SPI_HandleTypeDef *hspi = &hspi2;

/*private functions =====================================================*/

static void imu_writeregister1 (uint8_t addr, uint8_t value) {
    HAL_GPIO_WritePin(SPI_CS_IMU_GPIO_Port, SPI_CS_IMU_Pin, 0);
    data[0] = addr;
    data[1] = value;
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, data, 2, HAL_TIMEOUT );
    if(status != HAL_OK)  //idk where HAL_OK is supposed to be defined but guessing it's checking KBSR
        Error_Handler();
    HAL_GPIO_WritePin(SPI_CS_IMU_GPIO_Port, SPI_CS_IMU_Pin, 1);
}

static void imu_readregister(uint8_t size, uint8_t addr) {
    HAL_GPIO_WritePin(SPI_CS_IMU_GPIO_Port, SPI_CS_IMU_Pin, 0);
    data[0] = addr | 0x80;
    HAL_SPI_Transmit(hspi, data, 1, HAL_TIMEOUT );
    HAL_StatusTypeDef status = HAL_SPI_Receive(hspi, data, size, HAL_TIMEOUT);
    if(status != HAL_OK)  //idk where HAL_OK is supposed to be defined but guessing it's checking KBSR
        Error_Handler();
    HAL_GPIO_WritePin(SPI_CS_IMU_GPIO_Port, SPI_CS_IMU_Pin, 1);
}

static void imu_readregister1(uint8_t addr) {
    data[0] = addr;
    imu_readregister(1, addr);
}

static void imu_scale (){

}

/*public functions =======================================================*/
//ACCELEROMETER 208Hz high performance
#define CTRL1_XL_REG 0b00001010
#define CTRL1_XL_VAL 0b01010000
//GYRO 208Hz high performance, 4000dps
#define CTRL2_G_REG 0b00001011
#define CTRL2_G_VAL 0b01010001
void imu_init() {
    imu_writeregister1(CTRL1_XL_REG, CTRL1_XL_VAL);
    imu_writeregister1(CTRL2_G_REG, CTRL2_G_VAL);
    imu_calibrate();
}

void imu_calibrate() {
    // TODO implement
}
#define STATUS_REG 0b00011110
bool accel_ready() {
    imu_readregister1(STATUS_REG);
    bool ready = (data[0] & 0x01);
    return ready;
}

#define OUTX_H_A 0x28
#define GYRO_LSB 0.13734f
void imu_getAccel(xyz* vec) {
    imu_readregister(6, OUTX_H_A);
    int16_t accelX = data[0] | (data[1] << 8);
    vec->x = accelX * GYRO_LSB;
    int16_t accelY = data[2] | (data[3] << 8);
    vec->y = accelY * GYRO_LSB;
    int16_t accelZ = data[4] | (data[5] << 8);
    vec->z = accelZ * GYRO_LSB;

}

bool gyro_ready() {
    imu_readregister1(STATUS_REG);
    bool ready = (data[0] & 0x02);
    return ready;
}

#define OUTX_H_G 0x22
#define ACCEL_LSB 0.00478728f
void imu_getGyro(xyz* vec) {
    imu_readregister(6, OUTX_H_G);
    int16_t buff_gyroX = data[0] + (data[1] << 8);
    vec->x = buff_gyroX * ACCEL_LSB;
    int16_t buff_gyroY = data[2] + (data[3] << 8);
    vec->y = buff_gyroY * ACCEL_LSB;
    int16_t buff_gyroZ = data[4] + (data[5] << 8);
    vec->z = buff_gyroZ * ACCEL_LSB;
    //gyroData?
}
