// Wait in endless loop, monitor user idle time, when it exceed certain 
// value run specified command. This program is used to trigger slock
// after some time of user inactivity.

#include <X11/extensions/scrnsaver.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "libonce.h"
#include "libonce.c"

// FIXME: this program was intended to be universal, e.g. xidlerun -s 60 -e ~/bin/mycoolscreensaver but currently it only support fixed time and fixed program
// FIXME: get command from commandline
// FIXME: get seconds from commandline
// FIXME: get precommand from commandline or use pidof or something to detect if it is running
#define XIDLERUN_INTERVAL 300

#define XIDLERUN_PRECOMMAND NULL
//"killall slock"

#define XIDLERUN_COMMAND "/opt/jwmtools/front/bin/slock"

int main(int argc, char *argv[]) {
  XScreenSaverInfo *info = XScreenSaverAllocInfo();
  Display *display = XOpenDisplay(NULL);
  int event, error, seconds = XIDLERUN_INTERVAL;
  
  if (!display) {
    fprintf(stderr,"xidletime: cannot open display\n");
    return 1;
  }

  if (!XScreenSaverQueryExtension(display, &event, &error)) {
    fprintf(stderr,"xidletime: X screen sever not supported\n");
    return 2;
  }
  printf("xidlerun interval is %d seconds\n",XIDLERUN_INTERVAL);

  // TODO: perhaps make it interruptable by TERM signal?
  while (1) {
  
    // get time
    if (!XScreenSaverQueryInfo(display, DefaultRootWindow(display), info)) {
      fprintf(stderr,"xidletime: X screen sever query info failed\n");
      return 3;
    }
    //printf("%lu\n", info->idle);

    // is it bigger that 10s? run something
    if (info->idle > seconds*1000) {
      // do not execute program if it is already running
      /*
      if (strlen(XIDLERUN_PRECOMMAND) > 0) {
        printf("--> executing precommand %s\n",XIDLERUN_PRECOMMAND);
        system(XIDLERUN_PRECOMMAND);
        //sleep(2);
      }
      */
      
      printf("--> executing command %s\n",XIDLERUN_COMMAND);

      if (once_process_count("slock",1) == 0) // FIXME: this is not good
        system(XIDLERUN_COMMAND);
      //system("sleep 1m");
      
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
