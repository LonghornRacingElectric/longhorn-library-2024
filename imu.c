#include "imu.h"
#include <stdint.h>
#include "main.h"
#include "../../Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_spi.h"
#include <stdbool.h>

static uint8_t pData[5]; //not sure i need this?

extern SPI_HandleTypeDef hspi;

/*private functions =====================================================*/

static void write_register(uint8_t* pData, uint8_t Size) {
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi, pData, Size, HAL_TIMEOUT )
    if(status != HAL_OK)  //idk where HAL_OK is supposed to be defined but guessing it's checking KBSR
        Error_Handler();
}

static void write_register1 (uint8_t addr, uint8_t value) {
    pData[0] = addr;
    pData[1] = value;
    write_register(pData, 2);
}

static void read_register(uint8_t* pData, uint8_t Size, uint8_t addr) {
    pData[0] = addr;
    HAL_SPI_Transmit(&hspi, pData, 1, HAL_TIMEOUT );
    pData[0] | 0x08;
    HAL_StatusTypeDef status = HAL_SPI_Receive(&hspi, pData, Size, HAL_TIMEOUT);
    if(status != HAL_OK)  //idk where HAL_OK is supposed to be defined but guessing it's checking KBSR
        Error_Handler();
}

static void read_register1(uint8_t addr) {
    pData[0] = addr;
    read_register(pData, 1, addr);
}

static void scale (){

}

static void calibrate() {

}
/*public functions =======================================================*/
//ACCELEROMETER 208Hz high performance
#define CTRL1_XL_REG 0b00001010
#define CTRL1_XL_VAL 0b01010000
//GYRO 208Hz high performance, 4000dps
#define CTRL2_G_REG 0b00001011
#define CTRL2_G_VAL 0b01010001
void imu_init() {
    write_register1(CTRL1_XL_REG, CTRL1_XL_VAL);
    write_register1(CTRL2_G_REG, CTRL2_G_VAL);
    calibrate();
}

void imu_calibrate() {
    // TODO implement
}
#define STATUS_REG 0b00011110
bool accel_ready() {
    read_register1(STATUS_REG);
    bool ready = (pData[0] & 0x01);
    return ready;
}

#define OUTX_H_A 0x28
void imu_getAccel(xyz* vec) {
    read_register(pData, 6, OUTX_H_A);
}

bool gyro_ready() {
    read_register1(STATUS_REG);
    bool ready = (pData[0] & 0x02);
    return ready;
}

#define OUTX_H_G 0x22
void imu_getGyro(xyz* vec) {
    read_register(pData, 6, OUTX_H_G);
}
