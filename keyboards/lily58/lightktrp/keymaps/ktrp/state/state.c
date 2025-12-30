#include "action_util.h"
#include "transactions.h"
#include "state.h"

user_runtime_config_t user_state;

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    // if buffer length matches size of data structure (simple error checking)
    if (in_buflen == sizeof(user_state)) {
        // copy data from master into local data structure
        memcpy(&user_state, in_data, in_buflen);
    }
}

void keyboard_post_init_user(void) {
    // register sync handler
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // sync valuesS
        static uint32_t              last_sync       = 0;
        static user_runtime_config_t last_user_state = (user_runtime_config_t){.isSpace = 0, .isControl = 0, .isShift = 0, .selectedEffect = 0};
        static bool                  needs_sync      = false;

        // if value is different, then needs syncing
        if (memcmp(&user_state, &last_user_state, sizeof(user_state))) {
            needs_sync = true;
            // copy local user state to verify changes
            memcpy(&last_user_state, &user_state, sizeof(user_state));
        }
        // Send to slave every 250ms regardless of state change
        if (timer_elapsed32(last_sync) > 250) {
            needs_sync = true;
        }

        // if it needs syncing:
        if (needs_sync) {
            // send user_data stuct over to slave
            if (transaction_rpc_send(USER_SYNC_A, sizeof(user_state), &user_state)) {
                // reset sync checks
                last_sync  = timer_read32();
                needs_sync = false;
            }
        }
    }
}

bool process_record_user_state(uint16_t keycode, keyrecord_t* record) {
    bool pressed = record->event.pressed;

    user_state.isShift   = pressed && get_mods() & MOD_MASK_SHIFT;
    user_state.isControl = pressed && get_mods() & MOD_MASK_CTRL;
    user_state.isSpace   = pressed && keycode == KC_SPC;

    return true;
}
