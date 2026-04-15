#include "client.h"
#include "meadow.h"
#include "utils.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

client_t *grab_client_window(wm_t *wm, XEvent *e) {
  XMapRequestEvent *re = &e->xmaprequest;
  Window win = re->window;
  XWindowAttributes win_attr = get_window_attributes(wm->display, win);

  if (win_attr.override_redirect)
    return NULL;

  XSelectInput(wm->display, win,
               EnterWindowMask | LeaveWindowMask | ExposureMask);

  client_t *c = malloc(sizeof(client_t));
  c->window = win;
  c->width = win_attr.width;
  c->height = win_attr.height;
  c->x = win_attr.x;
  c->y = win_attr.y;
  c->frame = XCreateSimpleWindow(wm->display, wm->root, c->x, c->y, c->width,
                                 c->height, 2, WhitePixel(wm->display, 0),
                                 BlackPixel(wm->display, 0));
  XSelectInput(wm->display, c->frame,
               EnterWindowMask | LeaveWindowMask | ButtonPressMask |
                   ButtonReleaseMask);

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

client_t *find_client_using_window(client_t **head, Window win) {
  for (client_t *it = *head; it != NULL; it = it->next) {
    if (it->window == win)
      return it;
  }

  return NULL;
}

client_t *find_client_using_frame(client_t **head, Window frame) {
  for (client_t *it = *head; it != NULL; it = it->next) {
    if (it->window == frame)
      return it;
  }

  return NULL;
}

void remove_client_from_linked_list(client_t **head, Window window) {
  client_t *client = find_client_using_window(head, window);
  if (client == NULL) {
    printf("unable to find client struct for window (0x%lu)\n", window);
    return;
  }
  client_t *prev = NULL;
  client_t *curr = *head;

  while (curr != NULL && curr->window != client->window) {
    prev = curr;
    curr = curr->next;
  }

  if (curr == NULL)
    return;

  if (prev == NULL) {
    *head = curr->next;
  } else {
    prev->next = curr->next;
  }

  free(curr);
}
