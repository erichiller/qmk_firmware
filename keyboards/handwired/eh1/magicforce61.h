#ifndef MAGICFORCE61_H
#define MAGICFORCE61_H

#include "quantum.h"

#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D, \
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, \
    K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B,   K4D, \
    K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K6A,        K6D, \
    K80, K81, K82,           K85,                 K89,  K8A,  K8C,   K8D \
    ) { \
  {  KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E, KC_##K0F, KC_##K10, KC_##K11  },  \
  {  KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E, KC_##K0F, KC_##K10, KC_##K11  },  \
  {  KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D  },  \
  {  KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47, KC_##K48, KC_##K49, KC_##K4A, KC_##K4B, KC_NO,    KC_##K4D  },  \
  {  KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67, KC_##K68, KC_##K69, KC_##K6A, KC_NO,    KC_NO,    KC_##K6D  },  \
  {  KC_##K80, KC_##K81, KC_##K82, KC_NO,    KC_NO,    KC_##K85, KC_NO,    KC_NO,    KC_NO,    KC_##K89, KC_##K8A, KC_NO,    KC_##K8C, KC_##K8D  },  \
}

#endif
