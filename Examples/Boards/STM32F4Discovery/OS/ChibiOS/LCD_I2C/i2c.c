#include "i2c.h"

// I2C config

const I2CConfig i2cfg = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE
};

void i2c_init(void){
  i2cStop(&I2CD2);

  /* tune ports for I2C2*/
  palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUPDR_FLOATING); // SCL
  palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUPDR_FLOATING); // SDA

  i2cStart(&I2CD2, &i2cfg);

  /* startups. Pauses added just to be safe */
  chThdSleepMilliseconds(100);
}

void i2cStopCond(void){
    i2cStop(&I2CD2);
}

void i2cStartCond(void){
    i2cStart(&I2CD2, &i2cfg);
}

