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
#include "debug.h"
#include "matrix.h"
#include "print.h"
#include "util.h"
#include "wait.h"

#ifndef DEBOUNCE
#define DEBOUNCE 5
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
  debug_enable = true;
  debug_matrix = true;
  // debug_keyboard=true;
  // debug_mouse=true;
  print("hello world\n");
  // matrix_print();
}

struct pal_port_t {
  ioportid_t port;
  uint8_t pin;
};

// cols
#define PIN_30                                                                  \
  { .port = TEENSY_PIN30_IOPORT, .pin = TEENSY_PIN30 }
#define PIN_0                                                                  \
  { .port = TEENSY_PIN0_IOPORT, .pin = TEENSY_PIN0 }
#define PIN_1                                                                  \
  { .port = TEENSY_PIN1_IOPORT, .pin = TEENSY_PIN1 }
#define PIN_2                                                                  \
  { .port = TEENSY_PIN2_IOPORT, .pin = TEENSY_PIN2 }
#define PIN_3                                                                  \
  { .port = TEENSY_PIN3_IOPORT, .pin = TEENSY_PIN3 }
#define PIN_4                                                                  \
  { .port = TEENSY_PIN4_IOPORT, .pin = TEENSY_PIN4 }
#define PIN_5                                                                  \
  { .port = TEENSY_PIN5_IOPORT, .pin = TEENSY_PIN5 }
#define PIN_6                                                                  \
  { .port = TEENSY_PIN6_IOPORT, .pin = TEENSY_PIN6 }
#define PIN_7                                                                  \
  { .port = TEENSY_PIN7_IOPORT, .pin = TEENSY_PIN7 }
#define PIN_8                                                                  \
  { .port = TEENSY_PIN8_IOPORT, .pin = TEENSY_PIN8 }
#define PIN_9                                                                  \
  { .port = TEENSY_PIN9_IOPORT, .pin = TEENSY_PIN9 }
#define PIN_10                                                                 \
  { .port = TEENSY_PIN10_IOPORT, .pin = TEENSY_PIN10 }
#define PIN_11                                                                 \
  { .port = TEENSY_PIN11_IOPORT, .pin = TEENSY_PIN11 }
#define PIN_12                                                                 \
  { .port = TEENSY_PIN12_IOPORT, .pin = TEENSY_PIN12 }
/**
 * THIS IS THE ONBOARD LED
#define PIN_13                                                                 \
  { .port = TEENSY_PIN13_IOPORT, .pin = TEENSY_PIN13 }
**/
#define PIN_14                                                                 \
  { .port = TEENSY_PIN14_IOPORT, .pin = TEENSY_PIN14 }
#define PIN_15                                                                 \
  { .port = TEENSY_PIN15_IOPORT, .pin = TEENSY_PIN15 }
#define PIN_16                                                                 \
  { .port = TEENSY_PIN16_IOPORT, .pin = TEENSY_PIN16 }
#define PIN_17                                                                 \
  { .port = TEENSY_PIN17_IOPORT, .pin = TEENSY_PIN17 }
#define PIN_18                                                                 \
  { .port = TEENSY_PIN18_IOPORT, .pin = TEENSY_PIN18 }
// rows
#define PIN_19                                                                 \
  { .port = TEENSY_PIN19_IOPORT, .pin = TEENSY_PIN19 }
#define PIN_20                                                                 \
  { .port = TEENSY_PIN20_IOPORT, .pin = TEENSY_PIN20 }
#define PIN_21                                                                 \
  { .port = TEENSY_PIN21_IOPORT, .pin = TEENSY_PIN21 }
#define PIN_22                                                                 \
  { .port = TEENSY_PIN22_IOPORT, .pin = TEENSY_PIN22 }
#define PIN_23                                                                 \
  { .port = TEENSY_PIN23_IOPORT, .pin = TEENSY_PIN23 }
#define PIN_26                                                                 \
  { .port = TEENSY_PIN26_IOPORT, .pin = TEENSY_PIN26 }
#define PIN_27                                                                 \
  { .port = TEENSY_PIN27_IOPORT, .pin = TEENSY_PIN27 }
#define PIN_29                                                                 \
  { .port = TEENSY_PIN29_IOPORT, .pin = TEENSY_PIN29 }
#define PIN_30                                                                 \
  { .port = TEENSY_PIN30_IOPORT, .pin = TEENSY_PIN30 }
#define PIN_31                                                                 \
  { .port = TEENSY_PIN31_IOPORT, .pin = TEENSY_PIN31 }

// leftmost is start
// PIN_18 is ESCAPE
struct pal_port_t col_pins[MATRIX_COLS] = { PIN_30, PIN_17, PIN_16, PIN_15, PIN_14, PIN_26, PIN_12, PIN_11, PIN_10, PIN_9, PIN_8, PIN_7, PIN_6, PIN_5, PIN_4, PIN_3, PIN_2, PIN_1, PIN_0 };
struct pal_port_t row_pins[MATRIX_ROWS] = { PIN_31, PIN_20, PIN_21, PIN_22, PIN_23, PIN_27 };

inline uint8_t matrix_rows(void) { return MATRIX_ROWS; }

inline uint8_t matrix_cols(void) { return MATRIX_COLS; }

#define LED_ON()                                                               \
  do {                                                                         \
    palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);                              \
  } while (0)
#define LED_OFF()                                                              \
  do {                                                                         \
    palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);                            \
  } while (0)
#define LED_TGL()                                                              \
  do {                                                                         \
    palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);                           \
  } while (0)


void matrix_init(void) {
  // initialize row and col
  unselect_rows();
  init_cols();

  // initialize matrix state: all keys off
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    matrix_debouncing[i] = 0;
  }

  // debug
  debug_enable = true;
  debug_matrix = true;
  LED_ON();
  wait_ms(500);
  LED_OFF();
  LED_ON();
}

uint8_t matrix_scan(void) {
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    select_row(i);
    wait_us(30); // without this wait read unstable value.
    matrix_row_t cols = read_cols();
    if (matrix_debouncing[i] != cols) {
      matrix_debouncing[i] = cols;
      if (debouncing) {
        debug("bounce!: ");
        debug_hex(debouncing);
        debug("\n");
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

inline bool matrix_is_on(uint8_t row, uint8_t col) {
  return (matrix[row] & ((matrix_row_t)1 << col));
}

inline matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

void matrix_print(void) {
  print("\nr/c 01234567890123456789012345678901\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row);
    print(": ");
    // Output low to select
    dprintf("matrix_get_row[%u] | row: %u - hex: %X ; dec: %d ; unsigned: %u\n",
            row, row, matrix_get_row(row), matrix_get_row(row),
            matrix_get_row(row));
    switch (sizeof(matrix_row_t)) {
    case 1:
      print_bin_reverse8(matrix_get_row(row));
      break;
    case 2:
      print_bin_reverse16(matrix_get_row(row));
      break;
    case 4:
      print_bin_reverse32(matrix_get_row(row));
      break;
    }
    print("\n");
  }
}

/* Column pin configuration
 */
static void init_cols(void) {
  print("init_cols print");
  debug("init_cols DEBUG");
  // internal pull-up
  // palSetPadMode(TEENSY_PIN18_IOPORT, TEENSY_PIN18, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(TEENSY_PIN17_IOPORT, TEENSY_PIN17, PAL_MODE_INPUT_PULLUP);

  for (int col = 0; col < MATRIX_COLS; col++) {
    palSetPadMode(col_pins[col].port, col_pins[col].pin, PAL_MODE_INPUT_PULLUP);
  }
}

bool print_details = true;

static void print_pad_value(matrix_row_t pin_value, uint8_t pin_row,
                            uint8_t pin_col) {
  dprintf("**************************************** row: %u ; col: %u "
          "****************************************\n",
          pin_row, pin_col);
  dprintf("sizeof(matrix_row_t) = %d (%d)\n", sizeof(matrix_row_t),
          (sizeof(matrix_row_t) * 8));
  dprintf("matrix[%u] | row: %u col: %u  - hex: %X ; dec: %d ; unsigned: %u\n",
          pin_row, pin_row, pin_col, matrix[pin_row], matrix[pin_row],
          matrix[pin_row]);

  dprintf("vals | row: %u col: %u  - PAL_HIGH: %u ; PAL_LOW: %u ; PIN==HIGH ?: "
          "%u\n",
          pin_row, pin_col, PAL_HIGH, PAL_LOW, (pin_value == PAL_HIGH));

  dprintf("debug_enable = %d\n", debug_enable);
  dprintf("raw | row: %u col: %u  - hex: %X ; dec: %d ; unsigned: %u\n",
          pin_row, pin_col, pin_value, pin_value, pin_value);

  dprintf("raw | row: %d col: %d - bin8 : [", pin_row, pin_col);
  debug_bin8(pin_value);
  dprint("]\n");

  dprintf("raw | row: %d col: %d - bin16: [", pin_row, pin_col);
  debug_bin16(pin_value);
  dprint("]\n");

  dprintf("raw | row: %d col: %d - bin32: [", pin_row, pin_col);
  debug_bin32(pin_value);
  dprint("]\n");

  dprintf("raw | row: %d col: %d - bin8 reversed : [", pin_row, pin_col);
  debug_bin_reverse8(pin_value);
  dprint("]\n");

  dprintf("shift_0: %d, %u, %X\n", (pin_value ? 0 : (1 << 0)),
          (pin_value ? 0 : (1 << 0)), (pin_value ? 0 : (1 << 0)));
  print("shift_0 | debug_bin8 [");
  debug_bin8((pin_value ? 0 : (1 << 0)));
  print("]\n");

  dprintf("shift_1: %d, %u, %X\n", (pin_value ? 0 : (1 << 1)),
          (pin_value ? 0 : (1 << 1)), (pin_value ? 0 : (1 << 1)));
  print("shift_1 | debug_bin8: [");
  debug_bin8((pin_value ? 0 : (1 << 1)));
  print("]\n");

  // if (print_details) {
  //   palSetPadMode(TEENSY_PIN16_IOPORT, TEENSY_PIN16, PAL_MODE_INPUT_PULLUP);
  //   matrix_row_t p16val = palReadPad(TEENSY_PIN16_IOPORT, TEENSY_PIN16);
  //   dprintf("read p16 after input_pullup: %d, %u, %X\n", p16val, p16val,
  //           p16val);
  //   palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_PUSHPULL);
  //   palClearPad(TEENSY_PIN19_IOPORT, TEENSY_PIN19);
  //   p16val = palReadPad(TEENSY_PIN16_IOPORT, TEENSY_PIN16);
  //   dprintf("read p16 after row output pushpull: %d, %u, %X\n", p16val, p16val,
  //           p16val);
  //   print_details = false;
  // }
  dprintf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ row: %u ; col: %u "
          "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n",
          pin_row, pin_col);
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void) {
  matrix_row_t in_row = 0;
	matrix_row_t pin_val = 0;
  
	// matrix_row_t pin_val = palReadPad(TEENSY_PIN18_IOPORT, TEENSY_PIN18);
  // matrix_row_t in_row = ((pin_val == PAL_HIGH) ? 0 : (1 << 0));
  // if (in_row != 0) {
  //   print_pad_value(pin_val, 0, 18);
  // }
  // pin_val = (palReadPad(TEENSY_PIN17_IOPORT, TEENSY_PIN17) == PAL_HIGH);
  // matrix_row_t in_row_col_17 = ((pin_val == PAL_HIGH) ? 0 : (1 << 1));
  // if (in_row_col_17 != 0) {
  //   print_pad_value(pin_val, 0, 17);
  //   // wait_ms(500);
  // }
  for (int col = 0; col < MATRIX_COLS; col++) {
		pin_val = palReadPad(col_pins[col].port, col_pins[col].pin);
  	if (((pin_val == PAL_HIGH) ? 0 : (1 << 0)) != 0) {
    	print_pad_value( pin_val, 0, col);
		}
    in_row |= (( palReadPad(col_pins[col].port, col_pins[col].pin) == PAL_HIGH) ? 0 : (1 << col ));

		pin_val = 0;
  }

  // for (int col = 0; col < MATRIX_COLS; col++) {
  //   palSetPadMode(col_pins[col].port, col_pins[col].pin,
  //   PAL_MODE_INPUT_PULLUP);
  // }
  return in_row;
  // | ((palReadPad(...)==PAL_HIGH) ? 0 : (1<<1))
}

/* Row pin configuration
 */
static void unselect_rows(void) {
  // palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_INPUT); // hi-Z
  for (int row = 0; row < MATRIX_ROWS; row++) {
    palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_INPUT);
  }
}

static void select_row(uint8_t row) {
  (void)row;
  // Output low to select
  // switch (row) {
  // case 0:
  //   palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_PUSHPULL);
  //   palClearPad(TEENSY_PIN19_IOPORT, TEENSY_PIN19);
  //   break;
  // case 1:
  //   palSetPadMode(TEENSY_PIN20_IOPORT, TEENSY_PIN20, PAL_MODE_OUTPUT_PUSHPULL);
  //   palClearPad(TEENSY_PIN20_IOPORT, TEENSY_PIN20);
  //   break;
  // }
	palSetPadMode(row_pins[row].port, row_pins[row].pin, PAL_MODE_OUTPUT_PUSHPULL);
}
