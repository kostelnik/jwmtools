// Testing program for blkid.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "blkid.c"

int main (int argc, char *argv[]) {
  
  int c = 0, i;
  char **pole = proc_mounts_media_list(10,&c,1);

  printf("Which label is mounted:\n");
  printf("  ADATA       = %d\n",proc_mounts_is_mounted(10,pole,"ADATA"));
  printf("  KINGSTON    = %d\n",proc_mounts_is_mounted(10,pole,"KINGSTON"));
  printf("  SONY_MP3_PL = %d\n",proc_mounts_is_mounted(10,pole,"SONY_MP3_PL"));
  printf("  TRANSCEND   = %d\n",proc_mounts_is_mounted(10,pole,"TRANSCEND"));
  
  for (i=0; i<c; i++)
    printf("mounted: [%d] = %s\n",i,pole[i]);

  blkid_test();
  
  proc_mounts_free(10,pole);

  return 0;
}
