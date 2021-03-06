#include "eh1.h"

#define _QWERTY 0
#define _FN1 1
#define _FN2 2
#define KC_ KC_TRNS
#define KC_X0 LT(_FN2, KC_GRV)
#define KC_X1 MO(_FN1)
#define KC_NAV_ESC LT(_FN1, KC_ESC)
#define KC_GUI MAGIC_UNNO_GUI

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = KEYMAP(
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

//   [_FN1] = KEYMAP(
//  /*,----+----+----+----+----+----+----+----+----+----+----+----+----+--------. */
//     GRV , F1 , F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 ,F10 ,F11 ,F12 ,   BSPC ,           , E, E, E, E,
//  /*|esc-`-1--`-2--`-3--`-4--`-5--`-6--`-7--`-8--`-9--`-0--`mnus`plus`--bksp--| */
//           ,    , UP ,    ,    ,    ,    ,    ,INS ,PSCR,    ,    ,    ,      ,           , E, E, E, E,
//  /*|tab---`-q--`-w--`-e--`-r--`-t--`-y--`-u--`-i--`-o--`-p--`-{--`-}--`--|---| */
//            ,LEFT,DOWN,RGHT,    ,    ,    , PGDN , PGUP ,    ,    ,    ,      ,           , E, E, E, E,
//  /*|caps---`-a--`-s--`-d--`-f--`-g--`-h--`-j--`-k--`-l--`-;--`-'--`----enter-| */
//              ,    ,    ,    ,    ,    ,VOLD,VOLU,MUTE,    ,    ,             ,           , E, E, E, E,
//  /*|shift----`-z--`-x--`-c--`-v--`-b--`-n--`-m--`-,--`-.--`-/--`-------shift-. */
//          ,     ,     ,                              ,      ,     ,     , MPLY,           , E, E, E, E
//  /*`ctrl-+-gui-+-alt-+----------space---------------+-fn---+-alt-+ctrl-' ANY   */
//   ),

//   [_FN2] = KEYMAP(
//  /*,----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+. */
//     GRV , F1 , F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 ,F10 ,F11 ,F12 ,   BSPC ,           , E, E, E, E,
//  /*,----+----+----+----+----+----+----+----+----+----+----+----+----+---------+----+----+----+----+. */
//     GRV , F1 , F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 ,F10 ,F11 ,F12 ,   BSPC ,           , E, E, E, E,
//  /*|esc-`-1--`-2--`-3--`-4--`-5--`-6--`-7--`-8--`-9--`-0--`mnus`plus`--bksp---+----+----+----+----+| */
//           ,    ,    ,    ,    ,    ,    , 7  , 8  , 9  ,    ,    ,    ,      ,           , E, E, E, E,
//  /*|tab---`-q--`-w--`-e--`-r--`-t--`-y--`-u--`-i--`-o--`-p--`-{--`-}--`--|----+----+----+----+----+| */
//            ,    ,    ,    ,    ,    ,    , 4  , 5  , 6  ,    ,    ,          ,           , E, E, E, E,
//  /*|caps---`-a--`-s--`-d--`-f--`-g--`-h--`-j--`-k--`-l--`-;--`-'--`----enter--+----+----+----+----+| */
//              ,    ,    ,    ,    ,    , 0  , 1  , 2  , 3  ,    ,             ,           , E, E, E, E,
//  /*|shift----`-z--`-x--`-c--`-v--`-b--`-n--`-m--`-,--`-.--`-/--`-------shift--+----+----+----+----+. */
//          ,     ,     ,                              ,      ,     ,     ,           , E, E, E, E
//  /*`ctrl-+-gui-+-alt-+----------space---------------+-fn---+-alt-+ctrl-' ANY--+----+----+----+----+   */
//   )
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};
