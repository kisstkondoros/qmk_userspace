#include "raw_hid.h"
#include "action_layer.h"

typedef enum {
    _KEY_EVENT = 0xAA, // avoid conflict with VIA
    _lAYER_EVENT
} EVENT_TYPE;

bool process_record_user_send_state_over_raw_hid(uint16_t keycode, keyrecord_t* record) {
    if (!is_keyboard_master()) {
        return false;
    }

#ifdef RAW_HID_SEND_KEYS_ENABLED
    uint8_t data[32];
    memset(data, 0, 32);

    data[0] = _KEY_EVENT;
    data[1] = record->event.key.row;
    data[2] = record->event.key.col;
    data[3] = record->event.pressed;
    data[4] = (uint8_t)keycode >> 8; // high bits
    data[5] = (uint8_t)keycode;

    raw_hid_send(data, 32);
#endif /* ifdef RAW_HID_SEND_KEYS_ENABLED */
    return false;
}

void send_layer_over_raw_hid(void) {
#ifdef RAW_HID_SEND_KEYS_ENABLED
    uint8_t data[32];
    memset(data, 0, 32);

    data[0] = _lAYER_EVENT;
    data[1] = get_highest_layer(layer_state);

    raw_hid_send(data, 32);
#endif /* ifdef RAW_HID_SEND_KEYS_ENABLED */
}
