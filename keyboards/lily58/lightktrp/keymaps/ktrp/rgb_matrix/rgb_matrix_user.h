#pragma once
#include "keyboard.h"

void     rgb_matrix_init_user(void);
keypos_t rgb_matrix_get_position(uint8_t led_index);
void     rgb_matrix_set_color_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), uint8_t red, uint8_t green, uint8_t blue);
