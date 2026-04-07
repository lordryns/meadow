#include "meadow.h"
#include "config.h"
#include "utils.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>

void initialse_wm(wm_t *wm) {
  wm->display = XOpenDisplay(NULL);
  wm->root = XDefaultRootWindow(wm->display);
  wm->cursor = XCreateFontCursor(wm->display, XC_left_ptr);

  XDefineCursor(wm->display, wm->root, wm->cursor);
  XGrabButton(wm->display, Button1, 0, wm->root, False, ButtonPressMask,
              GrabModeSync, GrabModeAsync, wm->root, wm->cursor);

  XClearWindow(wm->display, wm->root);
  XSelectInput(wm->display, wm->root,
               SubstructureNotifyMask | SubstructureRedirectMask);

  // setting to AnyModifier will ensure that a request will be sent regardless
  // of the modifier used
  grab_key_with_string(wm, "q", AnyModifier);
  grab_key_with_string(
      wm, "space",
      Mod4Mask); // this request will only be sent if the super key is used
  XSync(wm->display, 0);
}

void handle_key_events(wm_t *wm, XEvent *e) {
  // note to self that this is solely for checking and the modifier is actually
  // important
  KeyCode quit_kcode = gen_keycode_from_string(wm, "q", Mod4Mask);
  KeyCode debug_kcode = gen_keycode_from_string(wm, "q", AnyModifier);
  if (e->xkey.state == Mod4Mask) {
    if (e->xkey.keycode == quit_kcode) {
      exit(0);
    }
  }
  if (e->xkey.keycode == debug_kcode) {
    printf("debug key hit!\n");
  }
}

int main(void) {
  wm_t wm;
  initialse_wm(&wm);

  XEvent e;
  while (1) {
    XNextEvent(wm.display, &e);
    switch (e.type) {
    case KeyPress: {
      handle_key_events(&wm, &e);
      break;
    }
    }
    XSync(wm.display, 0);
  }
  return 0;
}
