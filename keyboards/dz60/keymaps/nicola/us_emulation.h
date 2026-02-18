#pragma once
#include QMK_KEYBOARD_H
#include <keymap_japanese.h>

// 各キーが押された時の物理Shift状態を記録
static uint8_t emu_active_mods[256];
static uint16_t emu_active_targets[256];

bool process_us_emulation(uint16_t keycode, keyrecord_t *record) {
    if (keycode > 255) return true;

    if (record->event.pressed) {
        uint8_t real_mods = get_mods();
        bool shifted = real_mods & MOD_MASK_SHIFT;
        uint16_t target = 0;

        // US配列の各キーに対するマッピング
        switch (keycode) {
            case KC_1:    target = shifted ? JP_EXLM : KC_1; break;
            case KC_2:    target = shifted ? JP_AT   : KC_2; break;
            case KC_3:    target = shifted ? JP_HASH : KC_3; break;
            case KC_4:    target = shifted ? JP_DLR  : KC_4; break;
            case KC_5:    target = shifted ? JP_PERC : KC_5; break;
            case KC_6:    target = shifted ? JP_CIRC : KC_6; break;
            case KC_7:    target = shifted ? JP_AMPR : KC_7; break;
            case KC_8:    target = shifted ? JP_ASTR : KC_8; break;
            case KC_9:    target = shifted ? JP_LPRN : KC_9; break;
            case KC_0:    target = shifted ? JP_RPRN : KC_0; break;
            case KC_MINS: target = shifted ? JP_UNDS : JP_MINS; break;
            case KC_EQL:  target = shifted ? JP_PLUS : JP_EQL;  break;
            case KC_LBRC: target = shifted ? JP_LCBR : JP_LBRC; break;
            case KC_RBRC: target = shifted ? JP_RCBR : JP_RBRC; break;
            case KC_BSLS: target = shifted ? JP_PIPE : JP_BSLS; break;
            case KC_SCLN: target = shifted ? JP_COLN : JP_SCLN; break;
            case KC_QUOT: target = shifted ? JP_DQUO : JP_QUOT; break;
            case KC_GRV:  target = shifted ? JP_TILD : JP_GRV;  break;
            case KC_COMM: target = shifted ? JP_LABK : KC_COMM; break;
            case KC_DOT:  target = shifted ? JP_RABK : KC_DOT;  break;
            case KC_SLSH: target = shifted ? JP_QUES : KC_SLSH; break;
        }

        if (target) {
            // 現在の物理Modを保存
            emu_active_mods[keycode] = real_mods;
            emu_active_targets[keycode] = target;

            // ターゲットに必要なShift状態と現在の状態が異なる場合のみ操作
            bool needs_shift = (IS_QK_MODS(target) && (QK_MODS_GET_MODS(target) & MOD_MASK_SHIFT));
            
            if (shifted != needs_shift) {
                if (shifted) del_mods(MOD_MASK_SHIFT);
                if (needs_shift) add_mods(MOD_BIT(KC_LSFT));
                send_keyboard_report();
            }

            register_code(target & 0xFF);
            
            // 注意: ここでModを復帰させると、OSのリピート機能が混乱するため、離すまで維持する
            return false;
        }
    } else {
        if (emu_active_targets[keycode]) {
            uint16_t target = emu_active_targets[keycode];
            uint8_t pressed_mods = emu_active_mods[keycode];
            
            unregister_code(target & 0xFF);

            // 物理的にShiftがまだ押されているか確認（コアの状態はemu_active_mods等で変化しているため、実際のキー状態を考慮する必要があるが、
            // シンプルに保存したModを復帰させるのではなく、現在の「他のキーによる変更」も考慮して、一旦リセットしてから物理状態に戻す）
            
            // 他のエミュレーションキーがまだ押されている可能性を考慮し、一旦現在のModからShiftを掃除
            del_mods(MOD_MASK_SHIFT);
            // 物理的に押されていたShiftを復帰させる。add_mods(MOD_MASK_SHIFT)ではなく、保存した特定ビットのみ。
            if (pressed_mods & MOD_BIT(KC_LSFT)) add_mods(MOD_BIT(KC_LSFT));
            if (pressed_mods & MOD_BIT(KC_RSFT)) add_mods(MOD_BIT(KC_RSFT));
            
            send_keyboard_report();
            
            emu_active_targets[keycode] = 0;
            return false;
        }
    }
    return true;
}
