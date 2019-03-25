/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// #include "ch.h"
// #include "hal.h"
// #include "print.h"
// #include "led.h"
// #include "led_controller.h"
// #include "quantum.h"

#include "usb_main.h"

#include "ch.h"
#include "hal.h"
#include "print.h"
#include "led.h"
#include "host.h"

#include "led_controller.h"

#include "suspend.h"


void led_set(uint8_t usb_led) {
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        // output high
        palSetPadMode(TEENSY_PIN13_IOPORT, TEENSY_PIN13, PAL_MODE_OUTPUT_PUSHPULL);
        palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);
    } else {
        // Hi-Z
        palSetPadMode(TEENSY_PIN13_IOPORT, TEENSY_PIN13, PAL_MODE_INPUT);
    }
}

#ifdef LED_MATRIX


/* =================
 * ChibiOS I2C setup
 * ================= */
static const I2CConfig i2ccfg = {
  400000 // clock speed (Hz); 400kHz max for IS31
};

LED_DRIVER_ADDR_1 = 0x74;
// I2C0
I2C1_BANK = TEENSY_PIN29_IOPORT;
I2C1_SCL = TEENSY_PIN29;
I2C1_SDA = TEENSY_PIN30;
#endif