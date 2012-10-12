// Functions for parsing output of blkid
/*
FIXME: I know there is libblkid but I am offline right now

/dev/sda1: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="swap" 
/dev/sda2: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="reiserfs" 
/dev/sda3: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="reiserfs" 
/dev/sda4: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="crypto_LUKS" 
/dev/sdb1: LABEL="ADATA" UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="ext2" 
/dev/mapper/esda4: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="ext2" 
/dev/sdc1: LABEL="KINGSTON" UUID="xxxx-xxxx" TYPE="vfat" 

We only want this information:

/dev/sdb1 ADATA
/dev/sdc1 KINGSTON

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "proc_mounts.c"
#include "../s/s.h"
#include "../s/s.c"

#define BLKID_MAX_LEN 1000
#define BLKID_DEVICES 10

char * blkid_devices[10]; // /dev/sdb1, /dev/sdc1, ...
char * blkid_labels[10];  // KINGSTON, ADATA, ...
int blkid_mounted[10];    // 1=mounted, 0=not
int blkid_labels_count = 0; // number of labeled devices

void blkid_allocate (void);
int blkid_count (void);
int blkid_mounted_count (void);
void blkid_debug (void);
void blkid_free (void);
int blkid_parse (void);
void blkid_test (void);

void blkid_allocate (void) {
  // allocate array of devices and array of labels, call this function on the start of the program once!
  int i;
  for (i=0; i<BLKID_DEVICES; i++) {
    blkid_devices[i] = (char*)malloc(sizeof(char)*(BLKID_MAX_LEN+1));
    blkid_labels[i] = (char*)malloc(sizeof(char)*(BLKID_MAX_LEN+1));
    blkid_mounted[i] = 0;
  }
}

int blkid_count (void) {
  // return number of available devices
  int i,r=0;
  for (i=0; i<BLKID_DEVICES; i++)
    if (strlen(blkid_devices[i])>0)
      r++;
  return r;
}

int blkid_mounted_count (void) {
  // return count of mounted devices
  int i,r=0;
  for (i=0; i<BLKID_DEVICES; i++) {
    if (blkid_mounted[i]==1)
      r++; 
  }
  return r;
}

void blkid_debug (void) {
  // print list of devices and corresponding labels
  int i;
  printf("Debug list of %d devices and labels:\n",blkid_count());
  for (i=0; i<BLKID_DEVICES; i++) {
    if (strlen(blkid_devices[i])==0)
      continue;
    printf("  [%d]: device=%s label='%s' mounted=%d\n",i,blkid_devices[i],blkid_labels[i],blkid_mounted[i]);
  }
  printf("  blkid_labels_count=%d\n",blkid_labels_count);
  printf("\n");
}

void blkid_free (void) {
  // release those 10 labels and devices
  int i;
  for (i=0; i<BLKID_DEVICES; i++) {
    free(blkid_devices[i]);
    free(blkid_labels[i]);
  }
}

int blkid_parse (void) {

  // local variables
  char *line = SCreateSize(BLKID_MAX_LEN);
  FILE *f = (FILE*)popen("blkid","r");
  int len = 0, a, b, index=0;
  char *device;
  char *label;
  char ** proc;
  int c,i;

  // null it (not free)
  blkid_labels_count = 0;
  for (i=0; i<BLKID_DEVICES; i++) {
    blkid_devices[i][0]='\0';
    blkid_labels[i][0]='\0';
    blkid_mounted[i]=0;
  } 

  // parse /proc/mounts to find out which is mounted
  proc = proc_mounts_media_list(10,&c,1);
  //printf("c=%d\n",c);
  
  if (!f)
    return -1;
  // read all lines
  while (fgets(line,BLKID_MAX_LEN,f)) {
    len = strlen(line);

    //printf("LINE=%s\n",line);

    // end of file?
    if (feof(f))
      break;
    
    // skip lines without LABEL=
    a = SPos(line,"LABEL=\"");
    if (a < 0)
      continue;
      
    /* parse blkid output
    $ blkid
    /dev/sda1: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="swap" 
    /dev/sda2: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="reiserfs" 
    /dev/sda3: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="reiserfs" 
    /dev/sda4: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="crypto_LUKS" 
    /dev/sdb1: LABEL="ADATA" UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="ext2" 
    /dev/mapper/esda4: UUID="xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" TYPE="ext2" 
    /dev/sdc1: LABEL="KINGSTON" UUID="xxxx-xxxx" TYPE="vfat" 
    /dev/sdd1: SEC_TYPE="msdos" LABEL="SONY_MP3_PL" UUID="xxxx-xxxx" TYPE="vfat" 
    */

    // parse LABEL="something"    
    b = SPosFrom(line,"\"",a+strlen("LABEL=\""));
    //printf("line=%s a=%d b=%d\n",line,a,b);
    label = SCreateBetween(line,a+strlen("LABEL=\""),b-1);
    //printf("label=%s\n",label);
    
    // check label validity
    if (strlen(label) <= 0) {
      fprintf(stderr,"error: something went wrong while parsing blkid output\n");
      fprintf(stderr,"line: %s\n",line);
      exit(EXIT_FAILURE);
    }

    // parse "device: ..."
    b = SPos(line,":");
    device = SCreateBetween(line,0,b-1);
    
    // check device validity
    if (strlen(device) <= 0) {
      fprintf(stderr,"error: something went wrong while parsing blkid output\n");
      fprintf(stderr,"line: %s\n",line);
      exit(EXIT_FAILURE);
    }

    // fill values into array
    strcpy(blkid_devices[index],device);
    strcpy(blkid_labels[index],label);
    blkid_mounted[index] = proc_mounts_is_mounted(10,proc,label);
    blkid_labels_count = index+1;

    SFree(label);
    SFree(device);
    
    // move to next index  
    index++;
    if (index >= BLKID_DEVICES)
      break;
  } 
  
  // release local variables
  proc_mounts_free(10,proc);
  fclose(f);
  free(line);
//  free(device);
//  free(foo);
//  free(label);
//  free(label2);
   
  return 0;
}

void blkid_test (void) {
  // test it twice
  blkid_allocate();
  printf("First test:\n");
  blkid_parse();
  blkid_debug();
  printf("Second test:\n");
  blkid_parse();
  blkid_debug();
  blkid_free();
}
