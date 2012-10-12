// Setting volume up/down via "pacmd command" (pulseaudio)

int pa_hex_to_dec(char c);
long int pa_get_volume(void);
void pa_set_volume(long int v);

int pa_hex_to_dec(char c) {
  // converts char '0'..'f' to integer 0..15
  if ( (c>='0') && (c<='9') ) 
    return c-'0';
  if ( (c>='a') && (c<='f') ) 
    return c-'a'+10;
  if ( (c>='A') && (c<='F') ) 
    return c-'A'+10;
  return 0;
}

long int pa_get_volume(void) {
  // pacmd dump | grep set-sink-volume | tail -1 | awk '{ print $3 }' | hex2dec

  // local variables
  int i,j,a;
  unsigned int e,x=0;
  size_t l, n;
  char * buffer = (char *)malloc(sizeof(char)*256);
  FILE *f = popen("pacmd dump","r");

  // parse entire output by lines
  while (1) {
    l = getline(&buffer,&n,f);

    // find lines containing "set-sink-volume"
    i = strstr(buffer,"set-sink-volume")-buffer;
    //printf("%d\n",i);
    if (i==0) {
      //printf("  %s\n",buffer);
      a = strrchr(buffer,' ')-buffer;
      x = 0;
      e = 16*16*16;
      for (j=a+3; j<strlen(buffer)-1; j++) {
        x = x + e*pa_hex_to_dec(buffer[j]);
        e = e / 16;
        //printf("  %d = %c, dec=%d x=%d e=%d\n",j,buffer[j],pa_hex_to_dec(buffer[j]),x,e);
      }
    }

    // end of lines
    if ((int)l<=0)
      break;
  }

  // close file
  pclose(f);
  return x;
}

void pa_set_volume(long int v) {
  // pacmd set-sink-volume 1 $v

  // constraints
  if (v < 0) v = 0;
  if (v > 65535) v = 65535;

  // create command string
  char * cmd = (char*)malloc(sizeof(char)*256);
  sprintf(cmd,"pacmd set-sink-volume 0 %li 1>/dev/null",v);

  // execute command
  //printf("cmd=%s\n",cmd);
  system(cmd);

  // free
  free(cmd);
}
