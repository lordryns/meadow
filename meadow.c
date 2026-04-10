#include "meadow.h"
#include "client.h"
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

  XGrabServer(wm->display);

  XDefineCursor(wm->display, wm->root, wm->cursor);
  XGrabButton(wm->display, Button1, 0, wm->root, False, ButtonPressMask,
              GrabModeSync, GrabModeAsync, wm->root, wm->cursor);

  XClearWindow(wm->display, wm->root);
  XSelectInput(wm->display, wm->root,
               SubstructureNotifyMask | SubstructureRedirectMask |
                   ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

  // setting to AnyModifier will ensure that a request will be sent regardless
  // of the modifier used
  grab_key_with_string(wm, WM_EXIT_KEY, MODIFIER);
  grab_key_with_string(wm, APP_LAUNCHER_KEY, MODIFIER);
  grab_key_with_string(wm, "c", MODIFIER);
  grab_key_with_string(wm, "Left", MODIFIER);
  grab_key_with_string(wm, "Right", MODIFIER);
  grab_key_with_string(wm, "Up", MODIFIER);
  grab_key_with_string(wm, "Down", MODIFIER);
  grab_key_with_string(wm, "r", MODIFIER);
  grab_key_with_string(wm, "m", MODIFIER);
  XSync(wm->display, 0);

  wm->window_list_head = NULL;
  wm->move_client = false;
  wm->focused_client = NULL;
  wm->mod_active = false;

  XUngrabServer(wm->display);
}

void handle_key_events(wm_t *wm, XEvent *e) {
  KeyCode kcode = e->xkey.keycode;
  unsigned int state = e->xkey.state;
  // note to self that this is solely for checking and the modifier is actually
  // important
  KeyCode quit_kcode = gen_keycode_from_string(wm, WM_EXIT_KEY, MODIFIER);
  KeyCode app_launcher_kcode =
      gen_keycode_from_string(wm, APP_LAUNCHER_KEY, MODIFIER);
  KeyCode kill_client_kcode = gen_keycode_from_string(wm, "c", MODIFIER);
  KeyCode resize_client_kcode = gen_keycode_from_string(wm, "r", MODIFIER);
  KeyCode move_client_kcode = gen_keycode_from_string(wm, "m", MODIFIER);
  KeyCode left_client_kcode = gen_keycode_from_string(wm, "Left", MODIFIER);
  KeyCode right_client_kcode = gen_keycode_from_string(wm, "Right", MODIFIER);
  KeyCode up_client_kcode = gen_keycode_from_string(wm, "Up", MODIFIER);
  KeyCode down_client_kcode = gen_keycode_from_string(wm, "Down", MODIFIER);

  if (state == MODIFIER) {
    if (kcode == quit_kcode) {
      exit(0);
    } else if (kcode == app_launcher_kcode) {
      system(APP_LAUNCHER);
    } else if (kcode == kill_client_kcode) {
      if (wm->focused_client != NULL &&
          wm->focused_client->window != wm->root) {
        XDestroyWindow(wm->display, wm->focused_client->window);
      }
    } else if (kcode == resize_client_kcode) {
      wm->resize_client = !wm->resize_client;
      wm->move_client = false;
    } else if (kcode == move_client_kcode) {
      wm->move_client = !wm->move_client;
      wm->resize_client = false;
    }

    if (wm->move_client && wm->focused_client != NULL) {
      if (kcode == left_client_kcode) {
        wm->focused_client->x -= 10;
      } else if (kcode == right_client_kcode) {
        wm->focused_client->x += 10;
      } else if (kcode == up_client_kcode) {
        wm->focused_client->y -= 10;
      } else if (kcode == down_client_kcode) {
        wm->focused_client->y += 10;
      }

      XMoveWindow(wm->display, wm->focused_client->frame, wm->focused_client->x,
                  wm->focused_client->y);
    }

    if (wm->resize_client && wm->focused_client != NULL) {
      if (kcode == left_client_kcode) {
        wm->focused_client->width -= 10;
      } else if (kcode == right_client_kcode) {
        wm->focused_client->width += 10;
      } else if (kcode == up_client_kcode) {
        wm->focused_client->height -= 10;
      } else if (kcode == down_client_kcode) {
        wm->focused_client->height += 10;
      }
      XResizeWindow(wm->display, wm->focused_client->window,
                    wm->focused_client->width, wm->focused_client->height);
      XResizeWindow(wm->display, wm->focused_client->frame,
                    wm->focused_client->width, wm->focused_client->height);
    }
  }
}

void on_window_destroy_event(wm_t *wm, XDestroyWindowEvent *ev) {
  printf("destroyed =  0x%lx\n\n", ev->window);
  /*for (client_t *it = wm->window_list_head; it != NULL; it = it->next) {
    printf("win =  0x%lx | frame =  0x%lx\n", it->window, it->frame);
    if (it->window == ev->window) {
      printf("destroy event detected!\n");
      XDestroyWindow(wm->display, it->frame);
    }
  } */

  printf("c_struct = %p\n", (void *)wm->focused_client);
  if (wm->focused_client != NULL) {
    printf("window = 0x%lx\n", wm->focused_client->window);
    printf("frame = 0x%lx\n", wm->focused_client->frame);
    if (ev->window == wm->focused_client->window) {
      XDestroyWindow(wm->display, wm->focused_client->frame);
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
      break;
    }
    case ConfigureRequest: {
      XConfigureRequestEvent *ev = &e.xconfigurerequest;
      XWindowChanges changes;
      changes.x = ev->x;
      changes.y = ev->y;
      changes.width = ev->width;
      changes.height = ev->height;
      changes.border_width = ev->border_width;
      changes.sibling = ev->above;
      changes.stack_mode = ev->detail;
      XConfigureWindow(wm.display, ev->window, ev->value_mask, &changes);

      XConfigureEvent ce;
      ce.x = ev->x;
      ce.y = ev->y;
      ce.width = ev->width;
      ce.height = ev->height;
      ce.border_width = ev->border_width;
      ce.above = None;
      ce.override_redirect = False;

      XSendEvent(wm.display, ev->window, False, StructureNotifyMask,
                 (XEvent *)&ce);
      break;
    }
    case UnmapNotify: {
      break;
    }
    case EnterNotify: {
      for (client_t *it = wm.window_list_head; it; it = it->next) {
        if (it->window == e.xcrossing.window) {
          wm.hovered_client = it;
        }
        if (wm.focused_client != NULL &&
            it->window != wm.focused_client->window) {
          XSetWindowBorder(wm.display, it->frame, WhitePixel(wm.display, 0));
        }
      }
      // XSetInputFocus(wm.display, e.xcrossing.window, RevertToParent,
      //              CurrentTime);
      break;
    }
    case DestroyNotify: {
      printf("destroy notify sent!\n");
      on_window_destroy_event(&wm, &e.xdestroywindow);
      break;
    }
    case ButtonPress: {
      if (wm.hovered_client != NULL && wm.hovered_client->window != wm.root) {
        wm.focused_client = wm.hovered_client;
      }
      if (wm.focused_client != NULL) {
        XSetWindowBorder(wm.display, wm.focused_client->frame, 0x5E85BF);
        XRaiseWindow(wm.display, wm.focused_client->frame);

        XSetInputFocus(wm.display, wm.focused_client->window, RevertToParent,
                       CurrentTime);
        XAllowEvents(wm.display, ReplayPointer, CurrentTime);
      }

      if (wm.hovered_client != NULL)
        printf("hovered = 0x%lu\n", wm.hovered_client->window);
      if (wm.focused_client != NULL)
        printf("focused = 0x%lu\n\n", wm.focused_client->window);

      break;
    }

    case MotionNotify: {
      wm.mouse_x = e.xmotion.x_root;
      wm.mouse_y = e.xmotion.y_root;
      break;
    }
    }

    XSync(wm.display, 0);
  }
  return 0;
}
