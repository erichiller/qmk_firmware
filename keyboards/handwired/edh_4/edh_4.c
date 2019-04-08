#include "edh_4.h"
#include "print.h"
#include "is31fl3731-simple.h"

void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up

	// matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	// matrix_scan_user();
}

// uint8_t last
uint8_t *process_led_matrix_output;
uint8_t last_led = 0;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware
	xprintf("---->\nkeycode:\t");
	print_bin16(keycode);
	xprintf(" [0x%X] (%u)\n", keycode, keycode);
	xprintf("key row:\t");
	print_bin8(record->event.key.row);
	xprintf(" [0x%X] (%u)\n", record->event.key.row, record->event.key.row);
	xprintf("key col:\t");
	print_bin8(record->event.key.col);
	xprintf(" [0x%X] (%u)\n", record->event.key.col, record->event.key.col);
	print("<----\n");

	process_led_matrix_output = get_process_led_matrix( keycode, record);
    led_matrix_effect_splash(keycode, record);
    xprintf("process_record_kb() process_led_matrix returned process_led_matrix_output=\n");
    for ( uint8_t i = 0; i < 8 ; i++){
        xprintf("process_led_matrix_output[%u]: ", i);
        print_bin8(process_led_matrix_output[i]);
        xprintf(" (%u)\n", process_led_matrix_output[i]);
    }

    // xprintf("process_record_kb |--->| process_led_matrix\n");
    // led_matrix_set_index_value_all(128);


	return process_record_user(keycode, record);
}


void led_set_kb(uint8_t usb_led) {
	// put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

	xprintf("---->\nUSB_LED:\t");
	xprintf(" [0x%X] (%u)\n", usb_led, usb_led);
	print("<----\n");
	led_set_user(usb_led);
}


void led_matrix_indicators_kb(void) {
    

    
    if( process_led_matrix_output[0] != last_led){
        xprintf("\n---->\nled_matrix_indicators_kb : CHANGE DETECTED ; was %u now %u \n", last_led, process_led_matrix_output[0]);
        xprintf("\n...as int %i now %i \n", (int)last_led, (int)process_led_matrix_output[0]);
        last_led = process_led_matrix_output[0];
        led_matrix_set_index_value((int)process_led_matrix_output[0], 200);

        xprintf("\ncolumn = %u\n", (process_led_matrix_output[0] / 16) );
        xprintf("\nrow = %u\n", ( process_led_matrix_output[0] % 16) );

        print("<----\n");
    }
    // led_matrix_set_index_value_all(200);
}

// void led_matrix_indicators_user(void) {
// }


#ifdef LED_DRIVER_COUNT
#pragma message "LED_MATRIX_ENABLE"
const is31_led g_is31_leds[LED_DRIVER_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *  driver
 *  |   LED address
 *  |   | */
    {0, C1_1},
    {0, C1_2},
    {0, C1_3},
    {0, C1_4},
    {0, C1_5},
    {0, C1_6},
    {0, C1_7},
    {0, C1_8},

    {0, C1_9},
    {0, C1_10},
    {0, C1_11},
    {0, C1_12},
    {0, C1_13},
    {0, C1_14},
    {0, C1_15},
    {0, C1_16},

    {0, C7_1},
    {0, C7_2},
    {0, C7_3},

/***** ROW 2 *****/
    {0, C2_1},
    {0, C2_2},
    {0, C2_3},
    {0, C2_4},
    {0, C2_5},
    {0, C2_6},
    {0, C2_7},
    {0, C2_8},

    {0, C2_9},
    {0, C2_10},
    {0, C2_11},
    {0, C2_12},
    {0, C2_13},
    {0, C2_14},
    {0, C2_15},
    {0, C2_16},

    {0, C7_4},
    {0, C7_5},
    {0, C7_6},

/***** ROW 3 *****/
    {0, C3_1},
    {0, C3_2},
    {0, C3_3},
    {0, C3_4},
    {0, C3_5},
    {0, C3_6},
    {0, C3_7},
    {0, C3_8},

    {0, C3_9},
    {0, C3_10},
    {0, C3_11},
    {0, C3_12},
    {0, C3_13},
    {0, C3_14},
    {0, C3_15},
    {0, C3_16},

    {0, C7_7},
    {0, C7_8},
    {0, C7_9},

/***** ROW 4 *****/

    {0, C4_1},
    {0, C4_2},
    {0, C4_3},
    {0, C4_4},
    {0, C4_5},
    {0, C4_6},
    {0, C4_7},
    {0, C4_8},

    {0, C4_9},
    {0, C4_10},
    {0, C4_11},
    {0, C4_12},
    {0, C4_13},
    {0, C4_14},
    {0, C4_15},
    {0, C4_16},

    {0, C7_10},
    {0, C7_11},
    {0, C7_12},

/***** ROW 5 *****/

    {0, C5_1},
    {0, C5_2},
    {0, C5_3},
    {0, C5_4},
    {0, C5_5},
    {0, C5_6},
    {0, C5_7},
    {0, C5_8},

    {0, C5_9},
    {0, C5_10},
    {0, C5_11},
    {0, C5_12},
    {0, C5_13},
    {0, C5_14},
    {0, C5_15},
    {0, C5_16},

    {0, C7_13},
    {0, C7_14},
    {0, C7_15},

/***** ROW 6 *****/

    {0, C6_1},
    {0, C6_2},
    {0, C6_3},
    {0, C6_4},
    {0, C6_5},
    {0, C6_6},
    {0, C6_7},
    {0, C6_8},

    {0, C6_9},
    {0, C6_10},
    {0, C6_11},
    {0, C6_12},
    {0, C6_13},
    {0, C6_14},
    {0, C6_15},
    {0, C6_16},

    
    {0, C8_1},
    {0, C8_2},
    {0, C8_3},

};


#define LED_MATRIX_X(col) (224 / ( MATRIX_COLS - 1 ) * col)


#define LED_MATRIX_Y(row) (64 / ( MATRIX_ROWS - 1 ) * row)

// x is between (inclusive) 0-224, and y is between (inclusive) 0-64
const led_matrix g_leds[LED_DRIVER_LED_COUNT] = {

    /*{row | col << 4}
      |            LED_ROW_COL(row, col)
      |             |            modifier
      |             |            | */
    {{0|(0<<3)},   {LED_MATRIX_X(0), 0}, 1},           // k00  KC_GESC
    {{0|(1<<3)},   {LED_MATRIX_X(1), 0}, 1},           // k00  KC_GESC
    {{0|(2<<3)},   {LED_MATRIX_X(2), 0}, 0},       // k01 KC_1
    {{0|(3<<3)},   {LED_MATRIX_X(3), 0}, 0},        // k02 KC_2
    {{0|(4<<3)},   {LED_MATRIX_X(4), 0}, 0},       // k03 KC_3
    {{0|(5<<3)},   {LED_MATRIX_X(5), 0}, 0},        // k04 KC_4
    {{0|(6<<3)},   {LED_MATRIX_X(6), 0}, 0},       // k05 KC_5
    {{0|(7<<3)},   {LED_MATRIX_X(7), 0}, 0},        // k06 KC_6
    {{0|(8<<3)},   {LED_MATRIX_X(8), 0}, 0},       // k07 KC_7
    {{0|(9<<3)},   {LED_MATRIX_X(9), 0}, 0},       // k50 KC_8
    {{0|(10<<3)},  {LED_MATRIX_X(10), 0}, 0},         // k51 KC_9
    {{0|(11<<3)},  {LED_MATRIX_X(11), 0}, 0},       // k52 KC_0
    {{0|(12<<3)},  {LED_MATRIX_X(12), 0}, 0},         // k53 KC_MINS
    {{0|(13<<3)},  {LED_MATRIX_X(13), 0}, 0},       // k54 KC_EQL
    {{0|(14<<3)},  {LED_MATRIX_X(14), 0}, 1},       // k55 KC_BSPC
    {{0|(15<<3)},  {LED_MATRIX_X(15), 0}, 1},         // k57 KC_PGUP

	/** EDH additions **/
	// {{6|(0<<3)},   {0, 0}, 1},
	{{0|(16<<3)},   {LED_MATRIX_X(16), 0}, 1},
	{{0|(17<<3)},   {LED_MATRIX_X(17), 0}, 1},
	{{0|(18<<3)},   {LED_MATRIX_X(18), 0}, 1},

    {{1|(0<<3)},   {3.6125, 16}, 1},     // k10 KC_TAB
    {{1|(1<<3)},   {21.675, 16}, 0},     // k11 KC_Q
    {{1|(2<<3)},   {36.125, 16}, 0},     // k12 KC_W
    {{1|(3<<3)},   {50.575, 16}, 0},     // k13 KC_E
    {{1|(4<<3)},   {65.025, 16}, 0},     // k14 KC_R
    {{1|(5<<3)},   {79.475, 16}, 0},     // k15 KC_T
    {{1|(6<<3)},   {93.925, 16}, 0},     // k16 KC_Y
    {{1|(7<<3)},   {108.375, 16}, 0},    // k17 KC_U
    {{1|(8<<3)},   {122.825, 16}, 0},    // k60 KC_I
    {{1|(9<<3)},   {137.275, 16}, 0},    // k61 KC_O
    {{1|(10<<3)},  {151.725, 16}, 0},    // k62 KC_P
    {{1|(11<<3)},  {166.175, 16}, 0},    // k63 KC_LBRC
    {{1|(12<<3)},  {180.625, 16}, 0},    // k64 KC_RBRC
    {{1|(13<<3)},  {198.6875, 16}, 1},   // k65 KC_BSLS
    {{1|(14<<3)},  {224, 16}, 1},        // k67 KC_PGDN

	/** EDH additions **/
	{{1|(15<<3)},   {65, 16}, 1},
	{{1|(16<<3)},   {80.45, 16}, 1},
	{{1|(17<<3)},   {93, 16}, 1},
	{{1|(18<<3)},   {108.375, 16}, 1},



    {{2|(0<<3)},   {5.41875, 32}, 1},    // k20 KC_CAPS
    {{2|(1<<3)},   {25.2875, 32}, 0},    // k21 KC_A
    {{2|(2<<3)},   {39.7375, 32}, 0},    // k22 KC_S
    {{2|(3<<3)},   {54.1875, 32}, 0},    // k23 KC_D
    {{2|(4<<3)},   {68.6375, 32}, 0},    // k24 KC_F
    {{2|(5<<3)},   {83.0875, 32}, 0},    // k25 KC_G
    {{2|(6<<3)},   {97.5375, 32}, 0},    // k26 KC_H
    {{2|(7<<3)},   {111.9875, 32}, 0},   // k27 KC_J
    {{2|(8<<3)},   {126.4375, 32}, 0},   // k70 KC_K
    {{2|(9<<3)},   {140.8875, 32}, 0},   // k71 KC_L
    {{2|(10<<3)},  {155.3375, 32}, 0},   // k72 KC_SCLN
    {{2|(11<<3)},  {169.7875, 32}, 0},   // k73 KC_QUOT
    {{2|(12<<3)},  {184.2375, 32}, 1},   // k75 KC_ENT

	
	/** EDH additions **/
	{{2|(13<<3)},   {65, 32}, 1},
	{{2|(14<<3)},   {80.45, 32}, 1},
	{{2|(15<<3)},   {93, 32}, 1},
	{{2|(16<<3)},   {108.375, 32}, 1},
	{{2|(17<<3)},   {93, 32}, 1},
	{{2|(18<<3)},   {108.375, 32}, 1},



    {{3|(0<<3)},   {16.25625, 48}, 1},   // k30 KC_LSFT
    {{3|(1<<3)},   {32.5125, 48}, 0},    // k32 KC_Z
    {{3|(2<<3)},   {46.9625, 48}, 0},    // k33 KC_X
    {{3|(3<<3)},   {61.4125, 48}, 0},    // k34 KC_C
    {{3|(4<<3)},   {75.8625, 48}, 0},    // k35 KC_V
    {{3|(5<<3)},   {90.3125, 48}, 0},    // k36 KC_B
    {{3|(6<<3)},   {104.7625, 48}, 0},   // k37 KC_N
    {{3|(7<<3)},   {119.2125, 48}, 0},   // k80 KC_M
    {{3|(8<<3)},   {133.6625, 48}, 0},   // k81 KC_COMM
    {{3|(9<<3)},   {148.1125, 48}, 0},   // k82 KC_DOT
    {{3|(10<<3)},  {162.5625, 48}, 0},   // k83 KC_SLSH
    {{3|(11<<3)},  {187.85, 48}, 1},     // k85 KC_RSFT
    {{3|(12<<3)},  {209.525, 48}, 1},    // k86 KC_UP


    /** EDH additions **/
    {{3|(13<<3)},   {0,     48}, 1},
    {{3|(14<<3)},   {14.45, 48}, 1},
    {{3|(15<<3)},   {28.9,  48}, 1},
    {{3|(16<<3)},   {43.35, 48}, 1},
    {{3|(17<<3)},   {65,    48}, 1},
    {{3|(18<<3)},   {80.45, 48}, 1},


    // 19 * 4 = 76 ...
    {{4|(0<<3)},   {9.03125, 64}, 1},    // k40 KC_LCTL
    {{4|(1<<3)},   {27.09375, 64}, 1},   // k41 KC_LGUI
    {{4|(2<<3)},   {45.15625, 64}, 1},   // k42 KC_LALT
    {{4|(3<<3)},   {59.45, 64}, 1},      // Unassociated between LALT and SPACE_2.75
    {{4|(4<<3)},   {73.9, 64}, 1},       // k45 KC_SPC SPACE_2.75
    {{4|(5<<3)},   {88.35, 64}, 1},      // k45 KC_SPC SPACE_6.25
    {{4|(6<<3)},   {102.8, 64}, 1},      // k46 KC_SPC SPACE_2.25
    {{4|(7<<3)},   {117.40625, 64}, 1},  // Unassociated between SPACE_2.25 and SPACE_2.75
    {{4|(8<<3)},   {135.46875, 64}, 1},  // k90 KC_RGUI
    {{4|(9<<3)},   {153.53125, 64}, 1},  // k92 KC_RALT
    {{4|(10<<3)},  {167.98125, 64}, 1},  // k93 MO(_FL)
    {{4|(11<<3)},  {186.04375, 64}, 1},  // k94 KC_RCTL
    {{4|(12<<3)},  {195.075, 64}, 1},    // k95 KC_LEFT
    {{4|(13<<3)},  {209.525, 64}, 1},    // k96 KC_DOWN
    {{4|(14<<3)},  {224, 64}, 1},        // k97 KC_RGHT


    /** EDH additions **/
    {{4|(15<<3)},   {0, 64}, 1},
    {{4|(16<<3)},   {14.45, 64}, 1},
    {{4|(17<<3)},   {28.9, 64}, 1},
    {{4|(18<<3)},   {43.35, 64}, 1},


    /** EDH , 6th row **/

    {{5|(0<<3)},   {9.03125, LED_MATRIX_Y(5) }, 1},    // k40 KC_LCTL
    {{5|(1<<3)},   {27.09375, LED_MATRIX_Y(5) }, 1},   // k41 KC_LGUI
    {{5|(2<<3)},   {45.15625, LED_MATRIX_Y(5) }, 1},   // k42 KC_LALT
    {{5|(3<<3)},   {59.45, LED_MATRIX_Y(5) }, 1},      // Unassociated between LALT and SPACE_2.75
    {{5|(4<<3)},   {73.9, LED_MATRIX_Y(5) }, 1},       // k45 KC_SPC SPACE_2.75
    {{5|(5<<3)},   {88.35, LED_MATRIX_Y(5) }, 1},      // k45 KC_SPC SPACE_6.25
    {{5|(6<<3)},   {102.8, LED_MATRIX_Y(5) }, 1},      // k46 KC_SPC SPACE_2.25
    {{5|(7<<3)},   {117.40625, LED_MATRIX_Y(5) }, 1},  // Unassociated between SPACE_2.25 and SPACE_2.75
    {{5|(8<<3)},   {135.46875, LED_MATRIX_Y(5) }, 1},  // k90 KC_RGUI
    {{5|(9<<3)},   {153.53125, LED_MATRIX_Y(5) }, 1},  // k92 KC_RALT
    {{5|(10<<3)},  {167.98125, LED_MATRIX_Y(5) }, 1},  // k93 MO(_FL)
    {{5|(11<<3)},  {186.04375, LED_MATRIX_Y(5) }, 1},  // k94 KC_RCTL
    {{5|(12<<3)},  {195.075, LED_MATRIX_Y(5) }, 1},    // k95 KC_LEFT
    {{5|(13<<3)},  {209.525, LED_MATRIX_Y(5) }, 1},    // k96 KC_DOWN
    {{5|(14<<3)},  {215, LED_MATRIX_Y(5) }, 1},        // k97 KC_RGHT
    {{5|(15<<3)},  {224, LED_MATRIX_Y(5) }, 1},        // k97 KC_RGHT

    
    {{5|(16<<3)},   {0, LED_MATRIX_Y(5) }, 0},
    {{5|(17<<3)},   {14.45, LED_MATRIX_Y(5) }, 0},
    {{5|(18<<3)},   {28.9, LED_MATRIX_Y(5) }, 0},


    
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{7|(18<<3)},   {0, LED_MATRIX_Y(5) }, 1},
    // {{116},   {0, LED_MATRIX_Y(5) }, 1},
    // {{117},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{118},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{119},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{120},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{121},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{122},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{123},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{124},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{125},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{126},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{127},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{128},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{129},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{130},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{131},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{132},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{133},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{134},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{135},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{136},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{137},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{138},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{139},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{140},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{141},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{142},   {14.45, LED_MATRIX_Y(5) }, 1},
    // {{143},   {14.45, LED_MATRIX_Y(5) }, 1},




};
#endif