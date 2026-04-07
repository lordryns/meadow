#ifndef MEADOW_CLIENT_H
#define MEADOW_CLIENT_H

#include "meadow.h"
#include <X11/Xlib.h>

typedef struct client_t {
  Window window;
  int x, y;
  int width, height;
  struct client_t *next;
} client_t;

client_t *grab_client_window(wm_t *wm, XEvent *e);
void render_client(wm_t *wm, client_t *c);

#endif // !MEADOW_CLIENT_H
