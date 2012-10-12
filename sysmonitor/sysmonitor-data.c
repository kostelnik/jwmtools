// Functions for collecting various monitoring data from system (cpu, loadavg, memory usage, swap usage, network traffic, disk usage)

// header files 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sysmonitor-data.h>

void sysmonitor_meminfo(int * memtotal, int * memfree, int * memcached, int * swaptotal, int * swapfree) {
  // get memory info
  int value = 0;
  char * key = (char*)malloc(sizeof(char)*256);
  char * kb = (char*)malloc(sizeof(char)*256);
  FILE *f = fopen("/proc/meminfo","r");
  int i = 1;
  while (i > 0) {
    i = fscanf(f,"%s %d %s\n",key,&value,kb);
    if (strcmp(key,"MemTotal:")==0)  *memtotal = value;
    if (strcmp(key,"MemFree:")==0)   *memfree = value;
    if (strcmp(key,"Cached:")==0)    *memcached = value;
    if (strcmp(key,"SwapTotal:")==0) *swaptotal = value;
    if (strcmp(key,"SwapFree:")==0)  *swapfree = value;
    //printf("i=%d  key='%s'  value='%s'  kb='%s'\n",i,key,value,kb);
  };
  free(key);
  free(kb);
  //printf("memtotal=%d  memfree=%d  memcached=%d  swaptotal=%d  swapfree=%d\n",memtotal,memfree,memcached,swaptotal,swapfree);
}

void sysmonitor_meminfo_percent(int * memory, int * cache, int * swap) {
  // get human readable memory info in percents
  //
  // Example:
  //
  //   int m,c,s;
  //   sysmonitor_meminfo_percent(&m,&c,&s);
  //   printf("memory usage = %d %%\n",m);
  //   printf("cache usage  = %d %%\n",c);
  //   printf("swap usage   = %d %%\n",s);
  //
  int mt,mf,mc,st,sf;
  sysmonitor_meminfo(&mt,&mf,&mc,&st,&sf);
  *memory = (100*(mt-mf-mc))/mt;
  *cache = (100*mc)/mt;
  *swap = 0;
  if (st != 0)
    *swap = (100*(st-sf))/st;
}

int sysmonitor_loadavg(float * load1, float * load2, float * load3) {
  // get system average load from /proc/loadavg
  FILE *f = fopen("/proc/loadavg","r");
  int i = fscanf(f,"%f %f %f\n",load1,load2,load3);
  fclose(f);
  //printf("i=%d  a=%f  b=%f  c=%f\n",i,*load1,*load2,*load3);
  return i;
}

int sysmonitor_cpu_cores(void) {
  // find out number of cores for cpu usage counting
  FILE *f = fopen("/proc/stat","r");
  int i,cores=0;
  unsigned long long x,z;
  // ignore first line
  i = fscanf(f,"cpu %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&z,&x,&x,&x,&x,&x,&x,&x,&x,&x);
  //printf("cpu: i=%d\n",i);

  // read all cores (FIXME: insane code)
  int line = 0;
  char *s = (char*)malloc(sizeof(char)*256);
  while (1) {
    //sprintf(s,"cpu%d %%Ld %%Ld %%Ld %%Ld %%Ld %%Ld %%Ld %%Ld %%Ld %%Ld\\n",cores);
    //i = fscanf(f,s,&z,&x,&x,&x,&x,&x,&x,&x,&x,&x);
    i = fscanf(f,"%s",s);
    //printf("[%d] s=%s i=%d cores=%d\n",line,s,i,cores);
    if (strcmp(s,"cpu0")==0) cores=1;
    if (strcmp(s,"cpu1")==0) cores=2;
    if (strcmp(s,"cpu2")==0) cores=3;
    if (strcmp(s,"cpu3")==0) cores=4;
    if (strcmp(s,"cpu4")==0) cores=5;
    if (strcmp(s,"cpu5")==0) cores=6;
    if (strcmp(s,"cpu6")==0) cores=7;
    if (strcmp(s,"cpu7")==0) cores=8;
    if (strcmp(s,"cpu8")==0) cores=9;
    if (strcmp(s,"cpu9")==0) cores=10;
    if (strcmp(s,"cpu10")==0) cores=11;
    if (strcmp(s,"cpu11")==0) cores=12;
    if (strcmp(s,"cpu12")==0) cores=13;
    if (strcmp(s,"cpu13")==0) cores=14;
    if (strcmp(s,"cpu14")==0) cores=15;
    if (strcmp(s,"cpu15")==0) cores=16;
    //printf("cpu%d: s=%s i=%d z=%Ld\n",cores,s,i,z);
    if ( (cores >= 128)||(i!=1) )
      break;
    //cores++;
    line++;
  }
  free(s);  
  if (cores <= 0)  
    cores = 1;
  //printf("cores = %d\n",cores);

/*
  // detect few cores with this dumb code
  cores = 0;
  i = fscanf(f,"cpu0 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&z,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("\n\ncpu0 = %d\n",i);
  printf("z=%Ld\n");
  i = fscanf(f,"cpu1 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu1 = %d\n",i);
  i = fscanf(f,"cpu2 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu2 = %d\n",i);
  i = fscanf(f,"cpu3 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu3 = %d\n",i);
  i = fscanf(f,"cpu4 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu4 = %d\n",i);
  i = fscanf(f,"cpu5 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu5 = %d\n",i);
  i = fscanf(f,"cpu6 %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",&x,&x,&x,&x,&x,&x,&x,&x,&x,&x); if (i == 10) cores++; printf("cpu6 = %d\n\n\n",i);
  //printf("cores = %d\n",cores);
*/
  // close file
  fclose(f);
  return cores;
}

unsigned long long sysmonitor_cpu_idle(void) {
  // system idle jiffies (should be 100 per second on single core)
  FILE *f = fopen("/proc/stat","r");
  unsigned long long total_user, total_user_low, total_sys, total_idle;
  fscanf(f,"cpu %Ld %Ld %Ld %Ld",&total_user,&total_user_low,&total_sys,&total_idle);
  fclose(f);
  return total_idle;
}

void sysmonitor_net(unsigned long long *recv, unsigned long long *tran) {
  // monitor network load (receive and transmit), in bytes per seconds
  FILE *f = fopen("/proc/net/dev","r");
  unsigned long long bytes1, packets1, errs1, drop1, fifo1, frame1, compressed1, multicast1;
  unsigned long long bytes2, packets2, errs2, drop2, fifo2, frame2, compressed2, multicast2;

  //printf("A:\n");
  char *line = NULL;
  ssize_t read;
  size_t len = 0;
  while ((read = getline(&line, &len, f)) != -1) {
    // does not line contain eth0?
    if (strstr(line,"eth0:") == NULL)
      continue;
    //printf("--> '%s'\n", line);
    // parse it
    sscanf(line,"  eth0: %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld",
      &bytes1, &packets1, &errs1, &drop1, &fifo1, &frame1, &compressed1, &multicast1,
      &bytes2, &packets2, &errs2, &drop2, &fifo2, &frame2, &compressed2, &multicast2
      );
    //printf("==> recv=%Ld tran=%Ld\n",bytes1,bytes2);
    // return values
    *recv = bytes1;
    *tran = bytes2;
  }
  free(line);

  
/*  fgets(s,256,f);
  
  while (fscanf(f,"%s",s)==1) {
    printf("s='%s'\n",s);
    // eth0:
    if (strcmp(s,"eth0:")==0) {
      //printf("--> eth0\n");
      // receive
      fscanf(f,"%Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld",&bytes, &packets, &errs, &drop, &fifo, &frame, &compressed, &multicast);
      //printf("RECV ==> %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",bytes, packets, errs, drop, fifo, frame, compressed, multicast);
      *recv = bytes;
      // transmit
      fscanf(f,"%Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld",&bytes, &packets, &errs, &drop, &fifo, &frame, &compressed, &multicast);
      //printf("TRAN ==> %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n",bytes, packets, errs, drop, fifo, frame, compressed, multicast);
      *tran = bytes;
    }
  }
*/  
  
  //free(s);
  fclose(f);
}

unsigned long long sysmonitor_disk_load(void) {
  // disk usage statistics from /proc/diskstats (return ms spent reading or writing, 10th argument)
  FILE *fp = fopen("/proc/diskstats","r");
  //printf("/proc/diskstats = %d\n",fp);
  unsigned long long a, b, c, d, e, f, g, h, i, j, k, l=0, m, z=1;
  char * name = (char*)malloc(sizeof(char)*256);
  while (z == 1) {
    z = fscanf(fp,"%s",name);
    //printf("z=%Ld name=%s\n",z,name);
    if (strcmp(name,"sda")==0) {
      z = fscanf(fp,"%Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld\n", &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m);
      //printf("z=%Ld name=%s l=%Ld\n",z,name,l);
    }
  }
  free(name);
  fclose(fp);
  return l;
}
