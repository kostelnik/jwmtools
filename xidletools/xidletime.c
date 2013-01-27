// print miliseconds since last user interaction in X

#include <X11/extensions/scrnsaver.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  XScreenSaverInfo *info = XScreenSaverAllocInfo();
  Display *display = XOpenDisplay(NULL);
  int event, error;

  if (!display) {
    fprintf(stderr,"xidletime: cannot open display\n");
    return 1;
  }

  // FIXME: following 2 functions allocate and do not release 12 bytes of memory (luckily only once)
  if (!XScreenSaverQueryExtension(display, &event, &error)) {
    fprintf(stderr,"xidletime: X screen sever not supported\n");
    return 2;
  }

  // get time
  if (!XScreenSaverQueryInfo(display, DefaultRootWindow(display), info)) {
    fprintf(stderr,"xidletime: X screen sever query info failed\n");
    return 3;
  }

  // print it
  printf("%lu\n", info->idle);

  // done
  XCloseDisplay(display);
  free(info);
  return 0;
}