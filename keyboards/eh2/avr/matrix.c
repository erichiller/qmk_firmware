/*
Copyright 2016 Wez Furlong

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
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include <stdbool.h>

#include "debug.h"
#include "eh2.h"
#include "config.h"
#include "lib/lufa/LUFA/Drivers/Peripheral/TWI.h"
#include "matrix.h"
#include "print.h"
#include "timer.h"
#include "util.h"
#include "wait.h"
#include "pincontrol.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#endif
#include "suspend.h"
#ifdef ADAFRUIT_BLE_ENABLE
#include "adafruit_ble.h"
#endif
#include <util/atomic.h>
#include <string.h>

// The keyboard matrix is attached to the following pins:
// row0: A0 - PF7
// row1: A1 - PF6
// row2: A2 - PF5
// row3: A3 - PF4
// row4: A4 - PF1
// col0-7: mcp23107 GPIOA0-7
// col8-14: mcp23107 GPIOB1-7 (note that B0 is unused)
// PD3 (INT3) connect to interrupt pins on mcp23107
static const uint8_t row_pins[MATRIX_ROWS] = {F7, F6, F5, F4, F1};
#if DEBOUNCING_DELAY > 0
static bool debouncing;
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
#endif
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// matrix power saving
#define MATRIX_POWER_SAVE       600000 // 10 minutes
static uint32_t matrix_last_modified;

#define ENABLE_BLE_MODE_LEDS 0

#ifdef DEBUG_MATRIX_SCAN_RATE
static uint32_t scan_timer;
static uint32_t scan_count;
#endif

static inline void select_row(uint8_t row) {
  uint8_t pin = row_pins[row];

  pinMode(pin, PinDirectionOutput);
  digitalWrite(pin, PinLevelLow);
}

static inline void unselect_row(uint8_t row) {
  uint8_t pin = row_pins[row];

  digitalWrite(pin, PinLevelHigh);
  pinMode(pin, PinDirectionInput);
}

static void unselect_rows(void) {
  for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
    unselect_row(x);
  }
}

static void select_rows(void) {
  for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
    select_row(x);
  }
}

void matrix_power_down(void) {
  iota_gfx_off();
#if defined(ADAFRUIT_BLE_ENABLE) && ADAFRUIT_BLE_ENABLE_MODE_LEDS
  adafruit_ble_set_mode_leds(false);
#endif
}

void matrix_power_up(void) {
  unselect_rows();

  memset(matrix, 0, sizeof(matrix));
#if DEBOUNCING_DELAY > 0
  memset(matrix_debouncing, 0, sizeof(matrix_debouncing));
#endif

  matrix_last_modified = timer_read32();
#ifdef DEBUG_MATRIX_SCAN_RATE
  scan_timer = timer_read32();
  scan_count = 0;
#endif

  iota_gfx_on();
#if defined(ADAFRUIT_BLE_ENABLE) && ADAFRUIT_BLE_ENABLE_MODE_LEDS
  adafruit_ble_set_mode_leds(true);
#endif
}

void matrix_init(void) {
  TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 400000));
  iota_mcp23017_init();
  iota_gfx_init();

//  pinMode(D3, PinDirectionInput);
//  iota_mcp23017_enable_interrupts();

  matrix_power_up();
}

bool matrix_is_on(uint8_t row, uint8_t col) {
  return (matrix[row] & ((matrix_row_t)1 << col));
}

matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

static bool read_cols_on_row(matrix_row_t current_matrix[],
                             uint8_t current_row) {
  // Store last value of row prior to reading
  matrix_row_t last_row_value = current_matrix[current_row];

  // Clear data in matrix row
  current_matrix[current_row] = 0;

  // Select row and wait for row selection to stabilize
  select_row(current_row);
  _delay_us(30);

  current_matrix[current_row] = iota_mcp23017_read();

  unselect_row(current_row);

  return last_row_value != current_matrix[current_row];
}

#define MCP_GP_BASE 0x70
#define GPB0 (MCP_GP_BASE + 0)
#define GPB1 (MCP_GP_BASE + 1)
#define GPB2 (MCP_GP_BASE + 2)
#define GPB3 (MCP_GP_BASE + 3)
#define GPB4 (MCP_GP_BASE + 4)
#define GPB5 (MCP_GP_BASE + 5)
#define GPB6 (MCP_GP_BASE + 6)
#define GPB7 (MCP_GP_BASE + 7)
#define GPA0 (MCP_GP_BASE + 0x08 + 0)
#define GPA1 (MCP_GP_BASE + 0x08 + 1)
#define GPA2 (MCP_GP_BASE + 0x08 + 2)
#define GPA3 (MCP_GP_BASE + 0x08 + 3)
#define GPA4 (MCP_GP_BASE + 0x08 + 4)
#define GPA5 (MCP_GP_BASE + 0x08 + 5)
#define GPA6 (MCP_GP_BASE + 0x08 + 6)
#define GPA7 (MCP_GP_BASE + 0x08 + 7)

#define A0  F7
#define A1  F6
#define A2  F5
#define A3  F4
#define A4  F1
#define A5  F0
#define A7  D7
#define A9  B5
#define A10 B6
#define A11 D6


#define MATRIX_ROW_PINS { A0 , A1 , A2 , A3 , A4 , A5 }
//                        C=0    C=1    C=2    C=3    C=4    C=5    C=6    C=7    C=8   C=9   C=10   C=11   C=12   C=13   C=14   C=15   C=16   C=17   C=18 //
#define MATRIX_COL_PINS { GPA2 , GPA3 , GPA1 , GPB4 , GPB5 , GPB7 , GPB6 , D7   , B5  , B6  , GPA4 , D6   , GPB0 , GPA7 , GPB1 , GPA6 , GPB2 , GPA5 , GPB3 }
static const matrix_col_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;


static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row){
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selecton to stabilize
    select_row(current_row);
    wait_us(30);



    // For each col...
    for(uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {

      if ( (col_pins[col_index] >> 4) == (MCP_GP_BASE >> 4) ){
        // read pin from MCP23017

      } else {
        // Read pin from adafruit
        // Select the col pin to read (active low)
        uint8_t pin = col_pins[col_index];
        uint8_t pin_state = (_SFR_IO8(pin >> 4) & _BV(pin & 0xF));

        // Populate the matrix row with the state of the col pin
        current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
      }
    }

    // Unselect row
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}




static uint8_t matrix_scan_raw(void) {

  if (!iota_mcp23017_make_ready()) {
    return 0;
  }

  for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
    bool matrix_changed = read_cols_on_row(
#if DEBOUNCING_DELAY > 0
        matrix_debouncing,
#else
        matrix,
#endif
        current_row);

    if (matrix_changed) {
#if DEBOUNCING_DELAY > 0
      debouncing = true;
#endif
      matrix_last_modified = timer_read32();
    }
  }

#ifdef DEBUG_MATRIX_SCAN_RATE
  scan_count++;

  uint32_t timer_now = timer_read32();
  if (TIMER_DIFF_32(timer_now, scan_timer)>1000) {
    print("matrix scan frequency: ");
    pdec(scan_count);
    print("\n");

    scan_timer = timer_now;
    scan_count = 0;
  }
#endif

#if DEBOUNCING_DELAY > 0
  if (debouncing &&
      (timer_elapsed32(matrix_last_modified) > DEBOUNCING_DELAY)) {
    memcpy(matrix, matrix_debouncing, sizeof(matrix));
    debouncing = false;
  }
#endif

  return 1;
}

uint8_t matrix_scan(void) {
  iota_gfx_task();

  if (!matrix_scan_raw()) {
    return 0;
  }

  // Try to manage battery power a little better than the default scan.
  // If the user is idle for a while, turn off some things that draw
  // power.

  if (timer_elapsed32(matrix_last_modified) > MATRIX_POWER_SAVE) {
    matrix_power_down();

    // Turn on all the rows; we're going to read the columns in
    // the loop below to see if we got woken up.
    select_rows();

    while (true) {
      suspend_power_down();

      // See if any keys have been pressed.
      if (!iota_mcp23017_read()) {
        continue;
      }

      // Wake us up
      matrix_last_modified = timer_read32();
      suspend_wakeup_init();
      matrix_power_up();

      // Wake the host up, if appropriate.
      if (USB_DeviceState == DEVICE_STATE_Suspended &&
          USB_Device_RemoteWakeupEnabled) {
        USB_Device_SendRemoteWakeup();
      }
      break;
    }
  }

  matrix_scan_quantum();
  return 1;
}

void matrix_print(void) {
  print("\nr/c 0123456789ABCDEF\n");

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row);
    print(": ");
    print_bin_reverse16(matrix_get_row(row));
    print("\n");
  }
}
