// Return just cpu load, nothing else

// header files 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <assert.h>

#include <sysmonitor-data.c>

int main(int argc, char * argv[]) {

  // cpu idle
  unsigned long long idle1;
  unsigned long long idle2;
  idle1 = sysmonitor_cpu_idle();
  sleep(1);
  idle2 = sysmonitor_cpu_idle();
  printf("%1Ld",idle2-idle1);

  return 0;
}

