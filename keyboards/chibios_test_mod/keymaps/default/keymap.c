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

#include "chibios_test.h"
#include "action_layer.h"

// const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     {{KC_CAPS}}, // test with KC_CAPS, KC_A, RESET
// };


enum layer_id {
  BASE = 0,
  MDIA,
  FUNC
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = KEYMAP(
 /*,----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+. */
 /*  00 , 01 , 02 , 03 , 04 , 05 , 06 , 07 , 08 , 09 , 0A , 0B , 0C , 0D , 0E , 0F , 10 , 11 , 12 , */
    GRV , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  , 0  ,MINS,EQL , E  , E  , E  , E  , E  , E  ,
 /*,----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+. */
    GRV , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  , 0  ,MINS,EQL ,  BSPC   , E  , E  , E  , E  ,
 /*|----`----`----`----`----`----`----`----`----`----`----`----`----`--------| */
    TAB      , Q  , W  , E  , R  , T  , Y  , U  , I  , O  , P  ,LBRC,RBRC,BSLS, E  , E  , E  , E  ,
 /*|------`----`----`----`----`----`----`----`----`----`----`----`----`------| */
    NAV_ESC  , A  , S  , D  , F  , G  , H  , J  , K  , L  ,SCLN,QUOT,  ENTER  , E  , E  , E  , E  ,
 /*|-------`----`----`----`----`----`----`----`----`----`----`----`----------| */
    LSPO     , Z  , X  , C  , V  , B  , N  , M  ,COMM,DOT ,SLSH,   RSPC  , E  , E  , E  , E  , E  ,
 /*|---------`----`----`----`----`----`----`----`----`----`----`-------------. */
    LCTL , LGUI ,LALT ,            SPACE             ,  X1,RALT,RCTL, X1 , E  , E  , E  , E  , E
 /*`-----+-----+-----+------------------------------+------+-----+-----' ANY   */
  ),
};
