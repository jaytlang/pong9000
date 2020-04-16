/* FPSR libc: implementation */

#include "libfpsr.h"

void*
heapalloc(const int sizealloc)
{
  void *loc;

  if(!(loc = malloc(sizealloc))){
    Serial.println("ERR: Failed to allocate memory!");
    return NULL;
  }

  memset(loc, 0, sizealloc);
  return loc;
}

void
heapfree(int ptr)
{
  free((void*)ptr);
}

int
printtxt(const char* src, char* dest, int sizedest, int flag)
{
  switch(flag){
    case 0:
      Serial.println(src);
      break;
    case 1:
      if(sizedest < strlen(src)){
        Serial.println("ERR: dest too small, overflow possible");
        return -1;
      }
      *dest = '\0';
      sprintf(dest, "%s", src);
      break;
    case 2:
      if(sizedest < strlen(src) + strlen(dest)){
        Serial.println("ERR: dest too small, overflow possible");
        return -1;
      }
      strcat(dest, src);
      break;
  }
  return 0;
}

int
printchr(const char value, char* dest, int sizedest, int flag)
{
  char buf[2];

  buf[0] = value;
  buf[1] = '\0';
  return printtxt(buf, dest, sizedest, flag);
}

int
printnum(const int value, char* dest, int sizedest, int flag)
{
  char buf[20];
  int sprintfstat;

  sprintfstat = snprintf(buf, 20, "%d", value);
  if(sprintfstat < 0 || sprintfstat >= 20){
    Serial.println("ERR: buffer overflow in dbl-str casting");
    return -1;
  }
  
  switch(flag){
    case 0:
      Serial.println(buf);
      break;
    case 1:
      if(sizedest < strlen(buf)){
        Serial.println("ERR: dest too small, overflow possible");
        return -1;
      }
      *dest = '\0';
      sprintf(dest, "%s", buf);
      break;
    case 2:
      if(sizedest < strlen(buf) + strlen(dest)){
        Serial.println("ERR: dest too small, overflow possible");
        return -1;
      }
      strcat(dest, buf);
      break;
  }
  return 0;
}
