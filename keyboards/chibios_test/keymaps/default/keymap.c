/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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

#define ____ KC_NO

#include "quantum.h"
#include "chibios_test.h"
#include "eh3.h"
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
      //   1    |    2    |    3    |    4    |    5    |    6    |    7    |    8    |    9    |    10   |    11   |    12   |    13   |    14   |    15   |    16   |    17   |    18   |    19   |
      //   0    |    1    |    2    |    3    |    4    |    5    |    6    |    7    |    8    |    9    |    A    |    B    |    C    |    D    |    E    |    F    |    10   |    11   |    12   |
    {
      
      { KC_A    , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , KC_EQL  , KC_COMM , KC_W    , KC_X    , KC_Y    , KC_Z    , KC_ENT  , },
      { KC_B    , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , KC_EQL  , ____    , KC_COMM , KC_W    , KC_X    , KC_Y    , KC_ENT  , },
      { KC_C    , ____    , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , KC_EQL  , KC_COMM , KC_W    , KC_X    , KC_Y    , KC_ENT  , },
      { KC_D    , ____    , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , ____    , KC_EQL  , KC_COMM , KC_W    , KC_X    , KC_ENT  , },
      { KC_E    , ____    , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , ____    , KC_MINS , KC_EQL  , KC_COMM , KC_W    , KC_X    , KC_ENT  , },
      { KC_F    , KC_1    , KC_2    , ____    , ____    , ____    , KC_SPC  , ____    , ____    , ____    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_ENT  , },
      // { KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   , KC_F6   , KC_F7   , KC_F8   , KC_F9   , KC_F10  , KC_F11  , },
      // { KC_GRV  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , KC_EQL  , },
      // { KC_TAB  , KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , KC_LBRC , KC_RBRC , },
      // { KC_CAPS , ____    , KC_A    , KC_S    , KC_D    , KC_F    , KC_G    , KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN , KC_QUOT , },
      // { KC_LSFT , ____    , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    , KC_N    , KC_M    , KC_N    , KC_M    , KC_COMM , KC_DOT  , },
      // { KC_LCTL , KC_LGUI , KC_LALT , ____    , ____    , ____    , KC_SPACE, ____    , ____    , ____    , KC_APP  , KC_LGUI , KC_LEAD , } // KC_LEFT , KC_DOWN , KC_RIGHT , KC_P0    , KC_PDOT , KC_PENT }
    }, // test with KC_CAPS, KC_A, RESET
};


// Each layer gets a name for readability, which is then used in the keymap matrix below.
enum layer_id {
  BASE = 0,
  MDIA,
  FUNC
};


// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//       [BASE] = KEYMAP(
//     /*,----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+. */
//     /*  00  , 01  , 02  , 03  , 04  , 05  , 06  , 07  , 08  , 09  , 0A  , 0B  , 0C  , 0D  , 0E  , 0F  , 10  , 11  , 12  , */
//       ESC , F1 , F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 , F10, F11, F12,PSCR,PAUS, DEL,PGUP,HOME, END,
//     /*,----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+. */
//       GRV , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  , 0  ,MINS, EQL,   BSPC  ,NLCK,PGDN, INS,PSLS,
//     /*|----`----`----`----`----`----`----`----`----`----`----`----`----`--------| */
//       TAB   , Q  , W  , E  , R  , T  , Y  , U  , I  , O  , P  ,LBRC,RBRC,  BSLS , P7 , P8 , P9 ,PAST,
//     /*|------`----`----`----`----`----`----`----`----`----`----`----`----`------| */
//       CAPS   , A  , S  , D  , F  , G  , H  , J  , K  , L  ,SCLN,QUOT,   ENTER   , P4 , P5 , P6 ,PMNS,
//     /*|-------`----`----`----`----`----`----`----`----`----`----`----`----------| */
//       LSPO     , Z  , X  , C  , V  , B  , N  , M  ,COMM, DOT,SLSH,   RSPC  , UP , P1 , P2 , P3 ,PPLS,
//     /*|---------`----`----`----`----`----`----`----`----`----`----`-------------. */
//       LCTL  , LGUI , LALT  ,           SPACE           , APP,RGUI,LEAD,LEFT,DOWN,RGHT, P0 ,PDOT,PENT
//     /*`-----+-----+-----+------------------------------+------+-----+-----' ANY   */
//       ),
// };