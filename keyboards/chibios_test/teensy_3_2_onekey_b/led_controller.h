#include "config.h"

#ifdef LED_MATRIX
#ifndef _LED_CONTROLLER_H_
#define _LED_CONTROLLER_H_

#include "ch.h"

/* =========================
 *  communication functions
 * ========================= */

msg_t is31_write_data(uint8_t page, uint8_t *buffer, uint8_t size);
msg_t is31_write_register(uint8_t page, uint8_t reg, uint8_t data);
msg_t is31_read_register(uint8_t page, uint8_t reg, uint8_t *result);

/* ============================
 *  init functions/definitions
 * ============================*/

void led_controller_init(void);

#define CAPS_LOCK_LED_ADDRESS 46 //pin matrix location
#define NUM_LOCK_LED_ADDRESS 85

/* =============================
 * IS31 chip related definitions
 * ============================= */

#define IS31_ADDR_DEFAULT 0x74

#define IS31_REG_CONFIG  0x00
// // bits in reg
// #define IS31_REG_CONFIG_PICTUREMODE 0x00
// #define IS31_REG_CONFIG_AUTOPLAYMODE 0x08
// #define IS31_REG_CONFIG_AUDIOPLAYMODE 0x18
// // D2:D0 bits are starting frame for autoplay mode

// #define IS31_REG_PICTDISP 0x01 // D2:D0 frame select for picture mode

// #define IS31_REG_AUTOPLAYCTRL1 0x02
// // D6:D4 number of loops (000=infty)
// // D2:D0 number of frames to be used

// #define IS31_REG_AUTOPLAYCTRL2 0x03 // D5:D0 delay time (*11ms)

// #define IS31_REG_DISPLAYOPT 0x05
// #define IS31_REG_DISPLAYOPT_INTENSITY_SAME 0x20 // same intensity for all frames
// #define IS31_REG_DISPLAYOPT_BLINK_ENABLE 0x08
// // D2:D0 bits blink period time (*0.27s)

// #define IS31_REG_AUDIOSYNC 0x06
// #define IS31_REG_AUDIOSYNC_ENABLE 0x1

// #define IS31_REG_FRAMESTATE 0x07

// #define IS31_REG_BREATHCTRL1 0x08
// // D6:D4 fade out time (26ms*2^i)
// // D2:D0 fade in time (26ms*2^i)

// #define IS31_REG_BREATHCTRL2 0x09
// #define IS31_REG_BREATHCTRL2_ENABLE 0x10
// // D2:D0 extinguish time (3.5ms*2^i)

// #define IS31_REG_SHUTDOWN 0x0A
// #define IS31_REG_SHUTDOWN_OFF 0x1
// #define IS31_REG_SHUTDOWN_ON 0x0

// #define IS31_REG_AGCCTRL 0x0B
// #define IS31_REG_ADCRATE 0x0C

// #define IS31_COMMANDREGISTER 0xFD
// #define IS31_FUNCTIONREG 0x0B    // helpfully called 'page nine'

// #define IS31_TIMEOUT 10000 // needs to be long enough to write a whole page

/* ========================================
 * LED Thread related items
 * ========================================*/

extern mailbox_t led_mailbox;

void set_led_bit (uint8_t page, uint8_t *led_control_reg, uint8_t led_addr, uint8_t action);
void set_lock_leds (uint8_t led_addr, uint8_t led_action, uint8_t page);
void write_led_byte (uint8_t page, uint8_t row, uint8_t led_byte);
void write_led_page (uint8_t page, uint8_t *led_array, uint8_t led_count);

// constants for signaling the LED controller thread
enum led_msg_t {
    KEY_LIGHT,
    SET_FULL_ROW,
    OFF_LED,
    ON_LED,
    TOGGLE_LED,
    BLINK_OFF_LED,
    BLINK_ON_LED,
    BLINK_TOGGLE_LED,
    TOGGLE_ALL,
    TOGGLE_BACKLIGHT,
    DISPLAY_PAGE,
    RESET_PAGE,
    TOGGLE_NUM_LOCK,
    TOGGLE_CAPS_LOCK,
    TOGGLE_BREATH,
    STEP_BRIGHTNESS
};

#endif /* _LED_CONTROLLER_H_ */

#endif /* defined? LED_MATRIX */