// Functions for collecting various monitoring data from system (cpu, loadavg, memory usage, swap usage, network traffic, disk usage)

#ifndef SYSMONITOR_DATA
#define SYSMONITOR_DATA

void sysmonitor_meminfo(int * memtotal, int * memfree, int * memcached, int * swaptotal, int * swapfree);
void sysmonitor_meminfo_percent(int * memory, int * cache, int * swap);
int sysmonitor_loadavg(float * load1, float * load2, float * load3);
int sysmonitor_cpu_cores(void);
unsigned long long sysmonitor_cpu_idle(void);
void sysmonitor_net(unsigned long long *recv, unsigned long long *tran);
unsigned long long sysmonitor_disk_load(void);

#endif