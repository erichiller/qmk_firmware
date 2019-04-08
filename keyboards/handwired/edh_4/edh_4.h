
#pragma once
#include "quantum.h"

#ifdef __AVR__
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

// extern bool iota_gfx_init(void);
// extern void iota_gfx_task(void);
// extern bool iota_gfx_off(void);
// extern bool iota_gfx_on(void);
// extern void iota_gfx_flush(void);
// extern void iota_gfx_write_char(uint8_t c);
// extern void iota_gfx_write(const char *data);
// extern void iota_gfx_write_P(const char *data);
// extern void iota_gfx_clear_screen(void);

// bool iota_mcp23017_init(void);
// bool iota_mcp23017_make_ready(void);
// uint16_t iota_mcp23017_read(void);
// bool iota_mcp23017_enable_interrupts(void);


#define ROW_SHIFTER ((matrix_row_t)1UL)


#define _QWERTY 0
#define _FN1 1
#define _FN2 2
#define KC_ KC_TRNS
#define KC_X0 LT(_FN2, KC_GRV)
#define KC_X1 MO(_FN1)
#define KC_NAV_ESC LT(_FN1, KC_ESC)
#define KC_GUI MAGIC_UNNO_GUI


#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D, K0E, K0F, K10, K11, K12, \
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C,      K2E, K2F, K30, K31, K32, \
    K40,      K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, K4C, K4D, K4E, K4F, K50, K51, K52, \
    K60,      K62, K63, K64, K65, K66, K67, K68, K69, K6A, K6B, K6C,      K6E, K6F, K70, K71, K72, \
    K80,      K82, K83, K84, K85, K86, K87, K88, K89, K8A, K8B,      K8D, K8E, K8F, K90, K91, K92, \
    KA0, KA1, KA2,           KA6,                     KAA, KAB, KAC, KAD, KAE, KAF, KB0, KB1, KB2 \
    ) { \
  {  KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E, KC_##K0F, KC_##K10, KC_##K11, KC_##K12 },  \
  {  KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_NO,    KC_##K2E, KC_##K2F, KC_##K30, KC_##K31, KC_##K32 },  \
  {  KC_##K40, KC_NO   , KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47, KC_##K48, KC_##K49, KC_##K4A, KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E, KC_##K4F, KC_##K50, KC_##K51, KC_##K52 },  \
  {  KC_##K60, KC_NO   , KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67, KC_##K68, KC_##K69, KC_##K6A, KC_##K6B, KC_##K6A, KC_NO   , KC_##K6E, KC_##K6F, KC_##K70, KC_##K71, KC_##K72 },  \
  {  KC_##K80, KC_NO   , KC_##K82, KC_##K83, KC_##K84, KC_##K85, KC_##K86, KC_##K87, KC_##K88, KC_##K89, KC_##K8A, KC_##K8B, KC_NO   , KC_##K8D, KC_##K8E, KC_##K8F, KC_##K90, KC_##K91, KC_##K92 },  \
  {  KC_##KA0, KC_##KA1, KC_##KA2, KC_NO   , KC_NO   , KC_NO   , KC_##KA6, KC_NO   , KC_NO   , KC_NO   , KC_##KAA, KC_##KAB, KC_##KAC, KC_##KAD, KC_##KAE, KC_##KAF, KC_##KB0, KC_##KB1, KC_##KB2 }  \
}
