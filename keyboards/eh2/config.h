/*
Copyright 2018 Eric D Hiller

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

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Eric Hiller
#define USBSTR_MANUFACTURER 'Q', 0, 'M', 0, 'K', 0

#define PRODUCT         eh2
#define USBSTR_PRODUCT 'e', 0, 'h', 0, '2'
#define DESCRIPTION     Feather Bluefruit 32u4 

// Turn off the mode leds on the BLE module
#define ADAFRUIT_BLE_ENABLE_MODE_LEDS 0

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 19
#define DIODE_DIRECTION COL2ROW

#undef NO_DEBUG
#undef NO_PRINT


// #define RGB_DI_PIN C7 // Pin 13 on the Feather board
// #define RGBLED_NUM 8
// #undef RGBLIGHT_ANIMATIONS

// #define BACKLIGHT_PIN B7
// #define BACKLIGHT_BREATHING
// #define BACKLIGHT_LEVELS 3


/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCING_DELAY 0

/* number of backlight levels */

/*
 * Magic Key Options
 *
 * Magic keys are hotkey commands that allow control over firmware functions of
 * the keyboard. They are best used in combination with the HID Listen program,
 * found here: https://www.pjrc.com/teensy/hid_listen.html
 *
 * The options below allow the magic key functionality to be changed. This is
 * useful if your keyboard/keypad is missing keys and you want magic key support.
 *
 */

/* key combination for magic key command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* control how magic key switches layers */
//#define MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS  true
//#define MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS  true
//#define MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM false

/* override magic key keymap */
//#define MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS
//#define MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS
//#define MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM
//#define MAGIC_KEY_HELP1          H
//#define MAGIC_KEY_HELP2          SLASH
//#define MAGIC_KEY_DEBUG          D
//#define MAGIC_KEY_DEBUG_MATRIX   X
//#define MAGIC_KEY_DEBUG_KBD      K
//#define MAGIC_KEY_DEBUG_MOUSE    M
//#define MAGIC_KEY_VERSION        V
//#define MAGIC_KEY_STATUS         S
//#define MAGIC_KEY_CONSOLE        C
//#define MAGIC_KEY_LAYER0_ALT1    ESC
//#define MAGIC_KEY_LAYER0_ALT2    GRAVE
//#define MAGIC_KEY_LAYER0         0
//#define MAGIC_KEY_LAYER1         1
//#define MAGIC_KEY_LAYER2         2
//#define MAGIC_KEY_LAYER3         3
//#define MAGIC_KEY_LAYER4         4
//#define MAGIC_KEY_LAYER5         5
//#define MAGIC_KEY_LAYER6         6
//#define MAGIC_KEY_LAYER7         7
//#define MAGIC_KEY_LAYER8         8
//#define MAGIC_KEY_LAYER9         9
//#define MAGIC_KEY_BOOTLOADER     PAUSE
//#define MAGIC_KEY_LOCK           CAPS
//#define MAGIC_KEY_EEPROM         E
//#define MAGIC_KEY_NKRO           N
//#define MAGIC_KEY_SLEEP_LED      Z

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#endif


#ifndef CONFIG_USER_H
#define CONFIG_USER_H

// place overrides here

/* We only use CTL_T style mod-tap keys, so we want the built-in
 * mod-tap-interrupt behavior to allow quickly typing CTRL-W arrow
 * sequences with a CTL_T(ESC) key */
// #undef IGNORE_MOD_TAP_INTERRUPT

/* We want our layer toggle tap to be a double tap */
// #define TAPPING_TOGGLE 2





#define ROW_SHIFTER ((matrix_row_t)1UL)

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

// #define GPA0 (MCP_GP_BASE + 0)
// #define GPA1 (MCP_GP_BASE + 1)
// #define GPA2 (MCP_GP_BASE + 2)
// #define GPA3 (MCP_GP_BASE + 3)
// #define GPA4 (MCP_GP_BASE + 4)
// #define GPA5 (MCP_GP_BASE + 5)
// #define GPA6 (MCP_GP_BASE + 6)
// #define GPA7 (MCP_GP_BASE + 7)
// #define GPB0 (MCP_GP_BASE + 0x08 + 0)
// #define GPB1 (MCP_GP_BASE + 0x08 + 1)
// #define GPB2 (MCP_GP_BASE + 0x08 + 2)
// #define GPB3 (MCP_GP_BASE + 0x08 + 3)
// #define GPB4 (MCP_GP_BASE + 0x08 + 4)
// #define GPB5 (MCP_GP_BASE + 0x08 + 5)
// #define GPB6 (MCP_GP_BASE + 0x08 + 6)
// #define GPB7 (MCP_GP_BASE + 0x08 + 7)

#define ANL0  F7   // 0xF7
#define ANL1  F6   // 0xF6
#define ANL2  F5   // 0xF5
#define ANL3  F4   // 0xF4
#define ANL4  F1   // 0xF1
#define ANL5  F0   // 0xF0
#define ANL7  D7   // 0x97
#define ANL9  B5   // 0x35
#define ANL10 B6   // 0x36
#define ANL11 D6   // 0x96


// #define MATRIX_ROW_PINS { ANL0 , ANL1 , ANL2 , ANL3 , ANL4 , ANL5 }
/**
 * pins of the rows, from top to bottom
 */
// #define MATRIX_ROW_PINS { ANL5 , ANL4 , ANL3 , ANL2 , ANL1 , ANL0 }
#define MATRIX_ROW_PINS { F0   , F1   , F4   , F5   , F6   , F7 }

/**
 * pins of the columns, from left to right
 */
//                        0x7A   0x7B   0x79   0x74   0x75   0x77   0x76   0x97   0x35   0x36   0x7C   0x96   0x70   0x7F   0x71   0x7E   0x72   0x7D   0x73
//                        C=0    C=1    C=2    C=3    C=4    C=5    C=6    C=7    C=8    C=9    C=10   C=11   C=12   C=13   C=14   C=15   C=16   C=17   C=18 //
// #define MATRIX_COL_PINS { B6   , B5   , D7   , GPB0 , GPB1 , GPB2 , GPB3 , GPB4 , GPB5 , GPB6 , GPB7 , GPA7 , GPA6 , GPA5 , GPA4 , GPA3 , GPA2 , GPA1 , GPA0 }
// #define MATRIX_COL_PINS { B6   , B5   , D7   , GPB0 , GPB1 , GPB2 , GPB3 , GPB4 , GPB5 , GPB6 , GPB7 , GPA0 , GPA1 , GPA2 , GPA3 , GPA4 , GPA5 , GPA6 , GPA7 }
#define MATRIX_COL_PINS {    B6   , B5   , D7 , GPA7 , GPA6 , GPA5 , GPA4 , GPA3 , GPA2 , GPA1 , GPA0 , GPB7 , GPB6 , GPB5 , GPB4 , GPB3 , GPB2 , GPB1 , GPB0 }

// #define MATRIX_COL_PINS { ANL10, ANL9 , ANL7 , GPA7 , GPA6 , GPA5 , GPA4 , GPA3 , GPA2 , GPA1 , GPA0 , GPB7 , GPB6 , GPA5 , GPB4 , GPB3 , GPB2 , GPB1 , GPB0 }
// #define MATRIX_COL_PINS { GPA2 , GPA3 , GPA1 , GPB4 , GPB5 , GPB7 , GPB6 , D7   , B5   , B6   , GPA4 , D6   , GPB0 , GPA7 , GPB1 , GPA6 , GPB2 , GPA5 , GPB3 }



#endif
