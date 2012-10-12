// Move mouse cursor to desired location of screen

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <X11/Xlib.h>

void movemousecursor(int x, int y) {
  // move cursor to [x,y]
  Display *dpy;
  if(!(dpy = XOpenDisplay(0))) {
    fprintf(stderr, "movemousecursor: cannot open display\n");
    exit(EXIT_FAILURE);
  }
  if (x == -1)
    x = DisplayWidth(dpy, DefaultScreen(dpy));
  if (y == -1)
    y = DisplayHeight(dpy, DefaultScreen(dpy));
  fprintf(stderr,"moving to %d, %d\n",x,y);
  XWarpPointer(dpy, None, RootWindow(dpy, DefaultScreen(dpy)), 0, 0, 0, 0, x, y);
  XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
  XCloseDisplay(dpy);
}

int main(int argc, char *argv[]) {
  int x,y;

  // movemousecursor <x> <y>
  printf("argc=%d\n",argc);
  if ( (argc == 3) && (sscanf(argv[1], "%d", &x) == 1) && (sscanf(argv[2], "%d", &y) == 1) ) {
    movemousecursor(x,y);
    exit(EXIT_SUCCESS);
  }

  // without parameters move to bottom right corner
  //printf("%d\n",DisplayWidth(display, screen));
  movemousecursor(-1,-1);
  fputs("usage: movemousecursor <x> <y>\n", stderr);
  exit(EXIT_FAILURE);

  return 0;
}
