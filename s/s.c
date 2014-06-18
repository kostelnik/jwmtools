// Functions for simpler string parsing, esp. extracting parts of strings

#ifndef S_C
#define S_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "s.h"

char *SCreate(const char * src) {
  // allocate copy of string src
  SCheck(strlen(src)>0,"cannot create empty string");
  char *s = malloc(sizeof(char)*(strlen(src)+1));
  s[0] = '\0';
  SDebug("'%s'",src);
  strcat(s,src);
  return s;
}

char *SCreateSize(int length) {
  // allocate string of given byte length (e.g. 4 for 'abcd', 5 for 'abčd')
  SCheck(length>0,"cannot create string with length < 0");
  char *s = malloc(sizeof(char)*(length+1));
  s[0] = '\0';
  return s;
}

char *SCreateInt(const int i, const int lead) {
  // convert integer to string, with optional zero leading to specified places, like sprintf("%d") but allocates string automatically
  SCheck(lead >= 0,"lead must be >= 0");
  char *s = SCreateSize(20);
  sprintf(s,"%d",i); // FIXME: actually use lead, currently fixed to 2
  return s;
}

char *SCreateAppend(const char *src, const char * suffix) {
  // return new string crated by concatenation of 2 strings
  int a = strlen(src);
  int b = suffix==NULL ? 0 : strlen(suffix);
  //printf("a=%d b=%d src=%s suffix=%s\n",a,b,src,suffix);
  char *s = SCreateSize(a+b);
  if (suffix != NULL)
    sprintf(s,"%s%s",src,suffix);
  else
    sprintf(s,"%s",src);
  return s;
}

char *SCreateFromFile(const char *filename) {
  // read entire content of file to string
  SCheck(filename,"undefined filename");
  FILE *f = (FILE*)fopen(filename,"r");
  if (!f) {
    printf("filename=%s\n",filename);
    SCheck(f,"cannot open file");
    return NULL;
  }
  int r;
  char *s = SCreateSize(200+1);
  char *n = NULL;
  char *o = NULL;
  while (!feof(f)) {
    // read line from file
    r = fread(s,sizeof(char),200,f);
    //printf("r=%d\n",r);
    s[r] = '\0';
    // append read text to n 
    if (!n)
      o = SCreate(s);                           
    else {
      o = SCreateAppend(n,s);         // NOTE: no offense to this lib author (me)
      SFree(n);                       //       but this does not seems simpler
    }
    n = o;
  } 
  fclose(f);
  SFree(s);
  return n;
}

char *SCreateFromCommand(const char *command) {
  // run command, return it's std out as string
  // NOTE: copy paste from SCreateFromFile
  SCheck(command,"undefined filename");
  FILE *f = (FILE*)popen(command,"r");
  if (!f) {
    SCheck(f,"cannot run command");
    return NULL;
  }
  int r;
  char *s = SCreateSize(200+1);
  char *n = NULL;
  char *o = NULL;
  while (!feof(f)) {
    // read line from file
    r = fread(s,sizeof(char),200,f);
    //printf("r=%d\n",r)
    if (r <= 0)
      break;
    s[r] = '\0';
    // append read text to n 
    if (!n)
      o = SCreate(s);                           
    else {
      o = SCreateAppend(n,s);         // NOTE: no offense to this lib author (me)
      SFree(n);                       //       but this does not seems simpler
    }
    n = o;
  } 
  fclose(f);
  SFree(s);
  return n;
}

void SDump(const char * s) {
  // print string to stderr
  if (s==NULL)
    fprintf(stderr,"SDump(-,%d): NULL\n",(size_t)s);
  else
    fprintf(stderr,"SDump(%d,%d): \"%s\"\n",strlen(s),(size_t)s,s);
}

int SPos(char *haystack, char *needle) {
  // find position of substring in string, for ("FooBar","Ba") it returns 3
  SCheck(haystack,"undefined HAYSTACK");
  SCheck(needle,"undefined NEEDLE");
  SDebug("'%s', '%s'",haystack,needle);
  char *p = strstr(haystack,needle);
  int i = p - haystack;
  return i;
}

int SPosFrom(char *haystack, char *needle, int from) {
  // find position of substring in string from given index: ("AbcdeAbcde","A",3) returns 5
  SCheck(haystack,"undefined HAYSTACK");
  SCheck(needle,"undefined NEEDLE");
  SCheck(from >= 0,"FROM must be positive index in HAYSTACK");
  SCheck(from < strlen(haystack),"FROM must be lower than HAYSTACK length");
  SCheck(from < strlen(haystack)+strlen(needle),"FROM must be lower than HAYSTACK+NEEDLE length");
  // FIXME: not finding something in string is not fatal error, perhaps it should only return with -1 result  
  if (from == 0)
    return SPos(haystack,needle);
  char *f = SCreateBetween(haystack,from,strlen(haystack));
  int r = SPos(f,needle);
  //printf("f=%s r=%d\n",f,r);
  SFree(f);
  if (r >= 0)
    return from + r;
  else
    return -1;
}

char *SCreateBetween(const char * src, int from, int to) {
  // return string between "from" and "to" indices
  SCheck(src,"undefined SRC");
  SCheck(from<=to,"FROM must be lower than TO");
  SCheck(from>=0,"FROM must be positive");
  if (to >= strlen(src))
    to = strlen(src)-1;
  // SCheck(to<=strlen(src),"FROM must be positive");
  SDebug("'%s', %d, %d",src,from,to);
  char *p = malloc(sizeof(char)*(to-from+2));
  int i;
  for (i=0; i<to-from+1; i++) {
    //printf("i=%d c=%c\n",i,src[from+i]);
    p[i] = src[from+i];
  }
  //SDebug("  p='%s'\n",p);
  //printf("  p[%d] = \\0\n",to-from+1);
  //SDebug("  p='%s'\n",p);
  p[to-from+1] = '\0';
  return p;
}

char *SCreateReplace(const char *src, char *substring, char *replacement) {
  // replace any occurance of substring in src and return string without these substrings
  SCheck(src,"undefined SRC");
  SCheck(substring,"undefined SUBSTRING");
  SDebug("'%s', '%s', '%s'",src,substring,replacement);
  int a,iter=0,len1,len3,lenr=0;
  if (replacement)
    lenr = strlen(replacement);
  char *p = SCreate(src);
  char *p1, *p3;
  while ((a=SPos(p,substring))>=0) {
    iter++;
    SCheck(iter<S_MAX_ITER,"maximal iterations count exceeded");
    SDebug("ITER=%d: a=%d p='%s'",iter,a,p);
    //printf("p=%s\n",p);
    // create new string
    p1 = NULL;
    len1 = 0;
    if (a > 0) {
      p1 = SCreateBetween(p,0,a-1);
      len1 = strlen(p1);
    }
    p3 = NULL;
    len3 = 0;
    if (a+strlen(substring) < strlen(p)) {
      p3 = SCreateBetween(p,a+strlen(substring),strlen(src));
      len3 = strlen(p3);
    }
    SDebug("p1='%s' replacement='%s' p3='%s'",p1,replacement,p3);
    // if replacement is longer than substring, allocate p again
    if (strlen(p) < len1+lenr+len3) {
      SFree(p);
      p = SCreateSize(len1+lenr+len3+1);
    }
    // actual replacement
    if (p1) {
      if (p3==NULL) {
        sprintf(p,"%s%s",p1,replacement);
      } else {
        //printf("--strlen(p)=%d\n",strlen(p));
        //printf("--p1=%d strlen=%d str='%s'\n",(size_t)p1,strlen(p1),p1);
        //printf("--re=%d strlen=%d str='%s'\n",(size_t)replacement,strlen(replacement),replacement);
        //printf("--p3=%d strlen=%d str='%s'\n",(size_t)p3,strlen(p3),p3);
        sprintf(p,"%s%s%s",p1,replacement,p3);     
      }
    } else {
      if (p3==NULL)
        sprintf(p,"%s",replacement);
      else
        sprintf(p,"%s%s",replacement,p3);     
    }
    SDebug("p=%s",p);
    // release temporary strings
    if (p1) SFree(p1);
    if (p3) SFree(p3);
  }
  return p;
}

int SEqual(char *a, char *b) {
  // return 1 if strings a and b are the same, 0 otherwise
  return (strcmp(a,b) == 0);
}

int SInt(const char * src) {
  // convert string to int
  SCheck(src,"undefined SRC");
  int r;
  sscanf(src,"%d",&r);
  return r;
}

float SFloat(const char * src) {
  // convert string to int
  SCheck(src,"undefined SRC");
  float r;
  sscanf(src,"%f",&r);
  return r;
}


int S_TEST_ID = 0;

void STest(char *value, char * expected, char * error) {
  // print report of failed test and exit
  S_TEST_ID++;
  if (!SEqual(value,expected)) {
    fprintf(stderr,"STest(%2d): failed - %s\n",S_TEST_ID,error);
    fprintf(stderr," value   : '%s'\n",value);
    fprintf(stderr," expected: '%s'\n",expected);
    exit(EXIT_FAILURE);
  } else 
    if (S_DEBUG_LEVEL >= 1)
      printf("STest(%2d): passed - %s\n",S_TEST_ID,error);
}

void STestI(int value, int expected, char * error) {
  // print report of failed test and exit
  S_TEST_ID++;
  if (value != expected) {
    fprintf(stderr,"STestI(%2d): failed - %s\n",S_TEST_ID,error);
    fprintf(stderr," value   : %d\n",value);
    fprintf(stderr," expected: %d\n",expected);
    exit(EXIT_FAILURE);
  } else 
    if (S_DEBUG_LEVEL >= 1)
      printf("STest(%2d): passed - %s\n",S_TEST_ID,error);
}

void STests(int level) {
  // unit self tests
  char *a, *b, *c;
  int i,j,k;
  int ol = S_DEBUG_LEVEL;
  S_DEBUG_LEVEL=level;
  
  // test
  a = SCreate("Hello World");
  STest(a,"Hello World","string creation");
  
  // test
  SFree(a);
  STestI((int)(long)a,0,"string free and set to NULL");
        
  // test
  a = SCreate("0123456789");
  i = SPos(a,"0");    STestI(i,0,"position of char on the begining of string");
  i = SPos(a,"012");  STestI(i,0,"position of string on the begining of string");
  i = SPos(a,"9");    STestI(i,9,"position of char on the end of string");
  i = SPos(a,"789");  STestI(i,7,"position of string on the end of string");
  i = SPos(a,"4");    STestI(i,4,"position of char in the middle of string");
  i = SPos(a,"456");  STestI(i,4,"position of string in the middle of string");
  SFree(a);
  
  // test
  //S_DEBUG_LEVEL=1;
  a = SCreate("I walked into bar and talked to bar");
  b = SCreateReplace(a,"bar","BAR");
  STest(b,"I walked into BAR and talked to BAR",
        "replacing on the end of string");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("foo bar foo");
  b = SCreateReplace(a,"bar","BAR");
  STest(b,"foo BAR foo",
        "replacing in the middle of string once");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("foo bar and bar foo");
  b = SCreateReplace(a,"bar","BAR");
  STest(b,"foo BAR and BAR foo",
        "replacing in the middle of string twice");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("foo bar foo");
  b = SCreateReplace(a,"foo","FOO");
  STest(b,"FOO bar FOO",
        "replacing on the begining and end of string");
  SFree(a);
  SFree(b);
  
  // test
  a = SCreate("abc");
  b = SCreateReplace(a,"b","B");
  STest(b,"aBc",
        "replacing one char on 2nd position");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("abc");
  b = SCreateReplace(a,"a","A");
  STest(b,"Abc",
        "replacing one char on 1st position");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("abc");
  b = SCreateReplace(a,"c","C");
  STest(b,"abC",
        "replacing one char on last position");
  SFree(a);
  SFree(b);
  
  // test
  a = SCreate("abc");
  b = SCreateReplace(a,"b","BBB");
  STest(b,"aBBBc",
        "replacing one char with 3char string");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("abbbc");
  b = SCreateReplace(a,"bbb","B");
  STest(b,"aBc",
        "replacing string with one char");
  SFree(a);
  SFree(b);

  // test
  a = SCreate("abbba");
  b = SCreateReplace(a,"a","AA");
  STest(b,"AAbbbAA",
        "multiple replacement with different length");
  SFree(a);
  SFree(b);
  
  // test
  a = SCreate("97%");
  b = SCreateReplace(a,"%","");
  STest(b,"97",
        "replacing percent sing from 97%%");
  SFree(a);
  SFree(b);
  
  // test
  a = SCreate("  Mono: Playback 62 [97%] [-2.00dB] [on]");
  i = SPos(a,"[");
  j = SPos(a,"]");
  b = SCreateBetween(a,i+1,j-1);
  c = SCreateReplace(b,"%","");
  k = SInt(c);
  STestI(k,97,"extracting alsa volume");
  SFree(a);
  SFree(b);
  SFree(c);
  
  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  b = SCreateBetween(a,10,50);
  STest(b,"Abcde","create string between longer that source");
  SFree(a);
  SFree(b);

  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  i = SPosFrom(a,"A",0);
  STestI(i,0,"position from 0 at 0");
  SFree(a);

  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  i = SPosFrom(a,"A",1);
  STestI(i,5,"position from 1 at 5");
  SFree(a);

  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  i = SPosFrom(a,"A",8);
  STestI(i,10,"position from 8 at 10");
  SFree(a);

  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  i = SPosFrom(a,"A",11);
  STestI(i,-1,"position from 11 at -1");
  SFree(a);

  // test      012345678901234
  a = SCreate("AbcdeAbcdeAbcde");
  i = SPosFrom(a,"e",11);
  STestI(i,14,"position from 14 at -1");
  SFree(a);

  if (S_DEBUG_LEVEL >= 1)
    printf("STest: all %d tests passed\n",S_TEST_ID);
  S_DEBUG_LEVEL=ol;        
}

#endif // S_C
