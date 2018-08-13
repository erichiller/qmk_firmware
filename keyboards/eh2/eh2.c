#include "eh2.h"
#include "print.h"

void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up

	matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

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
	return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
	// put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

	led_set_user(usb_led);
}
