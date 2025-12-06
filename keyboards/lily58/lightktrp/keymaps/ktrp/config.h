/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2020 Ben Roesner (keycapsss.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define RAW_HID_SEND_KEYS_ENABLED false

#define MASTER_LEFT

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SPLASH // Static single hue, value pulse away from multiple key hits then fades value out
                                                // The animation uses these two defines in order to allow customization:

#define SOLID_REACTIVE_SPLASH_WAVE_RGB 0xE6CD12 // light yellow-ish
#define SOLID_REACTIVE_SPLASH_WAVE_WIDTH 50     // ~4 keys wide

#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A

/**======================
 **    Analog
 *========================**/

#undef RP_ADC_USE_ADC1
#define RP_ADC_USE_ADC1 TRUE

/**======================
 **    I2C Driver
 *========================**/

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

/**======================
 **      UART Driver
 *========================**/

#define UART_DRIVER SIOD0
#define UART_TX_PIN GP0
#define UART_RX_PIN GP1

/**======================
 **      RESET
 *========================**/

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET              // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U // Timeout window in ms in which the double tap can occur.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17     // Specify a optional status led by GPIO number which blinks when entering the bootloader

#define PALETTEFX_ENABLE_ALL_EFFECTS
#define PALETTEFX_ENABLE_ALL_PALETTES
