#ifndef MEADOW_CLIENT_H
#define MEADOW_CLIENT_H

#include "meadow.h"
#include <X11/Xlib.h>

typedef struct client_t {
  Window window;
  Window frame;
  int x, y;
  int width, height;
  struct client_t *next;
} client_t;

client_t *grab_client_window(wm_t *wm, XEvent *e);
void render_client(wm_t *wm, client_t *c);

void remove_client_from_linked_list(client_t **head, Window window);

client_t *find_client_using_window(client_t **head, Window win);
client_t *find_client_using_frame(client_t **head, Window frame);

#endif // !MEADOW_CLIENT_H
