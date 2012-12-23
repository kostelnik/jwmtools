// Simple library for detecting if some application is already running

#ifndef LIBONCE_C
#define LIBONCE_C

int once_process_count(char * name) {
  // how many times process is runing (0 not running, 1 once, 2 two or more times)
  // NOTE: if process is running 3 times, it will return 2 (meaning "more than once")
  
  char * command = (char*)malloc(sizeof(char)*1000);
  sprintf(command,"pgrep -u $USER ^%s$",name);
  
  FILE *f = (FILE*)popen(command,"r");
  if (!f) {
    fprintf(stderr,"error: cannot run command '%s'\n",command);
    return 0;
  }

  int a=0, b=0;
  fscanf(f,"%d %d",&a,&b);
  //printf("a=%d b=%d\n",a,b);

  fclose(f);
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


