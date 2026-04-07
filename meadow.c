#include "meadow.h"
#include "client.h"
#include "config.h"
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

  // setting to AnyModifier will ensure that a request will be sent regardless
  // of the modifier used
  grab_key_with_string(wm, WM_EXIT_KEY, AnyModifier);
  grab_key_with_string(wm, APP_LAUNCHER_KEY, AnyModifier);
  XSync(wm->display, 0);

  wm->window_list_head = NULL;
}

void handle_key_events(wm_t *wm, XEvent *e) {
  KeyCode kcode = e->xkey.keycode;
  unsigned int state = e->xkey.state;
  // note to self that this is solely for checking and the modifier is actually
  // important
  KeyCode quit_kcode = gen_keycode_from_string(wm, WM_EXIT_KEY, MODIFIER);
  KeyCode app_launcher_kcode =
      gen_keycode_from_string(wm, APP_LAUNCHER_KEY, MODIFIER);
  if (state == MODIFIER) {
    if (kcode == quit_kcode) {
      exit(0);
    } else if (kcode == app_launcher_kcode) {
      system(APP_LAUNCHER);
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
      handle_key_events(&wm, &e);
      break;
    }
    case MapRequest: {
      client_t *client = grab_client_window(&wm, &e);
      render_client(&wm, client);
    }
    }
    XSync(wm.display, 0);
  }
  return 0;
}
