// auxiliary function for retrieving directory listing and detecting if it changed
// bash equivalent: X=`ls path`; OLD=$X; ... if [ "$X" != "$OLD" ]; then ...

#ifndef SCANDIRSTR_H
#define SCANDIRSTR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include "s.h"

char * scandirstr(char * path);
int scandirstrchanged(char * path, char ** old);

#endif
