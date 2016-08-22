/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "i2c.h"
#include "mpu6050.h"
#include "liquid_crystal.h"
#include "stdio.h"

#define LCD_ADDR 0x3f

void error(void) {
    uint16_t delay = 100;

    while(1){
        palSetPad(GPIOD, GPIOD_LED5);
        chThdSleepMilliseconds(delay);

        palClearPad(GPIOD, GPIOD_LED5);
        chThdSleepMilliseconds(delay);
    }
}

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */

static THD_WORKING_AREA(waThreadBlinker, 128);
static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;
  chRegSetThreadName("ThreadBlinker");

  uint16_t delay = 100;

  while (1) {
    palSetPad(GPIOD, GPIOD_LED4);
    chThdSleepMilliseconds(delay);

    palClearPad(GPIOD, GPIOD_LED4);
    chThdSleepMilliseconds(delay);
  }
}

/*
 * This is thread which getting info from MPU6050 sensor
 */

void delay_ms(uint32_t delay) {
    chThdSleepMilliseconds(delay);
}

int write(uint8_t data) {
    return i2cMasterTransmitTimeout(&I2CD2, LCD_ADDR, &data, sizeof(data), NULL, 0, TIME_INFINITE);
}

static THD_WORKING_AREA(waThreadMPU6050, 2048);
static THD_FUNCTION(ThreadMPU6050, arg) {

  (void)arg;
  chRegSetThreadName("ThreadMPU6050");

  i2c_init();
  mpu6050_init();
  liquid_crystal_t lcd;
  LCD_init(&lcd, delay_ms, write, 4, 4);
  LCD_backlight_on(&lcd);

  uint16_t gyro_x_err = 65200;
  uint16_t gyro_y_err = 120;
  uint16_t gyro_z_err = 65200;

  uint16_t acc_x_err = 62556;
  uint16_t acc_y_err = 8748;
  uint16_t acc_z_err = 13036;

  msg_t ret = 0;
  uint16_t delay = 1000;
  uint8_t MSB = 1;
  uint8_t LSB = 2;
  uint8_t who_am_i = 0;
  char buf[20] = {};

  while(1){
    if(!who_am_i){
      ret = mpu6050_Read(0x75, &who_am_i);
      if(ret != 0) return error();
    }

    // reading the accelerometer data
    ret = mpu6050_Read(MPU6050_RA_ACCEL_XOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_ACCEL_XOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t acc_x = (MSB << 8) | LSB;

    ret = mpu6050_Read(MPU6050_RA_ACCEL_YOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_ACCEL_YOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t acc_y = (MSB << 8) | LSB;

    ret = mpu6050_Read(MPU6050_RA_ACCEL_ZOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_ACCEL_ZOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t acc_z = (MSB << 8) | LSB;

    // reading the gyro data
    ret = mpu6050_Read(MPU6050_RA_GYRO_XOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_GYRO_XOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t gyro_x = (MSB << 8) | LSB;

    ret = mpu6050_Read(MPU6050_RA_GYRO_YOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_GYRO_YOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t gyro_y = (MSB << 8) | LSB;

    ret = mpu6050_Read(MPU6050_RA_GYRO_ZOUT_H, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(MPU6050_RA_GYRO_ZOUT_L, &LSB);
    if(ret != 0) return error();

    uint16_t gyro_z = (MSB << 8) | LSB;

    // reading the temperature
    ret = mpu6050_Read(0x41, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(0x42, &LSB);
    if(ret != 0) return error();

    uint16_t temperature = (MSB << 8) | LSB;

    // Printing the output
    LCD_clear(&lcd);

    sprintf(buf, "Xa=%d", acc_x - acc_x_err);
    LCD_set_cursor(&lcd, 0, 0);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Ya=%d", acc_y - acc_y_err);
    LCD_set_cursor(&lcd, 0, 1);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Za=%d", acc_z - acc_z_err);
    LCD_set_cursor(&lcd, 0, 2);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Xg=%d", gyro_x - gyro_x_err);
    LCD_set_cursor(&lcd, 10, 0);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Yg=%d", gyro_y - gyro_y_err);
    LCD_set_cursor(&lcd, 10, 1);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Zg=%d", gyro_z - gyro_z_err);
    LCD_set_cursor(&lcd, 10, 2);
    LCD_write_str(&lcd, buf);

    sprintf(buf, "Temp=%d", temperature);
    LCD_set_cursor(&lcd, 0, 3);
    LCD_write_str(&lcd, buf);

    chThdSleepMilliseconds(delay);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO, ThreadBlinker, NULL);
  chThdCreateStatic(waThreadMPU6050, sizeof(waThreadMPU6050), NORMALPRIO, ThreadMPU6050, NULL);

  while(1){
    uint16_t delay = 500;
    chThdSleepMilliseconds(delay);
  };

  return 0;
}
