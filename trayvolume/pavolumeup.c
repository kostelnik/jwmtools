// Simple tool that increase volume a little bit using pulse audio

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include <pulseaudio_volume.c>

int main (int argc, char *argv[]) {
  long int v = pa_get_volume();
  pa_set_volume(v+2000);
  return 0;
}
