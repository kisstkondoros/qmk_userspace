#include "quantum.h"
#include QMK_KEYBOARD_H
#include "keymap_hungarian.h"
#include "modules/getreuer/orbital_mouse/orbital_mouse.h"

#include "layers.h"
#include "oled/temperature.h"
#include "oled/oled_stuff.h"
#include "state/state.h"
#include "state/send_keys_raw_hid.h"
#include "rgb_matrix/rgb_matrix_user.h"

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

enum custom_keycodes { UG_CMOD = QK_USER };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  Ö   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  Ü   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   É  |  Á   |
 * |------+------+------+------+------+------|   Ő   |    |    Ú  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   -  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RAlt |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    HU_0,     HU_ODIA,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     HU_UDIA,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    HU_EACU,  HU_AACU,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, HU_ODAC,  HU_UACU,  KC_N,    KC_M,    KC_COMM, KC_DOT,  HU_MINS,  KC_RSFT,
                             KC_LALT, KC_LGUI,LOWER, KC_SPC,   KC_ENT,   RAISE,   KC_BSPC, KC_RALT
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |  2c  |click | M_U  | btn2 | PSCR | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------| hold |rotl  | M_D  | rotr |bsel1 |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------| rel  | w_d  | w_u  | btn3 |bsel2 |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |  DEL |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT(
  _______, KC_F1,   KC_F2  ,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10 ,  KC_F11,
  _______, _______, _______, _______, _______, _______,                   OM_DBLS, OM_BTNS, OM_U   , OM_BTN2, KC_PSCR,  KC_F12,
  _______, _______, _______, _______, _______, _______,                   OM_HLDS, OM_L   , OM_D   , OM_R   , OM_SEL1, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, OM_RELS, OM_W_D , OM_W_U , OM_BTN3, OM_SEL2, _______,
                             _______, _______, _______, _______, _______, _______, KC_DEL , _______
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  !   |  @   |  [   |  ]   |  |   |                    |Insert| Home | PGDN | PGUP | End  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  #   |  $   |  (   |  )   |  `   |-------.    ,-------|      | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------|   Ű   |    |   Ó   |------+------+------+------+------+------|
 * |      |  %   |  ^   |  {   |  }   |  ~   |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______,
  _______, HU_EXLM, HU_AT  , HU_LBRC, HU_RBRC, HU_PIPE,                     KC_INS , KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
  _______, HU_HASH, HU_DLR , HU_LPRN, HU_RPRN, HU_GRV ,                     XXXXXXX, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, XXXXXXX,
  _______, HU_PERC, HU_CIRC, HU_LCBR, HU_RCBR, HU_TILD,  HU_UDAC, HU_OACU,  _______, _______, _______, _______, _______, _______,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | BOOT | RESET|      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |LUMINO|      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|UG_CMOD|      |      | HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------| MODE |      |      | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
  QK_BOOT, QK_RBT , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   LUMINO , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   UG_CMOD, XXXXXXX, XXXXXXX, RM_HUEU, RM_SATU, RM_VALU,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_NEXT, XXXXXXX, XXXXXXX, RM_HUED, RM_SATD, RM_VALD,
                             _______, _______, _______, _______, _______,  _______, _______, _______
  )
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
    return state;
}

void matrix_init_user(void) {
    rgb_matrix_init_user();
    init_temp_sensor();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_record_user_send_state_over_raw_hid(keycode, record);
    process_record_user_oled(keycode, record);
    process_record_user_state(keycode, record);

    switch (keycode) {
        case UG_CMOD:
            if (record->event.pressed) {
                user_state.selectedEffect = (user_state.selectedEffect + 1) % 5;
            }
            break;
        default:
            break;
    }

    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    send_layer_over_raw_hid();
}
