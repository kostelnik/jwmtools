// Decrease alsa volume by 1%

#include <stdio.h>
#include <stdlib.h>

#include "alsa.h"
#include "alsa.c"

int main (int argc, char * argv[]) {
  return alsa_volume_down();
}
