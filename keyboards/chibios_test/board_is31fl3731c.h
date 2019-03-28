/*
Copyright 2016 Fred Sundvik <fsundvik@gmail.com>

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

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#pragma message(VAR_NAME_VALUE(KINETIS_I2C_USE_I2C1))
#if KINETIS_I2C_USE_I2C1 == TRUE
    #define I2CD_ I2CD2
    #pragma message "running DRIVER 1"
    #ifdef I2C1_BANK
        #pragma message "running I2C_BANK I2C1_BANK"
        #pragma message(VAR_NAME_VALUE(I2C1_BANK))
        #pragma message(VAR_NAME_VALUE(TEENSY_PIN29_IOPORT))
        #define I2C_BANK I2C1_BANK
    #endif
    #ifdef I2C1_SCL
        #pragma message "running I2C_SCL I2C1_SCL"
        #pragma message(VAR_NAME_VALUE(I2C1_SCL))
        #pragma message(VAR_NAME_VALUE(TEENSY_PIN29))
        #define I2C_SCL I2C1_SCL
    #endif
    #ifdef I2C1_SDA
        #pragma message "running I2C_SDA I2C1_SDA"
        #pragma message(VAR_NAME_VALUE(I2C1_SDA))
        #pragma message(VAR_NAME_VALUE(TEENSY_PIN30))
        #define I2C_SDA I2C1_SDA
    #endif
#else
#define I2CD_ I2CD1
#define I2C_BANK GPIOB
#define I2C_SCL 16
#define I2C_SDA 1
#pragma message "running DEFAULT DRIVER 0"
#endif

#include "../../../../tmk_core/common/print.h"

static const I2CConfig i2ccfg = {
  400000 // clock speed (Hz); 400kHz max for IS31
};

static const uint8_t led_mask[] = {
	0xFF, 0x00, /* C1-1 -> C1-16 */
	0xFF, 0x00, /* C2-1 -> C2-16 */
	0xFF, 0x00, /* C3-1 -> C3-16 */
	0xFF, 0x00, /* C4-1 -> C4-16 */
	0xFF, 0x00, /* C5-1 -> C5-16 */
	0xFF, 0x00, /* C6-1 -> C6-16 */
	0xFF, 0x00, /* C7-1 -> C7-16 */
	0xFF, 0x00, /* C8-1 -> C8-16 */
	0xFF, 0x00, /* C9-1 -> C9-16 */
};

// The address of the LED
#define LA(c, r) (c + r * 16 )
// Need to be an address that is not mapped, but inside the range of the controller matrix
#define NA LA(8, 8)

// The numbers in the comments are the led numbers DXX on the PCB
// The mapping is taken from the schematic of left hand side
static const uint8_t led_mapping[GDISP_SCREEN_HEIGHT][GDISP_SCREEN_WIDTH] = {
//   1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16
   { LA(0, 0), LA(1, 0), LA(2, 0), LA(3, 0), LA(4, 0), LA(5, 0), LA(6, 0), LA(7, 0), LA(0, 1), LA(1, 1), LA(2, 1), LA(3, 1), LA(4, 1), LA(5, 1), LA(6, 1), LA(7, 1)},
//   17        18        19        20        21        22        23        24        25        26                  27        28        29        30        31
   { LA(0, 2), LA(1, 2), LA(2, 2), LA(3, 2), LA(4, 2), LA(5, 2), LA(6, 2), LA(7, 2), LA(0, 3), LA(1, 3), NA,       LA(2, 3), LA(3, 3), LA(4, 3), LA(5, 3), LA(6, 3)},
//   32        33        34        35        36        37        38        39        40        41                  42        43        44        45        46
   { LA(7, 3), LA(0, 4), LA(1, 4), LA(2, 4), LA(3, 4), LA(4, 4), LA(5, 4), LA(6, 4), LA(7, 4), LA(0, 5), NA,       LA(1, 5), LA(2, 5), LA(3, 5), LA(4, 5), LA(5, 5)},
//   47        48        49        50        51        52        53        54        55        56                  57        58        59        60        61
   { LA(6, 5), LA(7, 5), LA(0, 6), LA(1, 6), LA(2, 6), LA(3, 6), LA(4, 6), LA(5, 6), LA(6, 6), LA(7, 6), NA,       LA(0, 7), LA(1, 7), LA(2, 7), LA(3, 7), LA(4, 7)},
//   62        63        64                                      65                                      66        67        68        69        70        71
   { LA(5, 7), LA(6, 7), LA(7, 7), NA,       NA,       NA,       LA(0, 8), NA,       NA,       NA,       LA(1, 8), LA(2, 8), LA(3, 8), LA(4, 8), LA(5, 8), LA(6, 8)},
};


#define IS31_ADDR_DEFAULT 0x74 // AD connected to GND
#define IS31_TIMEOUT 5000

static GFXINLINE void init_board(GDisplay *g) {
    println("board_is31fl373c.h init_board");
// #ifdef LED_MATRIX_XXX
    (void) g;
    /* I2C pins */
    palSetPadMode(I2C_BANK, I2C_SCL, PAL_MODE_ALTERNATIVE_2); //I2C0 SCL = GPIOB PIN 16 = PTB0/I2C0/SCL
    palSetPadMode(I2C_BANK, I2C_SDA, PAL_MODE_ALTERNATIVE_2); //I2C0 SDA = GPIOB PIN 1 = PTB1/I2C0/SDA
    palSetPadMode(I2C_BANK, I2C_SCL, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(I2C_BANK, I2C_SCL);
    /* start I2C */
    i2cStart(&I2CD_, &i2ccfg);
    // try high drive (from kiibohd)
    I2CD_.i2c->C2 |= I2Cx_C2_HDRS;
    // try glitch fixing (from kiibohd)
    I2CD_.i2c->FLT = 4;
    
    chThdSleepMilliseconds(10);
// #endif
}

static GFXINLINE void post_init_board(GDisplay *g) {
    println("board_is31fl373c.h post_init_board");
	(void) g;
}

static GFXINLINE const uint8_t* get_led_mask(GDisplay* g) {
    println("board_is31fl373c.h get_led_mask");
    (void) g;
    return led_mask;
}

static GFXINLINE uint8_t get_led_address(GDisplay* g, uint16_t x, uint16_t y)
{
    print("g");
    // println("board_is31fl373c.h get_led_address");
    (void) g;
    return led_mapping[y][x];
}

static GFXINLINE void set_hardware_shutdown(GDisplay* g, bool shutdown) {
    println("board_is31fl373c.h set_hardware_shutdown");
// #ifdef LED_MATRIX_XXX
    (void) g;
    if(!shutdown) {
        palSetPad(I2C_BANK, I2C_SCL);
    }
    else {
        palClearPad(I2C_BANK, I2C_SCL);
    }
// #endif
}

static GFXINLINE void write_data(GDisplay *g, uint8_t* data, uint16_t length) {
    printf("board_is31fl373c.h write_data\n\tdata=%x\n\tdata*x=%0x\n\tlength=%u\n", data, *data, length);
// #ifdef LED_MATRIX_XXX
	(void) g;
	i2cMasterTransmitTimeout(&I2CD_, IS31_ADDR_DEFAULT, data, length, 0, 0, US2ST(IS31_TIMEOUT));
// #endif
}

#endif /* _GDISP_LLD_BOARD_H */
