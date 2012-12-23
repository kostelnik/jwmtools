// demo for libonce.h, allows this demo run only once 

#include <stdio.h>
#include <unistd.h>

#include <libonce.h>
#include <libonce.c>

int main(int argc, char * argv[]) {

  if (once_process_count("demo",1) > 1) {
    printf("demo is already running\n");
    return 1;
  }   
  
  while (1) {
    sleep(1);
    printf("demo\n");
  }

  return 0;
}
