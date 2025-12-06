// Copyright (C) 2023 @flowchartsman

#ifdef ENABLE_RGB_MATRIX_PIXEL_FLAME
RGB_MATRIX_EFFECT(PIXEL_FLAME)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#        include "rgb_matrix_user.h"
#        define PALETTE_SIZE 8
#        ifndef RGB_MATRIX_PIXEL_FLAME_DELAY_MS
#            define RGB_MATRIX_PIXEL_FLAME_DELAY_MS 60
#        endif
RGB hcolors[PALETTE_SIZE];

uint8_t user_hue;
uint8_t user_value;

uint8_t *framebuf;

uint8_t fb_c = MATRIX_COLS * 2;
uint8_t fb_r = MATRIX_ROWS / 2;

void init_framebuf(void) {
    static bool did_init = false;
    if (did_init) {
        return;
    }
    framebuf = calloc((fb_r) * (fb_c), sizeof(uint8_t));

    did_init = true;
}

void generate_palette(uint8_t user_hue, uint8_t user_value) {
    uint8_t max  = PALETTE_SIZE - 1;
    hcolors[max] = hsv_to_rgb((HSV){user_hue + 35, 100, user_value});
    for (uint8_t i = 1; i <= 3; i++) {
        hcolors[max - i] = hsv_to_rgb((HSV){blend8(user_hue + 35, user_hue + 5, (256 / 3) * i), 255, user_value});
    }
    for (uint8_t i = 0; i < max - 3; i++) {
        uint8_t scale = 256 / (max - 3) * i;
        if (i == max - 4) {
            scale = 255;
        }
        hcolors[i] = hsv_to_rgb((HSV){user_hue, 255, scale8(blend8(0, 255, scale), user_value)});
    }
}

/* Flare constants */
#        define MAX_FLARES 8
#        define FLARE_ROWS 2
#        define FLARE_CHANCE 50
/* decay rate of flare radiation; 14 is good */
#        define FLARE_DECAY 14

/* Flare variables*/
uint8_t  nflare = 0;
uint32_t flare[MAX_FLARES];

uint32_t isqrt(uint32_t n) {
    if (n < 2) return n;
    uint32_t smallCandidate = isqrt(n >> 2) << 1;
    uint32_t largeCandidate = smallCandidate + 1;
    return (largeCandidate * largeCandidate > n) ? smallCandidate : largeCandidate;
}

// Set pixels to intensity around flare
void glow(int x, int y, int z) {
    int b = z * 10 / FLARE_DECAY + 1;
    for (int i = (y - b); i < (y + b); ++i) {
        for (int j = (x - b); j < (x + b); ++j) {
            if (i < 0 || i >= fb_r || j < 0 || j >= fb_c) continue;
            int     d = (FLARE_DECAY * isqrt((x - j) * (x - j) + (y - i) * (y - i)) + 5) / 10;
            uint8_t n = 0;
            if (z > d) n = z - d;
            if (n > framebuf[i * fb_c + j]) { // can only get brighter
                framebuf[i * fb_c + j] = n;
            }
        }
    }
}

/* Fire functions*/
void newflare(void) {
    if (nflare < MAX_FLARES && rand() % FLARE_CHANCE == 0) {
        int x           = rand() % fb_c;
        int y           = rand() % FLARE_ROWS;
        int z           = PALETTE_SIZE - 1;
        flare[nflare++] = (z << 16) | (y << 8) | (x & 0xff);
        glow(x, y, z);
    }
}

void init_fire(void) {
    // Heat the bottom row
    for (uint8_t j = 0; j < fb_c; ++j) {
        framebuf[j] = random8_min_max(PALETTE_SIZE - 4, PALETTE_SIZE - 2);
    }
}

void make_fire(void) {
    uint8_t i, j;

    // First, move all existing heat points up the display and fade
    for (i = fb_r - 1; i > 0; --i) {
        for (j = 0; j < fb_c; ++j) {
            uint8_t n = 0;
            if (framebuf[(i - 1) * fb_c + j] > 0) n = framebuf[(i - 1) * fb_c + j] - 1;
            framebuf[i * fb_c + j] = n;
        }
    }

    // Heat the bottom row
    for (j = 0; j < fb_c; ++j) {
        i = framebuf[j];
        if (i > 0) {
            framebuf[j] = random8_min_max(PALETTE_SIZE - 4, PALETTE_SIZE - 2);
        }
    }
    // flare
    for (i = 0; i < nflare; ++i) {
        int x = flare[i] & 0xff;
        int y = (flare[i] >> 8) & 0xff;
        int z = (flare[i] >> 16) & 0xff;
        glow(x, y, z);
        if (z > 1) {
            flare[i] = (flare[i] & 0xffff) | ((z - 1) << 16);
        } else {
            // This flare is out
            for (int j = i + 1; j < nflare; ++j) {
                flare[j - 1] = flare[j];
            }
            --nflare;
        }
    }
    newflare();
}

// A timer to track the last time we updated the flame render.
static uint16_t flame_render_timer;
// Whether we should render the flame during the next update.
static bool render_flame;

bool found = false;

static bool PIXEL_FLAME(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->iter == 0) {
        if (params->init) {
            init_framebuf();
            rgb_matrix_set_color_all(0, 0, 0);
            user_hue   = rgb_matrix_get_hue();
            user_value = rgb_matrix_get_val();
            generate_palette(user_hue, user_value);
            init_fire();
        }
        render_flame = timer_elapsed(flame_render_timer) >= RGB_MATRIX_PIXEL_FLAME_DELAY_MS;
        if (render_flame) {
            if (user_hue != rgb_matrix_get_hue() || user_value != rgb_matrix_get_val()) {
                user_hue   = rgb_matrix_get_hue();
                user_value = rgb_matrix_get_val();
                generate_palette(user_hue, user_value);
            }
            make_fire();
            flame_render_timer = timer_read();
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        keypos_t keypos = rgb_matrix_get_position(i);
        uint8_t  row    = keypos.row;
        uint8_t  col    = keypos.col;

        if (row >= fb_r) {
            row -= fb_r;
            col += MATRIX_COLS;
        }

        RGB led_color = hcolors[framebuf[(fb_r - 1 - row) * fb_c + col]];
        rgb_matrix_set_color(i, led_color.r, led_color.g, led_color.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_PIXEL_FLAME
