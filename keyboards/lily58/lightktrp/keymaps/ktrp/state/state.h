#pragma once

#include "action.h"

typedef union {
    uint32_t raw;
    struct {
        bool isSpace : 1;
        bool isShift : 1;
        bool isControl : 1;

        int selectedEffect : 8;
    };
} user_runtime_config_t;

extern user_runtime_config_t user_state;

bool process_record_user_state(uint16_t keycode, keyrecord_t* record);
