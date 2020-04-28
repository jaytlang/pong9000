/* Bootstrap implementation */

#include "bootstrap.h"

void
bootstrap(char* dbuffer, int sizeofdbuffer)
{
  int cstat, ctr, tohost;
  char gamename[100];
  char tmp[500], myhost[50], *ptr;

  Serial.begin(115200);
  maintft.init();
  tftrotation(1);
  pinMode(16, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  tftfill(0x000);
  tftprint("Hello\nConnecting to Wifi...\n");

  cstat = 0;
  WiFi.begin(WIFISSID, PASSPHRASE);
  for(ctr = 0; ctr <= 5; ctr++){
    if(WiFi.isConnected()){
      cstat = 1;
      break;
    }
    delay(500);
  }

  if(!cstat){
    Serial.println("Connection failed");
    ESP.restart();
  }

  gethostname(myhost, 50);
  Serial.println(myhost);

  tftprint("Getting game info...\n");
  httpget("what=whatever", REMOTE, tmp, 500);

  *gamename = '\0';
  if((ptr = strstr(tmp, myhost)) == NULL){
    Serial.println("Couldn't find any hosted games!");
    while(1);
  }
  ptr += strlen(myhost) + 4;
  while(*ptr != '\''){
    Serial.print(*ptr);
    printchr(*ptr, gamename, 100, 2);
    ptr++;
  }

  tftprint("\n\nWelcome to P O N G 9 0 0 0\n\nSmashing stacks since 2020\n");
  tftprint("--------------------------\n\n");
  tftprint("You're currently playing: \n");
  tftprint(gamename);
  tftprint("\n\n\nPress A to JIT this game\nPress B to browse around\n");

  while(1){
    if(!buttonread(16)){
      tohost = 1; break;
    }
    else if(!buttonread(5)){
      tohost = 0; break;
    }
  }
  tftfill(0x0000);
  
  if(tohost){
    tftprint("Downloading remote code..\n");
    Serial.println();
    Serial.print("Game: ");
    Serial.println(gamename);
    printtxt("game_name=", tmp, 500, 1);
    printtxt(gamename, tmp, 500, 2);
    printtxt("&host=", tmp, 500, 2);
    printtxt(myhost, tmp, 500, 2);
    httpget(tmp, REMOTE, dbuffer, sizeofdbuffer);
    return;
  }
  else{
    tftprint("Downloading the browser...\n");
    httpget("game_name=bootstrap&host=0", REMOTE, dbuffer, sizeofdbuffer);
    return; 
  }
    
}
