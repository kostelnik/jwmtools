// Simple library for detecting if some application is already running

#ifndef LIBONCE_H
#define LIBONCE_H

#include <stdlib.h>

int once_process_count(char * name); // return 1 if no other process used "name"

#endif

