#include QMK_KEYBOARD_H
#include <keymap_japanese.h>
#include "users/nicola/nicola.h" 
#include "us_emulation.h"

enum layer_names {
    _BASE,
    _BASE_JP,
    _NICOLA,
    _FN      
};

enum custom_keycodes {
    KC_KANA  = NG_SAFE_RANGE, 
    KC_EISU,                 
    KC_WIN_SHFT_RIGHT,      
    KC_TEAM_MUTE,           
    TG_JP,
    KC_MDN_PRV                 
};

bool us_emu_enabled = false;

void update_led_status(void) {
    // Force RGB ON and Static mode without touching EEPROM
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

    bool nicola = nicola_state();
    if (us_emu_enabled) {
        // JP Compatible mode (Green)
        if (nicola) {
            rgblight_setrgb(255, 0, 255); // Purple
        } else {
            rgblight_setrgb(0, 255, 0);   // Green
        }
    } else {
        // Normal US mode (Blue)
        if (nicola) {
            rgblight_setrgb(255, 0, 0);   // Red
        } else {
            rgblight_setrgb(0, 0, 255);   // Blue
        }
    }
}

// --- Custom Key Hooks ---

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_directional(
        KC_LNG1, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_GRV,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_DEL,
        KC_ESC,  KC_LGUI, KC_LALT, KC_LNG2, LT(_FN, KC_LNG1), KC_SPC, KC_TEAM_MUTE, KC_WIN_SHFT_RIGHT, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_BASE_JP] = LAYOUT_directional(
        KC_LNG1, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    JP_MINS, JP_CIRC, JP_YEN,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_AT,   JP_LBRC, JP_RBRC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_COLN,          KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_DEL,
        KC_ESC,  KC_LGUI, KC_LALT, KC_LNG2, LT(_FN, KC_LNG1), KC_SPC, KC_TEAM_MUTE, KC_WIN_SHFT_RIGHT, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_FN] = LAYOUT_directional(
        KC_TRNS, KC_F1,        KC_F2,   KC_F3,   KC_F4,      KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, QK_BOOT,
        KC_TRNS, KC_TRNS,      KC_HOME, KC_UP,   KC_END,     KC_TRNS, KC_TRNS, KC_PGUP, KC_INS,  KC_DEL,  KC_PSCR, KC_SCRL, KC_PAUS, KC_TRNS,    
        KC_CAPS, LALT(KC_GRV), KC_LEFT, KC_DOWN, KC_RGHT,    KC_TRNS, KC_TRNS, KC_INS,  KC_HOME, KC_PGUP, KC_BSPC, KC_TRNS,          KC_TRNS,
        KC_TRNS, KC_VOLD,      KC_VOLU, KC_MUTE, KC_TRNS, KC_MDN_PRV, KC_TRNS, KC_DEL,  KC_END,  KC_PGDN, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        TG_JP,   KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT
    ),

    [_NICOLA] = LAYOUT_directional(
        KC_EISU, NG_1,    NG_2,    NG_3,    NG_4,    NG_5,    NG_6,    NG_7,    NG_8,    NG_9,    NG_0,    KC_MINS, KC_EQL,  KC_GRV,  KC_BSPC,
        KC_TAB,  NG_Q,    NG_W,    NG_E,    NG_R,    NG_T,    NG_Y,    NG_U,    NG_I,    NG_O,    NG_P,    NG_LBRC, NG_RBRC, KC_BSLS,
        KC_TRNS, NG_A,    NG_S,    NG_D,    NG_F,    NG_G,    NG_H,    NG_J,    NG_K,    NG_L,    NG_SCLN, NG_QUOT,          KC_ENT,
        KC_TRNS, NG_Z,    NG_X,    NG_C,    NG_V,    NG_B,    NG_N,    NG_M,    NG_COMM, NG_DOT,  NG_SLSH, NG_SHFTR, KC_UP,   KC_DEL,
        KC_TRNS, KC_LGUI, KC_LALT, NG_SHFTL, LT(_FN, KC_LNG1), NG_SHFTR,  KC_TEAM_MUTE, KC_WIN_SHFT_RIGHT,  KC_LEFT, KC_DOWN, KC_RGHT
    )
};

void matrix_init_user(void) {
    set_nicola(_NICOLA); 
}

void keyboard_post_init_user(void) {
    us_emu_enabled = false;
    nicola_off(); 
}

void matrix_scan_user(void) {
    static uint16_t boot_timer = 0;
    if (boot_timer < 500) { 
        boot_timer++;
        if (boot_timer == 500) {
            update_led_status();
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_WIN_SHFT_RIGHT:
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                wait_ms(100);
                tap_code(KC_RGHT);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
                return false;
            case KC_TEAM_MUTE:
                register_code(KC_LGUI);
                register_code(KC_LALT);
                wait_ms(100);
                tap_code(KC_K);
                unregister_code(KC_LALT);
                unregister_code(KC_LGUI);
                return false;
            case KC_MDN_PRV:
                register_code(KC_LSFT);
                register_code(KC_LCTL);
                wait_ms(100);
                tap_code(KC_V);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                return false;
            case TG_JP:
                us_emu_enabled = !us_emu_enabled;
                update_led_status();
                return false;
            case KC_LNG1:
                nicola_on();
                return false;
            case KC_LNG2:
                nicola_off();
                return false;
            case LT(_FN, KC_LNG1):
                if (record->event.pressed && record->tap.count > 0) {
                    nicola_on();
                    return false;
                }
                break;
            case LT(_FN, KC_LNG2):
                if (record->event.pressed && record->tap.count > 0) {
                    nicola_off();
                    return false;
                }
                break;
        }
    }

    if (us_emu_enabled && get_highest_layer(layer_state) == _BASE) {
        if (!process_us_emulation(keycode, record)) {
            return false;
        }
    }

    if (key_process_guard) {
        return true; 
    }

    nicola_mode(keycode, record);
    if (nicola_state()) {
        if (!process_nicola(keycode, record)) {
            return false;
        }
    }

    return true;
}

void nicola_on_user(void) {
    tap_code(KC_LNG1);
    update_led_status();
}

void nicola_off_user(void) {
    tap_code(KC_LNG2);
    update_led_status();
}
