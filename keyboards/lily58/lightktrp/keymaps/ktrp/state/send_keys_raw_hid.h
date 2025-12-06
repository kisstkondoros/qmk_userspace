#pragma once

#include "action.h"

bool process_record_user_send_state_over_raw_hid(uint16_t keycode, keyrecord_t* record);
void send_layer_over_raw_hid(void);
