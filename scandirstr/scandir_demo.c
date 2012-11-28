// demo which watch directory for changes, not recursive

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include "../s/s.h"
#include "../s/s.c"
#include "scandirstr.h"
#include "scandirstr.c"

int main(int argc, char * argv[]) {

  int i,c;
  char *s = NULL;
  
  /*
  s = scandirstr("/home/lynx/testdir");
  printf("%s\n",s);
  SFree(s);
  */
  
  //s = SCreateSize(1);
  for (i=0; i<10; i++) {
    c = scandirstrchanged("/sys/block",&s);
    printf("%d\n",c);
    //printf("%d s=%s\n",c,s);
    sleep(1);
  }
  SFree(s);
  
  return 0;
}