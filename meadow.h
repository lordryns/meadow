#ifndef MEADOW_H
#define MEADOW_H

#include <X11/X.h>
#include <X11/Xlib.h>

typedef struct {
  Display *display;
  Window root;
  Cursor cursor;
  int width, height;
} wm_t;

void initialise_wm(wm_t *wm);

void handle_key_events(wm_t *wm, XEvent *e);

#endif // !MEADOW_H
