// Console only HW info collecting tool

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

  // cpu IDLE CORES load L1 L5 L15 mem USED CACHED SWAP net RECV TRAN disk KBPS
  // cpu %Ld %d load %1.2f %1.2f %1.2f mem %d %d %d net %Ld %Ld disk %Ld

  //printf("1 1 Segmentation fault\n");
  //return 1;

  // cpu idle
  unsigned long long idle;
  idle = sysmonitor_cpu_idle();
  printf("cpu %Ld ",idle);
  //printf("%Ld\n",idle);

  // cores info
  int cores = sysmonitor_cpu_cores();
  printf("%d ",cores);
  //printf("%d\n",cores);

  // load average
  float l1,l5,l15;
  sysmonitor_loadavg(&l1,&l5,&l15);
  //printf("load %1.2f %1.2f %1.2f ",l1,l5,l15);
  printf("load %d %d %d ",(int)(100*l1),(int)(100*l5),(int)(100*l15));
  //printf("%d\n%d\n%d\n",(int)(100*l1),(int)(100*l5),(int)(100*l15));

  // memory
  int m,c,s;
  sysmonitor_meminfo_percent(&m,&c,&s);
  printf("mem %d %d %d ",m,c,s);
  //printf("%d\n%d\n%d\n",m,c,s);

  // network
  unsigned long long rx,tx;
  sysmonitor_net(&rx,&tx);
  printf("net %Ld %Ld ",rx,tx);
  //printf("%Ld\n%Ld\n",rx,tx);

  // disk
  unsigned long long d = sysmonitor_disk_load();
  printf("disk %Ld ",d);
  //printf("%Ld\n",d);

  // done
  printf("\n");
  return 0;
}

