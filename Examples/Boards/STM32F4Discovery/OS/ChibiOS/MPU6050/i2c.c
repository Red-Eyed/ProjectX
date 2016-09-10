#include "i2c.h"

// I2C config

static const I2CConfig i2cfg1 = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE
};

void i2c_init(void){
  i2cStop(&I2CD1);

  /* tune ports for I2C1*/
  palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUPDR_FLOATING); // SCL
  palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUPDR_FLOATING); // SDA

  i2cStart(&I2CD1, &i2cfg1);

  /* startups. Pauses added just to be safe */
  chThdSleepMilliseconds(100);
}
