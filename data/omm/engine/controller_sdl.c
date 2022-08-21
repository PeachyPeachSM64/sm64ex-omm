#ifdef CAPI_SDL2
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define VK_OFS_SDL_MOUSE 0x0100
#define VK_BASE_SDL_MOUSE (VK_BASE_SDL_GAMEPAD + VK_OFS_SDL_MOUSE)
#define NUM_JOYSTICK_BUTTONS (1 + MAX(MAX(VK_LTRIGGER, VK_RTRIGGER) - VK_BASE_SDL_GAMEPAD, SDL_CONTROLLER_BUTTON_MAX))

#define CM_BUTTON(cm) (u32) (((cm) >> 32llu) & 0xFFFFFFFFllu)
#define CM_MASK(cm) (u32) (((cm) >>  0llu) & 0xFFFFFFFFllu)
#define CM_MAPPING(button, mask) ((((u64) (button)) << 32llu) | (((u64) (mask)) << 0llu))

static OmmArray sJoystickMapping = omm_array_zero;
static OmmArray sMouseMapping = omm_array_zero;
static bool sJoystickButtonsDown[NUM_JOYSTICK_BUTTONS] = { false };
static u32 sMouseButtonsDown = 0;
static u32 sJoystickLastButton = VK_INVALID;
static u32 sMouseLastButton = VK_INVALID;
static bool sControllerInited;
static SDL_GameController *sController;
s32 gMouseHasFreeControl = FALSE;
s32 gMouseHasCenterControl = FALSE;

//
// Joystick
//

static void controller_sdl_update_joystick_button(s32 index, bool down) {
    bool pressed = (!sJoystickButtonsDown[index] && down);
    sJoystickButtonsDown[index] = down;
    if (pressed) {
        sJoystickLastButton = index;
    }
}

//
// Mouse
//

s32 gMouseX;
s32 gMouseY;

static u32 controller_sdl_get_mouse_state() {
    SDL_SetRelativeMouseMode(BETTER_CAM_MOUSE_CAM && OMM_CAMERA_CLASSIC && sCurrPlayMode != 2);
    return SDL_GetRelativeMouseState(&gMouseX, &gMouseY);
}

//
// Controller API
//

static void controller_sdl_add_binds(u32 mask, u32 *buttons) {
    for (s32 i = 0; i != MAX_BINDS; ++i) {
        if (buttons[i] < VK_BASE_SDL_GAMEPAD + VK_SIZE) {
            if (buttons[i] >= VK_BASE_SDL_MOUSE) {
                u64 cm = CM_MAPPING(buttons[i] - VK_BASE_SDL_MOUSE, mask);
                omm_array_add(sMouseMapping, u64, cm);
            } else if (buttons[i] >= VK_BASE_SDL_GAMEPAD) {
                u64 cm = CM_MAPPING(buttons[i] - VK_BASE_SDL_GAMEPAD, mask);
                omm_array_add(sJoystickMapping, u64, cm);
            }
        }
    }
}

static void controller_sdl_bind(void) {
    omm_array_clear(sJoystickMapping);
    omm_array_clear(sMouseMapping);
    controller_sdl_add_binds(A_BUTTON, gOmmControlsButtonA);
    controller_sdl_add_binds(B_BUTTON, gOmmControlsButtonB);
    controller_sdl_add_binds(X_BUTTON, gOmmControlsButtonX);
    controller_sdl_add_binds(Y_BUTTON, gOmmControlsButtonY);
    controller_sdl_add_binds(START_BUTTON, gOmmControlsButtonStart);
    controller_sdl_add_binds(L_TRIG, gOmmControlsTriggerL);
    controller_sdl_add_binds(R_TRIG, gOmmControlsTriggerR);
    controller_sdl_add_binds(Z_TRIG, gOmmControlsTriggerZ);
    controller_sdl_add_binds(U_CBUTTONS, gOmmControlsCUp);
    controller_sdl_add_binds(D_CBUTTONS, gOmmControlsCDown);
    controller_sdl_add_binds(L_CBUTTONS, gOmmControlsCLeft);
    controller_sdl_add_binds(R_CBUTTONS, gOmmControlsCRight);
    controller_sdl_add_binds(U_JPAD, gOmmControlsDUp);
    controller_sdl_add_binds(D_JPAD, gOmmControlsDDown);
    controller_sdl_add_binds(L_JPAD, gOmmControlsDLeft);
    controller_sdl_add_binds(R_JPAD, gOmmControlsDRight);
    controller_sdl_add_binds(STICK_UP, gOmmControlsStickUp);
    controller_sdl_add_binds(STICK_DOWN, gOmmControlsStickDown);
    controller_sdl_add_binds(STICK_LEFT, gOmmControlsStickLeft);
    controller_sdl_add_binds(STICK_RIGHT, gOmmControlsStickRight);
}

static void controller_sdl_init(void) {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return;
    }
    controller_sdl_bind();
    controller_sdl_get_mouse_state();
    sControllerInited = true;
}

static void controller_sdl_read(OSContPad *pad) {
    if (OMM_LIKELY(sControllerInited)) {

        // Mouse
        u32 mouseButtons = controller_sdl_get_mouse_state();
        omm_array_for_each(sMouseMapping, pcm) {
            u64 cm = pcm->as_u64;
            if (mouseButtons & SDL_BUTTON(CM_BUTTON(cm))) {
                pad->button |= CM_MASK(cm);
            }
        }
        sMouseLastButton = (sMouseButtonsDown ^ mouseButtons) & mouseButtons;
        sMouseButtonsDown = mouseButtons;

        // Joystick
        SDL_GameControllerUpdate();
        if (sController && !SDL_GameControllerGetAttached(sController)) {
            SDL_GameControllerClose(sController);
            sController = NULL;
        }
        if (!sController) {
            for (s32 i = 0; i < SDL_NumJoysticks(); ++i) {
                if (SDL_IsGameController(i)) {
                    sController = SDL_GameControllerOpen(i);
                }
            }
        }
        if (sController) {

            // Get axes values
            s16 lx = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_LEFTX);
            s16 ly = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_LEFTY);
            s16 rx = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_RIGHTX);
            s16 ry = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_RIGHTY);
            s16 lt = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            s16 rt = SDL_GameControllerGetAxis(sController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

            // Update buttons states
            for (s32 i = 0; i != SDL_CONTROLLER_BUTTON_MAX; ++i) {
                bool down = SDL_GameControllerGetButton(sController, i);
                controller_sdl_update_joystick_button(i, down);
            }
            controller_sdl_update_joystick_button(VK_LTRIGGER - VK_BASE_SDL_GAMEPAD, lt > 0x1C00);
            controller_sdl_update_joystick_button(VK_RTRIGGER - VK_BASE_SDL_GAMEPAD, rt > 0x1C00);

            // Register button presses
            u32 buttonsDown = 0;
            omm_array_for_each(sJoystickMapping, pcm) {
                u64 cm = pcm->as_u64;
                if (sJoystickButtonsDown[CM_BUTTON(cm)]) {
                    buttonsDown |= CM_MASK(cm);
                }
            }
            pad->button |= buttonsDown;

            // Stick
            switch (buttonsDown & STICK_XMASK) {
                case STICK_LEFT:  pad->stick_x = -0x7F; break;
                case STICK_RIGHT: pad->stick_x = +0x7F; break;
            }
            switch (buttonsDown & STICK_YMASK) {
                case STICK_DOWN:  pad->stick_y = -0x7F; break;
                case STICK_UP:    pad->stick_y = +0x7F; break;
            }

            // C-buttons
            if (rx < -0x4000) pad->button |= L_CBUTTONS;
            if (rx > +0x4000) pad->button |= R_CBUTTONS;
            if (ry < -0x4000) pad->button |= U_CBUTTONS;
            if (ry > +0x4000) pad->button |= D_CBUTTONS;

            // Left stick
            u32 lsMag2 = sqr(lx) + sqr(ly);
            u32 lsDza2 = sqr(configStickDeadzone * DEADZONE_STEP);
            if (lsMag2 > lsDza2) {
                pad->stick_x = clamp_s(+lx / 0x100, -0x7F, +0x7F);
                pad->stick_y = clamp_s(-ly / 0x100, -0x7F, +0x7F);
            }

            // Right stick
            u32 rsMag2 = sqr(rx) + sqr(ry);
            u32 rsDza2 = sqr(configStickDeadzone * DEADZONE_STEP);
            if (rsMag2 > rsDza2) {
                pad->ext_stick_x = clamp_s(+rx / 0x100, -0x7F, +0x7F);
                pad->ext_stick_y = clamp_s(-ry / 0x100, -0x7F, +0x7F);
            }
        }
    }
}

static u32 controller_sdl_rawkey(void) {
    if (sJoystickLastButton != VK_INVALID) {
        u32 button = sJoystickLastButton;
        sJoystickLastButton = VK_INVALID;
        return button;
    }
    for (s32 i = 1; i != 8 * sizeof(sMouseLastButton); ++i) {
        if (sMouseLastButton & SDL_BUTTON(i)) {
            u32 button = VK_OFS_SDL_MOUSE + i;
            sMouseLastButton = 0;
            return button;
        }
    }
    return VK_INVALID;
}

static void controller_sdl_shutdown(void) {
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER)) {
        if (sController) {
            SDL_GameControllerClose(sController);
            sController = NULL;
        }
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    }
    sControllerInited = false;
}

struct ControllerAPI controller_sdl = {
    VK_BASE_SDL_GAMEPAD,
    controller_sdl_init,
    controller_sdl_read,
    controller_sdl_rawkey,
    NULL,
    NULL,
    controller_sdl_bind,
    controller_sdl_shutdown
};

#endif // CAPI_SDL2
