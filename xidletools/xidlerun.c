// Wait in endless loop, monitor user idle time, when it exceed certain 
// value run specified command. This program is used to trigger slock
// after some time of user inactivity.

#include <X11/extensions/scrnsaver.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  XScreenSaverInfo *info = XScreenSaverAllocInfo();
  Display *display = XOpenDisplay(NULL);
  int event, error, seconds = 10;
  
  // FIXME: get command from commandline (param 1)
  // FIXME: get seconds from commandline (param 2, default 300)

  if (!display) {
    fprintf(stderr,"xidletime: cannot open display\n");
    return 1;
  }

  if (!XScreenSaverQueryExtension(display, &event, &error)) {
    fprintf(stderr,"xidletime: X screen sever not supported\n");
    return 2;
  }

  // TODO: perhaps make it interruptable by TERM signal?
  while (1) {
  
    // get time
    if (!XScreenSaverQueryInfo(display, DefaultRootWindow(display), info)) {
      fprintf(stderr,"xidletime: X screen sever query info failed\n");
      return 3;
    }
    printf("%lu\n", info->idle);

    // is it bigger that 10s? run something
    if (info->idle > seconds*1000) {
      printf("--> executing command\n");
      system("sleep 1m");
      
      // make sure it won't accidentaly run just after
      sleep(seconds);
    }
    
    // sleep 1 second
    sleep(1);
  }
  
  // done
  XCloseDisplay(display);
  free(info);
  return 0;
}