/* FPSR libc: implementation */

#include "libfpsr.h"

TFT_eSPI maintft = TFT_eSPI();

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
heapfree(void* ptr)
{
  free(ptr);
  return;
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

void
tftprint(char *toprint)
{
  maintft.print(toprint);
  return;
}

void
tftrotation(int amt)
{
  if(amt > 3){
    Serial.println("ERR: invalid rotation specified");
    return;
  }
  maintft.setRotation(amt);
  return;
}

void
tfttextcolor(int foreground, int background)
{
  maintft.setTextColor(foreground, background);
  return;
}

void
tfttextsize(int sz)
{
  maintft.setTextSize(sz);
  return;
}

void
tftfill(int color)
{
  maintft.fillScreen(color);
  maintft.setCursor(0, 0);
  return;
}

void
tftdrawcircle(int xcoord, int ycoord, int radius, int color)
{
  maintft.drawCircle(xcoord, ycoord, radius, color);
  return;
}

void 
tftdrawrect(int xcoord, int ycoord, int w, int h, int color)
{
  maintft.drawRect(xcoord, ycoord, w, h, color);
  return;
}

int
buttonread(int pin)
{
  if(pin != 16 && pin != 5){
    Serial.println("ERR: Bad button read pin");
    return -1;
  }
  return digitalRead(pin);
}

void
httpget(char* input, char* serverpath, char* output, int sizeofoutput)
{
  WiFiClient client;
  int len, i, j, clen;
  byte buf[sizeofoutput];

  client.setTimeout(10000);
  if(!client.connect("608dev-2.net", 80)){
    Serial.println("CONS FAILED");
    return;
  }
  client.print("GET ");
  client.print(serverpath);
  client.print("?");
  client.println(input);
  client.println("Host: 608dev-2.net");
  client.println();

  i = 0;
  clen = 0;
  while (!client.available()) {}
  while(client.connected()){
    len = client.available();
    j = 0;
    Serial.print("RUN LENGTH: ");
    Serial.println(len);
    clen += len;
    if(clen >= sizeofoutput){
      Serial.println("ERROR: BUFFER OVERFLOW POSSIBLE, MAKE YOUR OUTPUT ARRAY BIGGER");
      Serial.print(clen);
      Serial.print(" vs. ");
      Serial.println(sizeofoutput);
    }
    
    client.read(buf, len);
    while(j < len)
      output[i++] = buf[j++];
  }

  output[clen] = '\0';
  client.stop();
  Serial.println("Response is: ");
  Serial.println(output);
  return;
}

void
httppost(char* input, char* serverpath, char* output, int sizeofoutput)
{
  WiFiClient client;
  int len, i;
  byte buf[sizeofoutput];

  len = strlen(input);

  if(!client.connect("608dev-2.net", 80)){
    Serial.println("CONS FAILED");
    return;
  }
  client.print("POST ");
  client.print(serverpath);
  client.println(" HTTP/1.1");
  client.println("Host: 608dev-2.net");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(len);
  client.println();

  client.print(input);

  while (!client.available()) {}
  len = client.available();
  if(len >= sizeofoutput)
    Serial.println("ERROR: BUFFER OVERFLOW POSSIBLE, MAKE YOUR OUTPUT ARRAY BIGGER");
  
  client.read(buf, len);
  for (i = 0; i < len; i++)
    output[i] = buf[i];
    if(output[i] == '”' || output[i] == '“') output[i] = '"';

  output[len] = '\0';
  return;
}
