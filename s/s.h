/*
Functions for simpler string parsing, esp. extracting parts of strings

DOGMA:
  - this lib looks ugly, so that actual code can be nice, simple and safe
  - zero indexed (string itself, returned positions, indices, etc...)
  - first parameter is string that is being operated, the rest
    parameters can be some other string, or int, ...
  - haystack,needle
  - no char attributes or retval, everything is string, so for 
    SPos(s,"Hello") and SPos(s,'H') there is only 1 function, 
    use "c" instead of 'c' for single characters
  - never change string parsed as argument (except SFree which
    also set pointer to NULL after it was freed)
    
TODO: split into .h and .c with that ifdef macro si it can be nested
TODO: asserts?
TODO: SRPos
TODO: SNPos
TODO: SPosFrom
TODO: use in other programs where neccessary
TODO: SCreateAppend(src,suffix); // SCreateSize(strlen(src)+strlen(suffix)); sprintf(s,"%s%s",src,suffix);
TODO: SCreatePrepend(src,prefix); 
TODO: SCreateTrim(src)
TODO: SCreateTrimLeft(src)
TODO: SCreateTrimRight(src)
TODO: SCreateColumn(src,column_index) // from "foo bar zzz" keeps only e.g. 2nd column
TODO: SCreateColumnRemove(s,column_index) // ignores 2nd columns
TODO: SDebugArgs(argc,argv) debuging from cmdline, e.g. -debug 1 sets level to 1
TODO: SCreateFromProcess(char cmd*, int maxlength)
*/

#ifndef S_H
#define S_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int S_DEBUG_LEVEL = 0;  // 0=nothing, 1=important, 2=all
int S_MAX_ITER = 100;   // limit recursion hangup of SCreateReplace

// creating strings
char *SCreate(const char * src);
char *SCreateSize(int length);
char *SCreateBetween(const char * src, int from, int to);
char *SCreateReplace(const char *src, char *substring, char *replacement);
char *SCreateAppend(const char *src, const char * suffix);
char *SCreateFromFile(const char *filename);
char *SCreateFromCommand(const char *command);

// searching and comparing functions
int SPos(char *haystack, char *needle);
int SPosFrom(char *haystack, char *needle, int from);
int SEqual(char *a, char *b);

// string to numeric convertsions
int SInt(const char * src);
float SFloat(const char * src);

// debuging and testing
void SDump(const char * s);
void STest(char *value, char * expected, char * error);
void STestI(int value, int expected, char * error);
void STests(int level);

// SCheck is simply assert that cannot be turned of using NDEBUG macro
#define SCheck(expr,message) if (!(expr)) { printf("error: %s (in %s:%d, function %s)\n",message,__FILE__,__LINE__,__ASSERT_FUNCTION); exit(EXIT_FAILURE); };

// SDebug macro prints out function name and printf like arguments
#define SDebug(expr,args...) ({ if (S_DEBUG_LEVEL>1) { printf(":: %s(",__FUNCTION__); printf(expr,args); printf(")\n"); } })

// macro that release string and also sets it pointer to null
#define SFree(expr) ({ \
  if (!expr) { \
    printf("error: freeing null in (%s:%d, function %s)\n",__FILE__,__LINE__,__ASSERT_FUNCTION);\
    exit(EXIT_FAILURE); \
  } \
  free(expr); \
  expr = NULL; })

#endif // S_H