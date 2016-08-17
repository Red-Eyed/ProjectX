#ifndef I2C_H
#define I2C_H

#include "ch.h"
#include "hal.h"

void i2c_init(void);
void i2cStopCond(void);
void i2cStartCond(void);

#endif // I2C_H
