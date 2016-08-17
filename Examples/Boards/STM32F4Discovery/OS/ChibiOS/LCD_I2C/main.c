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
#include "liquid_crystal.h"

#define LCD_ADDR 0x3f

void error(void) {
    uint16_t delay = 100;

    palSetPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(delay);

    palClearPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(delay);
}

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */

static THD_WORKING_AREA(waThreadBlinker, 128);
static THD_FUNCTION(ThreadBlinker, arg) {

    (void) arg;
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

void delay_ms(uint32_t delay) {
    chThdSleepMilliseconds(delay);
}

void delay_us(uint32_t delay) {
    chThdSleepMicroseconds(delay);
}

int write(uint8_t data) {
    if (i2cMasterTransmitTimeout(&I2CD2, LCD_ADDR, &data, sizeof(data), NULL, 0, TIME_INFINITE))
        error();
    return 0;
}

static THD_WORKING_AREA(waThreadLCD, 128);
static THD_FUNCTION(ThreadLCD, arg) {

    (void) arg;
    chRegSetThreadName("ThreadLCD");
    int delay = 100;
    i2c_init();
    liquid_crystal_t lcd;
    LCD_init(&lcd, delay_ms, delay_us, write, 4, 4);

    while (1) {
        LCD_backlight(&lcd);
        LCD_clear(&lcd);
        LCD_write_String(&lcd, "Hello world!");
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
    chThdCreateStatic(waThreadLCD, sizeof(waThreadLCD), NORMALPRIO, ThreadLCD, NULL);

    while (1) {
        uint16_t delay = 500;
        chThdSleepMilliseconds(delay);
    };

    return 0;
}
