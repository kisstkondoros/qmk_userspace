// SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE
#    include "action_layer.h"
#    include "action_util.h"
#    include "rgb_matrix.h"
#    include "keyboard.h"
#    include "keycodes.h"
#    include "oled_driver.h"
#    include "progmem.h"
#    include "wpm.h"

#    include "oled_stuff.h"
#    include "luna.h"
#    include "../layers.h"
#    include "temperature.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_270;
    }
}

void render_wpm(uint8_t wpm) {
    char wpm_counter[4];
    wpm_counter[3] = '\0';
    wpm_counter[2] = '0' + wpm % 10;
    wpm_counter[1] = (wpm /= 10) % 10 ? '0' + (wpm) % 10 : (wpm / 10) % 10 ? '0' : ' ';
    wpm_counter[0] = wpm / 10 ? '0' + wpm / 10 : ' ';

    oled_write_ln_P(PSTR("WPM"), false);
    oled_write_P(PSTR(" "), false);
    oled_write(wpm_counter, false);
    oled_write_P(PSTR(" "), false);
}

#    define KEYLOG_LEN 6
char     keylog_str[KEYLOG_LEN] = {};
uint8_t  keylogs_str_idx        = 0;
uint16_t log_timer              = 0;

// clang-format off
const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};
// clang-format on

void add_keylog(uint16_t keycode) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        keycode = keycode & 0xFF;
    }

    for (uint8_t i = KEYLOG_LEN - 1; i > 0; i--) {
        keylog_str[i] = keylog_str[i - 1];
    }
    if (keycode < 60) {
        keylog_str[0] = code_to_name[keycode];
    }
    keylog_str[KEYLOG_LEN - 1] = 0;

    log_timer = timer_read();
}

void update_log(void) {
    if (timer_elapsed(log_timer) > 750) {
        add_keylog(0);
    }
}

void render_keylogger_status(void) {
    oled_write_P(PSTR("KLogr"), false);
    oled_write(keylog_str, false);
}

void render_default_layer_state(void) {
    oled_write_P(PSTR("Layer"), false);
    oled_write_P(PSTR(" "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QRTY"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("LOW"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("HIGH"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("ADJ"), false);
            break;
        default:
            oled_write_ln_P(PSTR("????"), false);
    }
}

void render_mod_status(uint8_t modifiers) {
    oled_write_ln_P(PSTR("Mods"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("S"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR("C"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR("A"), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR("G"), (modifiers & MOD_MASK_GUI));
}

void render_status_main(void) {
    // Show keyboard layout
    render_default_layer_state();
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    // Show modifier status
    render_mod_status(get_mods());
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    render_temp();
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    render_keylogger_status();
}

void render_rgb_status(void) {
    uint8_t mode = rgb_matrix_config.mode;

    oled_write_ln_P(PSTR("RGB"), false);
    char rgb_str[4];
    rgb_str[3] = '\0';
    rgb_str[2] = '0' + mode % 10;
    rgb_str[1] = (mode /= 10) % 10 ? '0' + (mode) % 10 : (mode / 10) % 10 ? '0' : ' ';
    rgb_str[0] = mode / 10 ? '0' + mode / 10 : ' ';
    oled_write_P(PSTR(" "), false);
    oled_write(rgb_str, false);
    oled_write_P(PSTR(" "), false);
}

void render_countdown(void) {
    uint32_t remaining = OLED_TIMEOUT - last_input_activity_elapsed();
    remaining /= 1000;

    oled_write_ln_P(PSTR("CDown"), false);
    char str[5];
    str[4] = '\0';
    str[3] = 's';
    str[2] = '0' + remaining % 10;
    str[1] = (remaining /= 10) % 10 ? '0' + (remaining) % 10 : (remaining / 10) % 10 ? '0' : ' ';
    str[0] = remaining / 10 ? '0' + remaining / 10 : ' ';
    oled_write_P(PSTR(" "), false);
    oled_write(str, false);
    oled_write_P(PSTR(" "), false);
}

void render_status_offhand(void) {
    int current_wpm = get_current_wpm();

    render_wpm(current_wpm);
    oled_write_P(PSTR("-----"), false);
    render_rgb_status();
    oled_write_P(PSTR("-----"), false);
    render_countdown();

    render_luna(0, 13, current_wpm);
}

bool oled_task_user(void) {
    update_log();

    if (last_input_activity_elapsed() > OLED_TIMEOUT) {
        oled_clear();
        oled_off();
    } else {
        oled_on();

        if (is_keyboard_master()) {
            render_status_main(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
        } else {
            render_status_offhand();
        }
    }
    return false;
}

bool process_record_user_oled(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;

    if (pressed) {
        add_keylog(keycode);
    }
    return false;
}
#endif // OLED_ENABLE
