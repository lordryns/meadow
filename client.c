#include "client.h"
#include "meadow.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

client_t *grab_client_window(wm_t *wm, XEvent *e) {
  XMapRequestEvent *re = &e->xmaprequest;
  Window win = re->window;

  client_t *c = malloc(sizeof(client_t));
  c->window = win;
  c->width = 900;
  c->height = 600;
  if (wm->window_list_head != NULL) {
    c->x = wm->window_list_head->x + c->width;
  } else {
    c->x = 0;
  }
  c->y = 0;
  c->next = wm->window_list_head;
  wm->window_list_head = c;

  printf("grab successful! g = %p\n", (void *)c);
  return c;
}

void render_client(wm_t *wm, client_t *c) {
  Window win = XCreateSimpleWindow(wm->display, wm->root, c->x, c->y, c->width,
                                   c->height, 2, WhitePixel(wm->display, 0),
                                   BlackPixel(wm->display, 0));

  XReparentWindow(wm->display, c->window, win, 0, 0);
  XMapWindow(wm->display, win);
  XMapWindow(wm->display, c->window);
  XSetInputFocus(wm->display, c->window, RevertToParent, CurrentTime);

  printf("render successful!\n");
  for (client_t *it = wm->window_list_head; it != NULL; it = it->next) {
    printf("it = %p\n", (void *)it);
  }
}
