/*
Alsa interface using "amixer set Master 1%+" or "amixer set Master 1%-" and detecting current volume
*/

#ifndef ALSA_H
#define ALSA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>

#include "../s/s.h"
#include "../s/s.c"

#define ALSA_MAX_LEN 1000

int alsa_get_volume(void);
int alsa_set_volume(int percent);
int alsa_change_volume(int delta_percent);
int alsa_volume_up(void);
int alsa_volume_down(void);

#endif // ALSA_H
