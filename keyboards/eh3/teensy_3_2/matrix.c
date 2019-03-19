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
#include <stdbool.h>
#include "ch.h"
// #include "k20x7.h"
// #include "./teensy_3_2/halconf.h"
#include "hal.h"
// #include "../../lib/chibios/os/hal/include/hal.h"
// #include "../config.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"

#include <stdint.h>
#include "timer.h"
#include <string.h>

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

struct pal_port_t {
  ioportid_t  port ;
  uint8_t     pin  ;
};


#define PIN_0           { .port=TEENSY_PIN0_IOPORT, .pin=TEENSY_PIN0 }
#define PIN_1           { .port=TEENSY_PIN1_IOPORT, .pin=TEENSY_PIN1 }
#define PIN_2           { .port=TEENSY_PIN2_IOPORT, .pin=TEENSY_PIN2 }
#define PIN_3           { .port=TEENSY_PIN3_IOPORT, .pin=TEENSY_PIN3 }
#define PIN_4           { .port=TEENSY_PIN4_IOPORT, .pin=TEENSY_PIN4 }
#define PIN_5           { .port=TEENSY_PIN5_IOPORT, .pin=TEENSY_PIN5 }
#define PIN_6           { .port=TEENSY_PIN6_IOPORT, .pin=TEENSY_PIN6 }
#define PIN_7           { .port=TEENSY_PIN7_IOPORT, .pin=TEENSY_PIN7 }
#define PIN_8           { .port=TEENSY_PIN8_IOPORT, .pin=TEENSY_PIN8 }
#define PIN_9           { .port=TEENSY_PIN9_IOPORT, .pin=TEENSY_PIN9 }
#define PIN_10          { .port=TEENSY_PIN10_IOPORT, .pin=TEENSY_PIN10 }
#define PIN_11          { .port=TEENSY_PIN11_IOPORT, .pin=TEENSY_PIN11 }
#define PIN_12          { .port=TEENSY_PIN12_IOPORT, .pin=TEENSY_PIN12 }
#define PIN_13          { .port=TEENSY_PIN13_IOPORT, .pin=TEENSY_PIN13 }
#define PIN_14          { .port=TEENSY_PIN14_IOPORT, .pin=TEENSY_PIN14 }
#define PIN_15          { .port=TEENSY_PIN15_IOPORT, .pin=TEENSY_PIN15 }
#define PIN_16          { .port=TEENSY_PIN16_IOPORT, .pin=TEENSY_PIN16 }
#define PIN_17          { .port=TEENSY_PIN17_IOPORT, .pin=TEENSY_PIN17 }
#define PIN_18          { .port=TEENSY_PIN18_IOPORT, .pin=TEENSY_PIN18 }
#define PIN_19          { .port=TEENSY_PIN19_IOPORT, .pin=TEENSY_PIN19 }
#define PIN_20          { .port=TEENSY_PIN20_IOPORT, .pin=TEENSY_PIN20 }
#define PIN_21          { .port=TEENSY_PIN21_IOPORT, .pin=TEENSY_PIN21 }
#define PIN_22          { .port=TEENSY_PIN22_IOPORT, .pin=TEENSY_PIN22 }
#define PIN_23          { .port=TEENSY_PIN23_IOPORT, .pin=TEENSY_PIN23 }
// #define PIN_24          { TEENSY_PIN24_IOPORT, .pin=TEENSY_PIN24 }
// #define PIN_25          { TEENSY_PIN25_IOPORT, .pin=TEENSY_PIN25 }
// #define PIN_25          { TEENSY_PIN25_IOPORT, .pin=TEENSY_PIN25 }
// #define PIN_26          { TEENSY_PIN26_IOPORT, .pin=TEENSY_PIN26 }
// #define PIN_27          { TEENSY_PIN27_IOPORT, .pin=TEENSY_PIN27 }
// #define PIN_28          { TEENSY_PIN28_IOPORT, .pin=TEENSY_PIN28 }
#define PIN_29          { .port=TEENSY_PIN29_IOPORT, .pin=TEENSY_PIN29 }
// #define PIN_30          { TEENSY_PIN30_IOPORT, .pin=TEENSY_PIN30 }
// #define PIN_31          { TEENSY_PIN31_IOPORT, .pin=TEENSY_PIN31 }
// #define PIN_32          { TEENSY_PIN32_IOPORT, .pin=TEENSY_PIN32 }
// #define PIN_33          { TEENSY_PIN33_IOPORT, .pin=TEENSY_PIN33 }



// static const uint8_t row_pins[MATRIX_ROWS] = {F7, F6, F5, F4, F1};
struct pal_port_t row_pins[MATRIX_ROWS] = { PIN_19, PIN_20, PIN_21, PIN_22, PIN_23, PIN_29 };
struct pal_port_t col_pins[MATRIX_COLS] = {
  PIN_0,
  PIN_1,
  PIN_2,
  PIN_3,
  PIN_4,
  PIN_5,
  PIN_6,
  PIN_7,
  PIN_8,
  PIN_9,
  PIN_10,
  PIN_11,
  PIN_12,
  PIN_13,
  PIN_14,
  PIN_15,
  PIN_16,
  PIN_17,
  PIN_18
};


// static matrix_col_t read_cols(void);
// static void init_cols(void);
// static void unselect_rows(void);
// static void select_row(uint8_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}


/*
 * Infinity Pinusage:
 * Column pins are input with internal pull-down. Row pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 *  INFINITY PRODUCTION (NO LED)
 *     col: { PTD1, PTD2, PTD3, PTD4, PTD5, PTD6, PTD7 }
 *     row: { PTB0, PTB1, PTB2, PTB3, PTB16, PTB17, PTC4, PTC5, PTD0 }
 *  INFINITY PRODUCTION (WITH LED)
 *     col: { PTD1, PTD2, PTD3, PTD4, PTD5, PTD6, PTD7 }
 *     row: { PTC0, PTC1, PTC2, PTC3, PTC4, PTC5, PTC6, PTC7, PTD0 }
 */
/* matrix state(1:on, 0:off) */
// static matrix_row_t matrix[MATRIX_ROWS];
// static matrix_row_t matrix_debouncing[MATRIX_ROWS];
// // static bool debouncing = false;
static uint16_t debouncing_time = 0;


void matrix_init(void)
{
    /* Column(sense) */
    // for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

    // palReadPad(TEENSY_PIN0_IOPORT, TEENSY_PIN0);
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
      palSetPadMode(col_pins[col].port, col_pins[col].pin, PAL_MODE_INPUT_PULLUP);
    }

#ifdef INFINITY_LED
    /* Row(strobe) */
    palSetPadMode(GPIOC, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 2,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 4,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 5,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 0,  PAL_MODE_OUTPUT_PUSHPULL);
#else
    /* Row(strobe) */
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
      palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_INPUT);
    }


    // palSetLineMode(TEENSY_PIN0_IOPORT, PAL_MODE_INPUT);

//     palSetPadMode(GPIOB, 1,  PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOB, 2,  PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOB, 3,  PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOB, 16, PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOB, 17, PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOC, 4,  PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOC, 5,  PAL_MODE_OUTPUT_PUSHPULL);
//     palSetPadMode(GPIOD, 0,  PAL_MODE_OUTPUT_PUSHPULL);
#endif
    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, MATRIX_ROWS * sizeof(matrix_row_t));

    matrix_init_quantum();
}




uint8_t matrix_scan(void) {
    for (int row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t data = 0;
    #ifdef INFINITY_LED
        // strobe row
        switch (row) {
            case 0: palSetPad(GPIOC, 0);    break;
            case 1: palSetPad(GPIOC, 1);    break;
            case 2: palSetPad(GPIOC, 2);    break;
            case 3: palSetPad(GPIOC, 3);    break;
            case 4: palSetPad(GPIOC, 4);    break;
            case 5: palSetPad(GPIOC, 5);    break;
            case 6: palSetPad(GPIOC, 6);    break;
            case 7: palSetPad(GPIOC, 7);    break;
            case 8: palSetPad(GPIOD, 0);    break;
        }
    #else
        // strobe row
        palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_OUTPUT_PUSHPULL);
    #endif

        // need wait to settle pin state
        // if you wait too short, or have a too high update rate
        // the keyboard might freeze, or there might not be enough
        // processing power to update the LCD screen properly.
        // 20us, or two ticks at 100000Hz seems to be OK
        wait_us(20);

        // read col data
      for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        data = ( ( data << 1 ) | palReadPad(col_pins[col].port, col_pins[col].pin) );
      }
    #ifdef INFINITY_LED
        // un-strobe row
        switch (row) {
            case 0: palClearPad(GPIOC, 0);    break;
            case 1: palClearPad(GPIOC, 1);    break;
            case 2: palClearPad(GPIOC, 2);    break;
            case 3: palClearPad(GPIOC, 3);    break;
            case 4: palClearPad(GPIOC, 4);    break;
            case 5: palClearPad(GPIOC, 5);    break;
            case 6: palClearPad(GPIOC, 6);    break;
            case 7: palClearPad(GPIOC, 7);    break;
            case 8: palClearPad(GPIOD, 0);    break;
        }
    #else
        // un-strobe row
        palClearPad(row_pins[row].port, row_pins[row].pin);
    #endif

        if (matrix_debouncing[row] != data) {
            matrix_debouncing[row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    matrix_scan_quantum();
    return 1;
}





bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}




void matrix_print(void)
{
    xprintf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: ");
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}

// __attribute__ ((weak))
// void matrix_init_kb(void) {
//     matrix_init_user();
// }

// __attribute__ ((weak))
// void matrix_scan_kb(void) {
//     matrix_scan_user();
// }

// __attribute__ ((weak))
// void matrix_init_user(void) {
// }

// __attribute__ ((weak))
// void matrix_scan_user(void) {
// }




// #define LED_ON()    do { palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13) ;} while (0)
// #define LED_OFF()   do { palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)
// #define LED_TGL()   do { palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)

// void matrix_init(void)
// {
//     // initialize row and col
//     unselect_rows();
//     init_cols();

//     // initialize matrix state: all keys off
//     for (uint8_t i=0; i < MATRIX_ROWS; i++) {
//         matrix[i] = 0;
//         matrix_debouncing[i] = 0;
//     }

//     //debug
//     debug_matrix = true;
//     // LED_ON();
//     wait_ms(500);
//     // LED_OFF();
// }

// uint8_t matrix_scan(void)
// {
//     for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
//         select_row(i);
//         wait_us(30);  // without this wait read unstable value.
//         matrix_row_t cols = read_cols();
//         if (matrix_debouncing[i] != cols) {
//             matrix_debouncing[i] = cols;
//             if (debouncing) {
//                 debug("bounce!: "); debug_hex(debouncing); debug("\n");
//             }
//             debouncing = DEBOUNCE;
//         }
//         unselect_rows();
//     }

//     if (debouncing) {
//         if (--debouncing) {
//             wait_ms(1);
//         } else {
//             for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
//                 matrix[i] = matrix_debouncing[i];
//             }
//         }
//     }

//     return 1;
// }

// inline
// bool matrix_is_on(uint8_t row, uint8_t col)
// {
//     return (matrix[row] & ((matrix_row_t)1<<col));
// }

// inline
// matrix_row_t matrix_get_row(uint8_t row)
// {
//     return matrix[row];
// }

// void matrix_print(void)
// {
//     print("\nr/c 0123456789ABCDEF\n");
//     for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//         phex(row); print(": ");
//         pbin_reverse16(matrix_get_row(row));
//         print("\n");
//     }
// }

// /* Column pin configuration
//  */
// static void  init_cols(void)
// {
//     // internal pull-up
//     palSetPadMode(TEENSY_PIN2_IOPORT, TEENSY_PIN2, PAL_MODE_INPUT_PULLUP);
// }

// /* Returns status of switches(1:on, 0:off) */
// static matrix_col_t read_cols(void)
// {
//     return ((palReadPad(TEENSY_PIN2_IOPORT, TEENSY_PIN2)==PAL_HIGH) ? 0 : (1<<0));
//     // | ((palReadPad(...)==PAL_HIGH) ? 0 : (1<<1))
// }

// /* Row pin configuration
//  */
// static void unselect_rows(void)
// {
//     palSetPadMode(TEENSY_PIN5_IOPORT, TEENSY_PIN5, PAL_MODE_INPUT); // hi-Z
// }

// static void select_row(uint8_t row)
// {
//     (void)row;
//     // Output low to select
//     switch (row) {
//         case 0:
//             palSetPadMode(TEENSY_PIN5_IOPORT, TEENSY_PIN5, PAL_MODE_OUTPUT_PUSHPULL);
//             palClearPad(TEENSY_PIN5_IOPORT, TEENSY_PIN5);
//             break;
//     }
// }
