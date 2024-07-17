// Alsa interface using "amixer set Master 1%+" or "amixer set Master 1%-" and detecting current volume

#ifndef ALSA_C
#define ALSA_C

//#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>

#include "../s/s.h"
#include "../s/s.c"

int alsa_get_volume(void) {
  // return master volume in percents
  /*
  $ amixer get Master
  Simple mixer control 'Master',0
    Capabilities: pvolume pvolume-joined pswitch pswitch-joined penum
    Playback channels: Mono
    Limits: Playback 0 - 64
    Mono: Playback 64 [100%] [0.00dB] [on]
  */

  // local variables
  char *line = SCreateSize(ALSA_MAX_LEN);
  char *s, *t;
  int /*read, */a, b, c, r=-1;

  // execute pipe
  FILE *f = popen("amixer get Master","r");
  if (!f)
    return -1;
  
  // parse output from pipe
  while (!feof(f)) {

    // read one line
    fgets(line,ALSA_MAX_LEN,f);
    /*read = strlen(line);*/
    if (feof(f))
      break;
    //printf("LINE(%d)=%s\n",read,line);
    
    // find first [ and ] in 
    c = SPos(line,"Mono: Playback");
    if (c < 0)
      continue;
    a = SPos(line,"[");
    b = SPos(line,"]");
    if ( (a<0) || (b<0) )
      continue;
    s = SCreateBetween(line,a+1,b-1);
    t = SCreateReplace(s,"%","");
    r = SInt(t); 
    //printf("volume=%d\n",r);
    SFree(s);
    SFree(t);
  }

  SFree(line);

  // done
  pclose(f);
  return r;
}

int alsa_volume_up(void) {
  // change volume up by 1%
  system("amixer set Master 1%+");
  return alsa_get_volume();
}

int alsa_volume_down(void) {
  // change volume up by 1%
  system("amixer set Master 1%-");
  return alsa_get_volume();
}

int alsa_change_volume(int delta_percent) {
  // change volume up or down by few percents, return new volume value
  char *s = SCreateSize(1000);
  if (delta_percent > 0)
    sprintf(s,"amixer set Master %d%%+",delta_percent);
  else
    sprintf(s,"amixer set Master %d%%-",-delta_percent);
  printf("executing: %s\n",s);
  system(s);
  SFree(s);
  return alsa_get_volume();
}

int alsa_set_volume(int percent) {
  // set volume to specified amount of percents, return new volume value
  char *s = SCreateSize(1000);
  sprintf(s,"amixer set Master %d%%",percent);
  printf("executing: %s\n",s);
  system(s);
  SFree(s);
  return alsa_get_volume();
}

#endif // ALSA_C
