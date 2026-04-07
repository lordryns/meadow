#ifndef MEADOW_UTILS_H
#define MEADOW_UTILS_H

#include "meadow.h"
#include <X11/X.h>

#define ALT_KEY Mod1Mask
#define SUPER_KEY Mod4Mask
#define SHIFT_KEY ShiftMask

void grab_key_with_string(wm_t *wm, const char *k, unsigned int mod);
KeyCode gen_keycode_from_string(wm_t *wm, const char *k, unsigned int mod);

#endif // ! MEADOW_UTILS_H
