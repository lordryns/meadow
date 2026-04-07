#include "meadow.h"
#include "utils.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
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

  grab_key_with_string(wm, "q", Mod4Mask);
  XSync(wm->display, 0);
}

void handle_key_events(wm_t *wm, KeySym ksym) {
  switch (ksym) {
  case XK_q: {
    exit(0);
  }
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
      handle_key_events(&wm, XLookupKeysym(&e.xkey, 0));
      break;
    }
    }
    XSync(wm.display, 0);
  }
  return 0;
}
