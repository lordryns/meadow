#ifndef MEADOW_H
#define MEADOW_H

#include <X11/Xlib.h>

typedef struct {
  Display *display;
  Window root;
  int width, height;
} wm_t;

void initialise_wm(wm_t *wm);

#endif // !MEADOW_H
