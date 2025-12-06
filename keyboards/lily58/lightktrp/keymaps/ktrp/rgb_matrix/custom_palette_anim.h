#ifdef ENABLE_RGB_MATRIX_PALETTES
RGB_MATRIX_EFFECT(PALETTES)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#        include "rgb_matrix_user.h"
#        define PHYSICAL_ROWS (MATRIX_ROWS / 2)

HSV     palette[PHYSICAL_ROWS];
RGB     scaled_palette[PHYSICAL_ROWS];
uint8_t user_value;

static bool PALETTES(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (params->init || user_value != rgb_matrix_get_val()) {
        palette[0] = rgb_to_hsv(rgb(248, 246, 241));
        palette[1] = rgb_to_hsv(rgb(242, 158, 53));
        palette[2] = rgb_to_hsv(rgb(220, 99, 62));
        palette[3] = rgb_to_hsv(rgb(102, 150, 206));
        palette[4] = rgb_to_hsv(rgb(102, 150, 206));
        user_value = rgb_matrix_get_val();

        for (int i = 0; i < PHYSICAL_ROWS; i++) {
            HSV row_color     = palette[i];
            RGB led_color     = hsv_to_rgb((HSV){row_color.h, row_color.s, scale8(row_color.v, user_value)});
            scaled_palette[i] = led_color;
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        keypos_t keypos    = rgb_matrix_get_position(i);
        uint8_t  row       = keypos.row % PHYSICAL_ROWS;
        RGB      led_color = scaled_palette[row];
        rgb_matrix_set_color(i, led_color.r, led_color.g, led_color.b);
    }

    return rgb_matrix_check_finished_leds(led_max);
}
#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_PALETTES
