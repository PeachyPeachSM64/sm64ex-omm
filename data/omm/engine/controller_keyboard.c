#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define KM_SCANCODE(km) (u32) (((km) >> 32llu) & 0xFFFFFFFFllu)
#define KM_MASK(km) (u32) (((km) >>  0llu) & 0xFFFFFFFFllu)
#define KM_MAPPING(scancode, mask) ((((u64) (scancode)) << 32llu) | (((u64) (mask)) << 0llu))

static OmmArray sKeyboardMapping = omm_array_zero;
static u32 sKeyboardDownMask = 0;
static u32 sKeyboardLastKey = VK_INVALID;

//
// Keyboard API
//

static void keyboard_add_binds(u32 mask, u32 *scancodes) {
    for (s32 i = 0; i != MAX_BINDS; ++i) {
        if (scancodes[i] < VK_BASE_KEYBOARD + VK_SIZE) {
            u64 km = KM_MAPPING(scancodes[i], mask);
            omm_array_add(sKeyboardMapping, u64, km);
        }
    }
}

static void keyboard_bindkeys(void) {
    omm_array_clear(sKeyboardMapping);
    keyboard_add_binds(A_BUTTON, gOmmControlsButtonA);
    keyboard_add_binds(B_BUTTON, gOmmControlsButtonB);
    keyboard_add_binds(X_BUTTON, gOmmControlsButtonX);
    keyboard_add_binds(Y_BUTTON, gOmmControlsButtonY);
    keyboard_add_binds(START_BUTTON, gOmmControlsButtonStart);
    keyboard_add_binds(SPIN_BUTTON, gOmmControlsButtonSpin);
    keyboard_add_binds(L_TRIG, gOmmControlsTriggerL);
    keyboard_add_binds(R_TRIG, gOmmControlsTriggerR);
    keyboard_add_binds(Z_TRIG, gOmmControlsTriggerZ);
    keyboard_add_binds(U_CBUTTONS, gOmmControlsCUp);
    keyboard_add_binds(D_CBUTTONS, gOmmControlsCDown);
    keyboard_add_binds(L_CBUTTONS, gOmmControlsCLeft);
    keyboard_add_binds(R_CBUTTONS, gOmmControlsCRight);
    keyboard_add_binds(U_JPAD, gOmmControlsDUp);
    keyboard_add_binds(D_JPAD, gOmmControlsDDown);
    keyboard_add_binds(L_JPAD, gOmmControlsDLeft);
    keyboard_add_binds(R_JPAD, gOmmControlsDRight);
    keyboard_add_binds(STICK_UP, gOmmControlsStickUp);
    keyboard_add_binds(STICK_DOWN, gOmmControlsStickDown);
    keyboard_add_binds(STICK_LEFT, gOmmControlsStickLeft);
    keyboard_add_binds(STICK_RIGHT, gOmmControlsStickRight);
}

static void keyboard_init(void) {
    keyboard_bindkeys();
}

static void keyboard_read(OSContPad *pad) {
    pad->button |= sKeyboardDownMask;
    gOmmMario->spin.pressed |= (sKeyboardDownMask & SPIN_BUTTON) != 0;
    switch (sKeyboardDownMask & STICK_XMASK) {
        case STICK_LEFT:  pad->stick_x = -0x7F; break;
        case STICK_RIGHT: pad->stick_x = +0x7F; break;
    }
    switch (sKeyboardDownMask & STICK_YMASK) {
        case STICK_DOWN:  pad->stick_y = -0x7F; break;
        case STICK_UP:    pad->stick_y = +0x7F; break;
    }
}

static u32 keyboard_rawkey(void) {
    u32 key = sKeyboardLastKey;
    sKeyboardLastKey = VK_INVALID;
    return key;
}

static void keyboard_shutdown(void) {
}

struct ControllerAPI controller_keyboard = {
    VK_BASE_KEYBOARD,
    keyboard_init,
    keyboard_read,
    keyboard_rawkey,
    NULL,
    NULL,
    keyboard_bindkeys,
    keyboard_shutdown
};

//
// Keyboard Callbacks
//

static u32 keyboard_get_mask(u32 scancode) {
    u32 mask = 0;
    omm_array_for_each(sKeyboardMapping, pkm) {
        u64 km = pkm->as_u64;
        if (KM_SCANCODE(km) == scancode) {
            mask |= KM_MASK(km);
        }
    }
    return mask;
}

bool keyboard_on_key_down(s32 scancode) {
    u32 mask = keyboard_get_mask(scancode);
    sKeyboardDownMask |= mask;
    sKeyboardLastKey = scancode;
    return mask != 0;
}

bool keyboard_on_key_up(s32 scancode) {
    u32 mask = keyboard_get_mask(scancode);
    sKeyboardDownMask &= ~mask;
    if (sKeyboardLastKey == (u32) scancode) {
        sKeyboardLastKey = VK_INVALID;
    }
    return mask != 0;
}

void keyboard_on_all_keys_up(void) {
    sKeyboardDownMask = 0;
}
