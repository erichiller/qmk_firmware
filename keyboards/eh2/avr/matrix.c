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
#include "LUFA/Drivers/Peripheral/TWI.h"
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
#include <util/delay.h>


// The keyboard matrix is attached to the following pins:
// row0: A0 - PF7
// row1: A1 - PF6
// row2: A2 - PF5
// row3: A3 - PF4
// row4: A4 - PF1
// col0-7: mcp23107 GPIOA0-7
// col8-14: mcp23107 GPIOB1-7 (note that B0 is unused)
// PD3 (INT3) connect to interrupt pins on mcp23107

static const matrix_col_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

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
  // iota_gfx_off();
#if defined(ADAFRUIT_BLE_ENABLE) && ADAFRUIT_BLE_ENABLE_MODE_LEDS
  adafruit_ble_set_mode_leds(false);
#endif
}



void matrix_power_up(void) {
  unselect_rows();
//   init_cols();

  memset(matrix, 0, sizeof(matrix));
#if DEBOUNCING_DELAY > 0
  memset(matrix_debouncing, 0, sizeof(matrix_debouncing));
#endif

  matrix_last_modified = timer_read32();
#ifdef DEBUG_MATRIX_SCAN_RATE
  scan_timer = timer_read32();
  scan_count = 0;
#endif

  // iota_gfx_on();
#if defined(ADAFRUIT_BLE_ENABLE) && ADAFRUIT_BLE_ENABLE_MODE_LEDS
  adafruit_ble_set_mode_leds(true);
#endif
}

static void init_cols(void){
	for(uint8_t x = 0; x < MATRIX_COLS; x++) {
		uint8_t pin = col_pins[x];

		if ( ( (col_pins[x]  & MCP_GP_BASE )  ==  MCP_GP_BASE ) ){
			xprintf("init_cols [%u] is ignoring pin:\t0x%X (%u)\n", x, pin, pin);
		} else {
			xprintf("init_cols [%u] *SETTING* pin:\t0x%X (%u)\n", x, pin, pin);
			_SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
			_SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
		}
	}
	_delay_ms(1000);
}


void matrix_init(void) {
	debug_enable = true;
	debug_matrix = true;
	_delay_ms(1000);
	xprintf("matrix_init\n");
	TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 400000));
	iota_mcp23017_init();
	init_cols();

  // iota_gfx_init();

//  pinMode(D3, PinDirectionInput);
//  iota_mcp23017_enable_interrupts();

	matrix_power_up();
}



bool matrix_is_on(uint8_t row, uint8_t col) {
  xprintf("matrix is on row:\n\t%08b (%u)\ncol:\n\t%08b (%u)", row, row, col, col);
  return (matrix[row] & ((matrix_row_t)1 << col));
}

matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

// static bool read_cols_on_row(matrix_row_t current_matrix[],
//                              uint8_t current_row) {
//   // Store last value of row prior to reading
//   matrix_row_t last_row_value = current_matrix[current_row];

//   // Clear data in matrix row
//   current_matrix[current_row] = 0;

//   // Select row and wait for row selection to stabilize
//   select_row(current_row);
//   _delay_us(30);

//   current_matrix[current_row] = iota_mcp23017_read();

//   unselect_row(current_row);

//   return last_row_value != current_matrix[current_row];
// }

// #define CONDITION current_row==0
#define CONDITION false
#define CONDITIONAL_XPRINTF(format,...) if(CONDITION){xprintf(format,__VA_ARGS__);}

uint16_t counter_since_zero			= 0;	// since last 0 (keysprocessed)
uint16_t counter_cycle					= 0;	// rows processed
uint16_t counter_keysprocessed	= 0;	// total keys processed (=cycles*#ofcols)
uint16_t counter_lastchange			= 0;	// since last change in keymap (measured in cycles)
bool		 trigger								= false;
bool		 trigger2								= true;
bool		 trigger3								= true;
#define MAX_COUNTER 9


static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row){
	// Store last value of row prior to reading
	matrix_row_t last_row_value = current_matrix[current_row];
	matrix_row_t mcp_pin_result;

	// if (current_row !=0){
	// 	return false;
	// }

	// Select row and wait for row selecton to stabilize
	select_row(current_row);
	// select_row(0);
	wait_us(30);

	matrix_row_t original_mcp_pin_result = 0;
	while ( original_mcp_pin_result == 0 ){
		original_mcp_pin_result     = iota_mcp23017_read();
		if (original_mcp_pin_result == 0){
			xprintf("\n****!!!!**** ERROR MCP returned all zeroes ****!!!!****\n");
			if (!iota_mcp23017_init()) {
				xprintf("\n****!!!!**** ERROR MCP makeready failed ****!!!!****\n");
			}
		}
		

	}
	if( MATRIX_COLS > 16){
		 mcp_pin_result = ( original_mcp_pin_result << ( MATRIX_COLS - 16 ) ) | ((matrix_row_t) pow( 2 , (MATRIX_COLS-16)) - 1) ;
	}
	// uint16_t mcp_pin_result = 0xffff;
	// while( mcp_pin_result  == 0xffff ){
		// mcp_pin_result = iota_mcp23017_read();
		// print(".");
	// }

	// if (mcp_pin_result != 0xffff){
	// 	xprintf("mcp_pin_result:\t%016b\n",mcp_pin_result);
	// 	print_bin32(mcp_pin_result);
	// }
	
	// xprintf("\nread_cols_on_row is on row:\n\t%08b (%u)\n", current_row, current_row);

	// Clear data in matrix row
	current_matrix[current_row] = (matrix_row_t)0;
	// Select row and wait for row selecton to stabilize
	// xprintf("test");

	// For each col...
	for(uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {

		// if ( (col_pins[col_index] >> 4) == (MCP_GP_BASE >> 4) ){
		// check if the pin is from the mcp
		if ( ( (col_pins[col_index]  & MCP_GP_BASE )  ==  MCP_GP_BASE ) ){
		
		// CONDITIONAL_XPRINTF("\nMCP->read_cols_on_row current scanning row:\t%08b (%u)\n", current_row, current_row);
		// CONDITIONAL_XPRINTF("\tcurrent       : %016b\n", current_matrix[current_row]);
		// CONDITIONAL_XPRINTF("\tmcp_bin_result: %016b\n", mcp_pin_result);
		

		// // read pin from MCP23017
		// CONDITIONAL_XPRINTF("\tmcp_pin_result >> ( col_index ) = %016b\n", ( mcp_pin_result >> ( col_index ) ) );
		if ( trigger3 | (( original_mcp_pin_result & 0xffff) != 0xffff)){
			print("\n----->\n");
			// matrix_row_t mcp_mod = ( ( ( mcp_pin_result << (col_index - 16) ) >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index);
			// matrix_row_t mcp_mod = ( ~( ( mcp_pin_result << (col_index > 15 ? col_index - 15 : 0) ) >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index);
			matrix_row_t mcp_mod = ( ( mcp_pin_result >> ( (col_pins[col_index]&0xF) + ( MATRIX_COLS - 16 ) ) ) & 1 ) ? 0 : ( ROW_SHIFTER << (col_index));
			current_matrix[current_row] |= mcp_mod;
			// current_matrix[current_row] |= (mcp_pin_result & (matrix_row_t)( 1 << (matrix_row_t)col_index ));
			xprintf("col_pins[col_index]&0xF):\t0x%X (%u)\t", (col_pins[col_index]&0xF), (col_pins[col_index]&0xF));
			print_bin16((col_pins[col_index]&0xFF));
			print("\n");
			xprintf("( mcp_pin_result >> ( col_pins[col_index] - 1 + ( MATRIX_COLS - 16 ) ) ):\t");
			print_bin32(( mcp_pin_result >> ( (col_pins[col_index]&0xFF) - 1 + ( MATRIX_COLS - 16 ) ) ));
			print("\n");
			xprintf("( mcp_pin_result >> ( col_pins[col_index] - 1 + ( MATRIX_COLS - 16 ) ) ) & 1:\t");
			print_bin32(( mcp_pin_result >> ( (col_pins[col_index]&0xFF) - 1 + ( MATRIX_COLS - 16 ) ) )&1);
			xprintf("\nafter MCP matrix update[col=%u]:\t", col_index);
			print_bin32(current_matrix[current_row]);
			xprintf("\ncol_pins[col_index]:\t0x%X\t(%u)\t", col_pins[col_index], col_pins[col_index], col_pins[col_index]);
			print_bin16(col_pins[col_index]);
			print("\n\toriginal_mcp_pin_result #16:\t                ");
			print_bin16(original_mcp_pin_result);
			print("\n\toriginal_mcp_pin_result #32:\t");
			print_bin32(original_mcp_pin_result);
			print("\n\tmcp_pin_result #16:\t\t                ");
			print_bin16(mcp_pin_result);
			print("\n\tmcp_pin_result #32:\t\t");
			print_bin32(mcp_pin_result);
			print("\n\tmcp_pin_result modified #16:\t                ");
			// print_bin16(   ( ( mcp_pin_result >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index)  );
			print_bin16(   mcp_mod );
			print("\n\tmcp_pin_result modified #32:\t");
			// print_bin32(   ( ( mcp_pin_result >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index)  );
			print_bin32( mcp_mod);
			print("\n<-----\n");

		}
		// CONDITIONAL_XPRINTF("\tend           : %016b\n", current_matrix[current_row]);

			// if (mcp_pin_result != 0xffff and counter_cycle < MAX_COUNTER){
			// //   xprintf("\nmcp_pin_read is non-Zero @row=%d col=%d with value %016b\n", current_row, col_index, mcp_pin_result);
			// 	xprintf("bit set by mcp_pin_result:\t%016b\n", ( ( mcp_pin_result >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index));
			// 	print_bin16(   ( ( mcp_pin_result >> ( col_index ) ) & 1 ) ? 0 : ( ROW_SHIFTER << col_index)  );
			// //  _delay_ms(10000);
			// }

		} else {
		
			// Read pin from adafruit
			// Select the col pin to read (active low)
			uint8_t pin = col_pins[col_index];
			// uint8_t pin_state = (_SFR_IO8(pin >> 4) & _BV(pin & 0xF));

			uint8_t sfr_io8   = _SFR_IO8(pin >> 4);
			// bv gets the bitmask for a single pin, defined by the last 4 bits of its ioaddr (ie. 0x3b becomes b or 1011)
			uint8_t bv        = _BV(pin & 0xF);
			uint8_t pin_state = (sfr_io8 & bv);

			if ( pin_state && counter_cycle < MAX_COUNTER && trigger2 ){
				xprintf("\n(%u)LOCAL(feather)->read_cols_on_row current scanning row:\t%08b (%u)\n", counter_cycle, current_row, current_row);
				xprintf("\tcol_index:\t%u (0x%X)\n", col_index, col_index);
				xprintf("\tpin: %08b (%u)(0x%X)", pin, pin, pin);
				xprintf("\tpin_state:\t%08b", pin_state);
				print("\t#bin8:\t");
				print_bin8(pin_state);
				print("\n");
				xprintf("\tpin & 0xF:\t%08b (%u)(0x%X)\n", (pin & 0xF), (pin & 0xF), (pin & 0xF));
				xprintf("\tsfr_io8:\t%08b (%u)(0x%X)\n", sfr_io8, sfr_io8, sfr_io8);
				xprintf("\tbv:\t%08b (%u)(0x%X)\n", bv, bv, bv);
				xprintf("\tcurrent_matrix[%u]:\t", current_row);
				print_bin32(current_matrix[current_row]);
				xprintf("(%lu)\n", current_matrix[current_row]);
				trigger=true;
			}
			if (counter_cycle > MAX_COUNTER){
				trigger2=false;
			}


			// Populate the matrix row with the state of the col pin
			current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);

			if ( pin_state && trigger2 ){
				print("after matrix update->\n");
				print("\tcurrent_matrix[current_row]/32:\t");
				print_bin32(current_matrix[current_row]);
				print("\n");
			}

			// if ( pin_state > 0 ){
			//   xprintf("\n****\npin_state is non-Zero @row=%d col=%d with value %016b\n****\n", current_row, col_index, pin_state);
			//  _delay_ms(5000);
			// }
			if ( pin_state == 0 ){
			//   xprintf("passed iterations:\t%0u\n", counter);
				xprintf("\n****\npin_state is Zero on pin=0x%X @row=%d col=%d with value %08b (%u)", pin, current_row, col_index, pin_state, pin_state );
				print("\t#bin8: \t");
				print_bin8(pin_state);
				print("\n****\n");

				xprintf("\tpin & 0xF:\t%08b (%u)(0x%X)\n", (pin & 0xF), (pin & 0xF), (pin & 0xF));
				xprintf("\tsfr_io8:\t%08b (%u)(0x%X)\n", sfr_io8, sfr_io8, sfr_io8);
				xprintf("\tbv:\t%08b (%u)(0x%X)\n", bv, bv, bv);
				xprintf("\t(current_matrix[%u]\t %20b (%u)\n", current_row, current_matrix[current_row], current_matrix[current_row]);
				counter_since_zero = 0;
				trigger=true;
			// _delay_ms(1000);
			} else {
				counter_since_zero++;
			}
		}
		counter_keysprocessed++;
	}

	// Unselect row
	unselect_row(current_row);


	trigger3 = false;

	// _delay_ms(5000);
	if ( ( last_row_value != current_matrix[current_row] ) && trigger ){

		xprintf("---->\ncycles since last change:\t%0u\n", ( counter_cycle - counter_lastchange) );
		xprintf("last_row_value != current_matrix[current_row]) %019b (%u) #bin32:",(last_row_value != current_matrix[current_row]), (last_row_value != current_matrix[current_row]) );
		print_bin32((last_row_value != current_matrix[current_row]) );
		xprintf("\n\tlast_row_value:\t\t#19:%019b (%u)\n", last_row_value, last_row_value);
		print("\t\tbin32:\t\t");
		print_bin32(last_row_value);
		print("\n\t\treverse32:\t");
		print_bin_reverse32(last_row_value);
		print("\n\t\tval reverse32:\t");
		print_val_bin_reverse32(last_row_value);

		xprintf("\tcurrent_matrix[%u]\t%019b (%u)\n", current_row, current_matrix[current_row], current_matrix[current_row]);
		print("\t\tbin32:\t\t");
		print_bin32(current_matrix[current_row]);
		print("\n\t\treverse32:\t");
		print_bin_reverse32(current_matrix[current_row]);
		print("\n\t\tval reverse32:\t");
		print_val_bin_reverse32(current_matrix[current_row]);
		print("\n<----\n");
		counter_lastchange = counter_cycle;
		trigger=false;

		// _delay_ms(3000);
	}
	counter_cycle++;

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
   xprintf("matrix scan frequency: ");
	pdec(scan_count);
   xprintf("\n");

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
  // iota_gfx_task();

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
			if (USB_DeviceState == DEVICE_STATE_Suspended && USB_Device_RemoteWakeupEnabled) {
				USB_Device_SendRemoteWakeup();
			}
			break;
		}
  }

  matrix_scan_quantum();
  return 1;
}

void matrix_print(void) {
	xprintf("\nr/c 0123456789ABCDEF\n");

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
		phex(row);
		xprintf(": ");
		print_bin_reverse16(matrix_get_row(row));
		xprintf("\n");
  }
}
