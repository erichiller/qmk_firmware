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

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
  print("hello world");
  // matrix_print();

}



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

#define LED_ON()    do { palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13) ;} while (0)
#define LED_OFF()   do { palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)
#define LED_TGL()   do { palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();
    debug("matrix_init")

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    //debug
  debug_enable=true;
    debug_matrix = true;
    LED_ON();
    wait_ms(500);
    LED_OFF();
    LED_ON();
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

/* Column pin configuration
 */
static void init_cols(void)
{
  print("init_cols PRINT");
  debug("init_cols DEBUG");
    // internal pull-up
    for ( uint8_t i = 0 ; i < MATRIX_COLS ; i++ ){
      palSetPadMode(col_pins[i].port , col_pins[i].pin , PAL_MODE_INPUT_PULLUP);
    }
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void)
{
  matrix_row_t col_status = 0;
  for ( uint8_t i = 0 ; i < MATRIX_COLS ; i++){
    col_status = col_status | ((palReadPad(col_pins[i].port , col_pins[i].pin)==PAL_HIGH) ? 0 : (1<<i));
  }
  return col_status;
    // | ((palReadPad(...)==PAL_HIGH) ? 0 : (1<<1))
}

/* Row pin configuration
 */
static void unselect_rows(void)
{
    // palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_INPUT); // hi-Z
    for (int row = 0; row < MATRIX_ROWS; row++) {
      palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_INPUT);
    }
}

static void select_row(uint8_t row)
{


    for (int col = 0; col < MATRIX_COLS; col++) {
      palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(row_pins[row].port, row_pins[row].pin);
    }


    // (void)row;
    // // Output low to select
    // switch (row) {
    //     case 0:
    //         palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_PUSHPULL);
    //         palClearPad(TEENSY_PIN19_IOPORT, TEENSY_PIN19);
    //         break;
    // }
}
