// auxiliary function for retrieving directory listing
// bash equivalent: X=`ls path`

#ifndef SCANDIRSTR_C
#define SCANDIRSTR_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include "s.h"
#include "s.c"

char * scandirstr(char * path) {
  // return \n separated string of directory listing, e.g "foo\nbar\nsomething.txt\0"
  struct dirent **namelist;
  int i,n;
  char *s = NULL, *o;
  n = scandir(path, &namelist, 0, alphasort);
  if (n < 0)
    perror("error scandir");
  else {
    //s = SCreateSize(5000);
    for (i=0; i<n; i++) {
      //printf("%d: %s\n",i,namelist[i]->d_name);
      if ((strcmp(namelist[i]->d_name,".")==0)||(strcmp(namelist[i]->d_name,"..")==0)) {
        free(namelist[i]);
        continue;
      }
      // append entry to result
      if (!s)
        o = SCreate(namelist[i]->d_name);
      else {
        o = SCreateAppend(s,namelist[i]->d_name);
        SFree(s);
      }
      s = o;
      // append pipe | to result
      if (i != n - 1) {
        o = SCreateAppend(s,"|");
        SFree(s);
        s = o;
      }
      free(namelist[i]);
    }
    free(namelist);
  }
  return s;
}

int scandirstrchanged(char * path, char ** old) {
  // return 1 if path changed since last time
  char *n = scandirstr(path);
  if (*old == NULL)
    *old = SCreateSize(1);
  //char *s = *old;
  int b = 0;
  //printf(" AA\n");
  //printf("   n   = %s\n",n);
  //printf("   old = %s\n",*old);
  if (strcmp(*old,n)!=0)
    b = 1;
  //printf(" BB\n");
  free(*old);
  *old = n;
  return b;
}

#endif
