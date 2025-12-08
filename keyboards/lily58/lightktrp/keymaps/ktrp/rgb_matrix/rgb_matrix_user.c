#ifdef RGB_MATRIX_ENABLE
#    include <lib/lib8tion/lib8tion.h>

#    include "keymap_common.h"
#    include "rgb_matrix.h"
#    include "action_layer.h"

#    include "rgb_matrix_user.h"
#    include "../state/state.h"

keypos_t led_index_key_position[RGB_MATRIX_LED_COUNT];

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

double threeway_max(double a, double b, double c) {
    return max(a, max(b, c));
}

double threeway_min(double a, double b, double c) {
    return min(a, min(b, c));
}

/*
 * Initialise RGB matrix; invert the mapping of g_led_config.matrix_co,
 * so instead of a mapping from key position to led index, we now create
 * led index to key position.
 */
void rgb_matrix_init_user() {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index != NO_LED) led_index_key_position[led_index] = (keypos_t){.row = row, .col = col};
        }
    }
}

uint8_t lastActiveLayer = 0;

void set_indicator_colors(uint8_t led_min, uint8_t led_max, uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (g_led_config.flags[i] == LED_FLAG_UNDERGLOW) {
            rgb_matrix_set_color(i, r, g, b);
        }
        if (g_led_config.flags[i] == LED_FLAG_INDICATOR) {
            rgb_matrix_set_color(i, r >> 4, g >> 4, b >> 4);
        }
    }
}

static HSV effect_solid(HSV hsv, uint8_t index, uint32_t timer, uint8_t speed) {
    return hsv;
}

static HSV effect_breathe(HSV hsv, uint8_t index, uint32_t timer, uint8_t speed) {
    uint16_t time = scale16by8(timer, speed / 8);
    hsv.v         = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
    return hsv;
}

static HSV effect_spectrum(HSV hsv, uint8_t index, uint32_t timer, uint8_t speed) {
    uint16_t time = scale16by8(timer, speed / 8);
    hsv.h         = time;
    return hsv;
}

static HSV effect_swirl(HSV hsv, uint8_t index, uint32_t timer, uint8_t speed) {
    uint16_t time = scale16by8(timer, speed / 8);
    hsv.h         = (255 / RGB_MATRIX_LED_COUNT * index + time);
    return hsv;
}

static HSV effect_riverflow(HSV hsv, uint8_t index, uint32_t timer, uint8_t speed) {
    uint16_t time = scale16by8(timer + (index * 315), speed / 8);
    hsv.v         = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
    return hsv;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_get_suspend_state()) {
        return false;
    }
    uint8_t layer = get_highest_layer(layer_state);

    bool isDefaultLayer = layer == 0;

    uint8_t  speed = rgb_matrix_config.speed;
    uint32_t timer = g_rgb_timer;

    HSV (*effect)(HSV, uint8_t, uint32_t, uint8_t);
    switch (user_state.selectedEffect % 5) {
        case 0:
            effect = &effect_riverflow;
            break;
        case 1:
            effect = &effect_breathe;
            break;
        case 2:
            effect = &effect_spectrum;
            break;
        case 3:
            effect = &effect_swirl;
            break;

        case 4:
            effect = &effect_solid;
            break;
        default:
            effect = &effect_solid;
            break;
    }
    for (uint8_t index = led_min; index < led_max; index++) {
        if ((index >= led_min) && (index < led_max) && (index != NO_LED)) {
            HSV hsv = rgb_matrix_config.hsv;

            if (!isDefaultLayer) {
                hsv.h += 42 * layer;
            }

            RGB rgb = hsv_to_rgb((*effect)(hsv, index, timer, speed));

            if (g_led_config.flags[index] == LED_FLAG_UNDERGLOW || (!isDefaultLayer && keymap_key_to_keycode(layer, led_index_key_position[index]) > KC_TRNS)) {
                rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
            }
        }
    }

    return false;
}
#endif
