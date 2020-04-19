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
  httpget("game_name=test_1&host=1", "/sandbox/sc/team070/request_handler/request_handler.py",\
      dbuffer, sizeofdbuffer);
  Serial.println("This is a normal quote \"");
  return;
}
