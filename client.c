#include "client.h"
#include "meadow.h"
#include "utils.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdlib.h>

client_t *grab_client_window(wm_t *wm, XEvent *e) {
  XMapRequestEvent *re = &e->xmaprequest;
  Window win = re->window;
  XWindowAttributes win_attr = get_window_attributes(wm->display, win);

  XSelectInput(wm->display, win,
               EnterWindowMask | LeaveWindowMask | ExposureMask);

  client_t *c = malloc(sizeof(client_t));
  c->window = win;
  c->width = win_attr.width;
  c->height = win_attr.height;
  c->x = 100;
  c->y = 0;
  c->frame = XCreateSimpleWindow(wm->display, wm->root, c->x, c->y, c->width,
                                 c->height, 2, WhitePixel(wm->display, 0),
                                 BlackPixel(wm->display, 0));
  XSelectInput(wm->display, c->frame,
               ButtonPressMask | EnterWindowMask | StructureNotifyMask |
                   PointerMotionMask | ButtonPressMask | ButtonReleaseMask);

  c->next = wm->window_list_head;
  wm->window_list_head = c;

  return c;
}

void render_client(wm_t *wm, client_t *c) {
  XReparentWindow(wm->display, c->window, c->frame, 0, 0);
  XMapWindow(wm->display, c->frame);
  XMapWindow(wm->display, c->window);

  XSync(wm->display, 0);

  for (client_t *it = wm->window_list_head; it != NULL; it = it->next) {
  }
}

void remove_client_from_linked_list(client_t **head, client_t *client) {}
