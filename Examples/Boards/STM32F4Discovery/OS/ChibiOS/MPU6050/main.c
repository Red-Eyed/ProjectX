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

void error(void){
  uint16_t delay = 100;

  while (true) {
    palSetPad(GPIOD, GPIOD_LED3);
    palSetPad(GPIOD, GPIOD_LED4);
    palSetPad(GPIOD, GPIOD_LED5);
    palSetPad(GPIOD, GPIOD_LED6);

    chThdSleepMilliseconds(delay);

    palClearPad(GPIOD, GPIOD_LED3);
    palClearPad(GPIOD, GPIOD_LED4);
    palClearPad(GPIOD, GPIOD_LED5);
    palClearPad(GPIOD, GPIOD_LED6);

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

  uint16_t delay = 500;

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

static THD_WORKING_AREA(waThreadMPU6050, 128);
static THD_FUNCTION(ThreadMPU6050, arg) {

  (void)arg;
  chRegSetThreadName("ThreadMPU6050");

  i2c_init();
  mpu6050_init();

  msg_t ret = 0;
  i2cflags_t i2c_ret = 0;
  uint16_t delay = 100;
  uint8_t MSB = 0;
  uint8_t LSB = 0;
  uint8_t who_am_i = 0;

  while(1){
    chThdSleepMilliseconds(delay);

    if(!who_am_i){
      ret = mpu6050_Read(0x75, &who_am_i);
      if(ret != 0) return error();
    }

    ret = mpu6050_Read(0x3B, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(0x3C, &LSB);
    if(ret != 0) return error();

    uint16_t acc_x = (MSB << 8) | LSB; // Read X accelerometer

    ret = mpu6050_Read(0x3D, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(0x3E, &LSB);
    if(ret != 0) return error();

    uint16_t acc_y = (MSB << 8) | LSB; // Read Y accelerometer

    ret = mpu6050_Read(0x3F, &MSB);
    if(ret != 0) return error();

    ret = mpu6050_Read(0x40, &LSB);
    if(ret != 0) return error();

    volatile uint16_t acc_z = (MSB << 8) | LSB; // Read Z accelerometer
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
