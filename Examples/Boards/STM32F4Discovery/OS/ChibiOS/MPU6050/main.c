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
#include "chprintf.h"

#include "usbcfg.h"
#include "i2c.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "mpu6050.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char len, unsigned char const* data) {
  static uint8_t data_arr[256];// = malloc(len + 1);
  data_arr[0] = reg_addr;
//  memcpy(data_arr + 1, data, len);
  int i = 0;
  for(i = 1; i <= len; ++i){
    data_arr[i] = data[i - 1];
  }

  int ret = i2cMasterTransmitTimeout(&I2CD1, slave_addr, data_arr, len + 1, NULL, 0, TIME_INFINITE);
//  free(data_arr);
  return ret;
}

int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char len, unsigned char* data) {
  return i2cMasterTransmitTimeout(&I2CD1, slave_addr, &reg_addr, 1, data, len, TIME_INFINITE);
}

void delay_ms(unsigned long ms) {
  chThdSleepMilliseconds(ms);
}

void get_ms(unsigned long* ms_out) {
  *ms_out = chVTGetSystemTime();
}

int reg_int_cb(struct int_param_s *int_param)
{
    (void)int_param;
    return 0;
}

void error(void) {
  uint16_t delay = 100;

  while (1) {
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

  (void) arg;
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

static THD_WORKING_AREA(waThreadMPU6050, 4096);
static THD_FUNCTION(ThreadMPU6050, arg) {
  (void) arg;
  uint32_t delay = 100;

//  mpu_set_dmp_state(1);
//  dmp_load_motion_driver_firmware();

  short accel[3] = {0}, gyro[3] = {0};
  long int temperature;
  mpu_init(NULL);
//  mpu6050_init();

  while (1) {
    chThdSleepMilliseconds(delay);
    if(dmp_read_fifo(NULL, accel, NULL, NULL, NULL, NULL)){
      chprintf((BaseSequentialStream*)&SDU1, "error!\n");
      continue;
    }
//    mpu_get_gyro_reg(gyro, NULL);
//    mpu_get_temperature(&temperature, NULL);

    chprintf((BaseSequentialStream*)&SDU1, "acc  X = %d, acc  Y = %d, acc  Z = %d \n", accel[0], accel[1], accel[2]);
//    chThdSleepMilliseconds(10);
//    chprintf((BaseSequentialStream*)&SDU1, "gyro X = %10d, gyro Y = %10d, gyro Z = %10d\n", gyro[0], gyro[1], gyro[2]);
//    chThdSleepMilliseconds(10);
//    chprintf((BaseSequentialStream*)&SDU1, "temperature = %d\n\n", temperature);

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
   * Initializes a serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);
  
    /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  i2c_init();

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO, ThreadBlinker, NULL);
  chThdCreateStatic(waThreadMPU6050, sizeof(waThreadMPU6050), NORMALPRIO, ThreadMPU6050, NULL);

  while (1) {
    uint16_t delay = 500;
    chThdSleepMilliseconds(delay);
  };

  return 0;
}
