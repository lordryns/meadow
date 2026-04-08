#ifndef MEADOW_H
#define MEADOW_H

#include <X11/X.h>
#include <X11/Xlib.h>

// this is a trick i found out worked so i used it
// i couldn't import the client_t from client.h because of circular import
// limitations but apparently this works
struct client_t;

typedef struct {
  Display *display;
  Window root;
  Cursor cursor;
  int width, height;
  struct client_t *window_list_head;
} wm_t;

void initialise_wm(wm_t *wm);

void handle_key_events(wm_t *wm, XEvent *e);

#endif // !MEADOW_H
