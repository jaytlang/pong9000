/* Bootstrap implementation */

#include "bootstrap.h"

void
bootstrap(char* dbuffer, int sizeofdbuffer)
{
  int cstat, ctr;

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

  tftprint("Ready to go. DL'ing...\n");
  httpget("game_name=bootstrap&host=0", "/sandbox/sc/team070/request_handler/request_handler.py",\
      dbuffer, sizeofdbuffer);
  tftprint("Done\n");
  return;
}

void
restrap(char* srcurl, char* dbuffer, int sizeofdbuffer, int gohome)
{
  tftprint("Ready to go. DL'ing...\n");
  if(gohome) httpget("game_name=bootstrap&host=0", "/sandbox/sc/team070/request_handler/request_handler.py",\
      dbuffer, sizeofdbuffer);
  else httpget(srcurl, "/sandbox/sc/team070/request_handler/request_handler.py",\
      dbuffer, sizeofdbuffer);
  return;
}
