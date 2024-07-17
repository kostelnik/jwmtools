// Simple library for detecting if some application is already running

#ifndef LIBONCE_C
#define LIBONCE_C

int once_process_count(char * name, int mine) {
  // how many times process is runing (0 not running, 1 once, 2 two or more times)
  // NOTE: if process is running 3 times, it will return 2 (meaning "more than once")
  // if mine == 1 only my own processes will be shown
  
  char * command = (char*)malloc(sizeof(char)*1000);
  if (mine == 1)
    sprintf(command,"pgrep -u $USER ^%s$",name);
  else
    sprintf(command,"pgrep ^%s$",name);
  
  FILE *f = (FILE*)popen(command,"r");
  if (!f) {
    fprintf(stderr,"error: cannot run command '%s'\n",command);
    return 0;
  }

  int a=0, b=0;
  if (fscanf(f,"%d %d",&a,&b) > 2)
    printf("note: strange fscanf result\n");
  //printf("a=%d b=%d\n",a,b);

  pclose(f);
  free(command);

  // how many times is process running?
  if ( (a==0)&&(b==0) )
    return 0;
  if ( (a>0)&&(b==0) )
    return 1;
  if ( (a>0)&&(b>0) )
    return 2;
  return -1;
}

#endif


