// Function for parsing /proc/mounts for devices mounted in /media directory
/*
Usage:

  int c = 0, i;
  char **pole = proc_mounts_media_list(10,&c);
  for (i=0; i<c; i++)
    printf("[%d] = %s\n",i,pole[i]);

TODO: maybe also display filesystem usage from df:

$ df -l -T -x tmpfs -x devtmpfs | grep '/media/'
/dev/sdb1     vfat     7811088    623968   7187120   8% /media/KINGSTON
/dev/sdc1     vfat     7769240   1767680   6001560  23% /media/ADATA
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <s.h>
#include <s.c>

char * substr(char *src, int from, int len);
char ** proc_mounts_media_list(int max, int *count, int labels_only);
int strpos(char *src, char c);
int proc_mounts_is_mounted(int max, char ** list, char * label);
void proc_mounts_free(int max, char ** list);
int proc_mounts_changed(char ** old);

int proc_mounts_changed(char ** old) {
  // return 1 if /proc/mounts changed since last time

  // read entire content to "n"  
  char *n = SCreateFromFile("/proc/mounts");
  
  // did it changed?
  int r = 0;
  if (!(*old)) {
    r = 1;
  } else {
    if (strcmp(n,*old)!=0)
      r = 1;
  }
  
  // remember old
  if (*old)
    SFree(*old);
  *old = n;
    
  //printf("r=%d n=%s\n",r,n);
  return r;
}

int strpos(char *src, char c) {
  // return position of char position in string
  // FIXME: I can't believe such function does not exist in C
  int i;
  for (i=0; i<strlen(src); i++) {
    //printf("src[%d] = %d, c=%d\n",i,src[i],c);
    if (src[i] == c)
      return i;
  }
  return -1;
}

char * substr(char *src, int from, int len) {
  // copy part of the string into new string, "from" is 0-indexed
  // FIXME: I can't believe there is no substr or similar function in string.h or glib, I searched it all and aparently there is no such function

  // local variables
  char *n = NULL;
  int i = 0;

  // check src
  if (src == NULL)
    return NULL;

  // check from 
  if (from < 0)
    from = 0;
  if (from > strlen(src) - 1)
    from = strlen(src) - 1;

  // check len
  if (len <= 0)
    return n;
  if (len > strlen(src) - from)
    len = strlen(src) - from;

  // alocate new string with propper length
  n = (char*)malloc(sizeof(char)*(len+1));

  // copy part of src to new string
  //printf("len=%d\n",len);
  for (i=from; i<from+len; i++) {
    //printf("i=%d\n",i);
    n[i-from] = src[i];
  }

  // terminate new string end return it
  n[len] = '\0';
  return n;
}


char ** proc_mounts_media_list(int max, int * count, int labels_only) {
  // list /proc/mounts for any FS mounted in /media directory
  
  // local variables
  int index = 0;
  int i;
  char ** pole = (char**)malloc(sizeof(char*)*max);
  for (i=0; i<max; i++)
    pole[i] = NULL;

  // alocate strings
  char *dev = (char*)malloc(sizeof(char)*256);
  char *dir = (char*)malloc(sizeof(char)*256);
  char *typ = (char*)malloc(sizeof(char)*256);
  char *opt = (char*)malloc(sizeof(char)*256);
  char *n1 = (char*)malloc(sizeof(char)*256);
  char *n2 = (char*)malloc(sizeof(char)*256);

  // open /proc/mounts  
  FILE *f = (FILE*)fopen("/proc/mounts","r");

  if (f) {

    while (!feof(f)) {

      // read line from file
      fscanf(f,"%s %s %s %s %s %s\n",dev,dir,typ,opt,n1,n2);

      // is it mounted in /media dir?
      if (strstr(dir,"/media/")) {

        // do not exceed maximal requested media count
        max--;
        if (max < 0)
          break;

        // create "DIRECTORY (FSTYPE, DEVICE)" string
        char * a = substr(dir,7,32);
        char * b = substr(dev,5,32);
        char * s = (char*)malloc(sizeof(char)*256);
        if (labels_only==0)
          sprintf(s,"%s (%s, %s)",a,typ,b);
        else
          sprintf(s,"%s",a);

        // if label contain space, it will be encoded as \040
        char * se = SCreateReplace(s,"\\040"," ");
        SFree(s);
        s = SCreate(se);
        SFree(se);
        //printf("s  =__%s__\n",s);
        //printf("se =__%s__\n",se);
        
        // add it to the array
        pole[index] = s;
        index++;

        // release temporary variables
        free(a);
        free(b);
      } else {
        // not in /media, ignore it
        //printf("IGN: dev=%s  dir=%s  typ=%s\n",dev,dir,typ);
      }
    }

    // close /proc/mounts
    fclose(f);
  }

  // release local vars
  free(dev);
  free(dir);
  free(typ);
  free(opt);
  free(n1);
  free(n2);

  *count = index;
  return pole;
}

int proc_mounts_is_mounted(int max, char ** list, char * label) {
  // return 1 if LABEL is mounted according to LIST, 0 otherwise
  int i;
  for (i=0; i<max; i++)
    if (list[i]) {
      //printf("--> comparing: '%s' and '%s'\n",list[i],label);
      if (strcmp(list[i],label)==0)
        return 1;
    }
  return 0;
}

void proc_mounts_free(int max, char ** list) {
  // release allocated list
  int i;
  //printf("Releasing %d items\n",max);
  for (i=0; i<max; i++)
    if (list[i]) {
      //printf("  [%d]\n",i);
      free(list[i]);
    }
  //printf("asdf\n");
  free(list);
}

