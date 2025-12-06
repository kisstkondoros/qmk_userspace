RGB_MATRIX_CUSTOM_USER = yes
ANALOG_DRIVER_REQUIRED = yes
RAW_ENABLE = yes

SRC += ./oled/temperature.c
SRC += ./oled/luna.c
SRC += ./oled/oled_stuff.c
SRC += ./state/state.c
SRC += ./state/send_keys_raw_hid.c
SRC += ./rgb_matrix/rgb_matrix_user.c
