#include "utils.h"
#include <X11/Xlib.h>

void grab_key_with_string(wm_t *wm, const char *k, unsigned int mod) {
  KeySym ksym = XStringToKeysym(k);
  KeyCode code = XKeysymToKeycode(wm->display, ksym);
  XGrabKey(wm->display, code, mod, wm->root, True, GrabModeSync, GrabModeAsync);
}
