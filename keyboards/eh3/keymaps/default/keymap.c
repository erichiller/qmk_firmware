#include "eh3.h"
#include "action_layer.h"
#ifdef MOUSEKEY_ENABLE
#include "mousekey.h"
#endif


enum layer_id {
  BASE = 0,
  MDIA,
  FUNC
};


// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
// xprintf("HELLO! from matrix_init_user ");
// matrix_print();
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

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
