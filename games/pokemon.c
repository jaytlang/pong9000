int timer; 
int LOOP_PERIOD;
int primary_timer;
int posting_timer;
int BUTTON_TIMEOUT;
int button_timer;

int RESPONSE_TIMEOUT; 
int IN_BUFFER_SIZE;
int OUT_BUFFER_SIZE;
char* request_buffer; 
char* response_buffer; 

char *saved_pointer;
int break_this_bitch;

int chars_att;
int chars_def;
int squirt_att;
int squirt_def;
int ven_att;
int ven_def;

int player;
int opponents_turn;
int max_hp;
int pokemon_health;
int op_hp;

int opp_attack_stat;
int attack_damage;
int op_damage;
int damage;
int done;

char* op_attack_type;
char* op_pokemon;
char* op_attack_name;

char* pokemon;
int attack_stat;
int defense_stat;
char* pokemon_type;
int game_done;

int values_16;
int old_value_16;
int values_5;
int old_value_5;
int state;

char** attack_list;
char** my_attack_list;
char** attack_type_list;
char** my_attack_type_list;
int* attack_damage_list;
int* my_attack_damage_list;

char** pokemon_list;

char* choosen_attack_type;
char* choosen_attack_name;
int choosen_attack_damage;
int pokemon_count;
int pokemon_attack_count;

int didsetplayer;
int didsetpokemon;
int didsetpokemonattack;
int x;

char* names;
char* host;

int what_1;
int what_2;
int what_3;
int what_4;

char *body;
int body_len;

char* char_data;
char* delim;

void drawNameBoxes(){
  


  tftfillrect(2,   2,   160/2-0-2, 96-128/2-0-4, 0x0000);
  tftfillrect(2+2, 2+2, 160/2-4-2, 96-128/2-4-4, 8936264);
  tftfillrect(2+4, 2+4, 160/2-8-2, 96-128/2-8-4, 0xFFFF);
  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(2+6, 2+6);
  tftprint(op_pokemon);
  tftdrawrect(2+6, 2+6 + 9, 160/2-12-2 + 2 -2, 4 + 2, 0x0000);
  tftfillrect(2+6 + 1, 2+6 + 10, 160/2-12-2 -2, 4, 8936264);
  tftfillrect(2+6 + 1, 2+6 + 10, (((100*op_hp)/max_hp)*(160/2-12-2 -2))/100, 4,  0x17C0);


  

  tftfillrect(80,   128/2,   160/2-0-2, 96-128/2-0-2, 0x0000);
  tftfillrect(80+2, 128/2+2, 160/2-4-2, 96-128/2-4-2, 8936264);
  tftfillrect(80+4, 128/2+4, 160/2-8-2, 96-128/2-8-2, 0xFFFF);
  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(80+6, 128/2+6);
  tftprint(pokemon);
  tftdrawrect(80+6, 128/2+6 + 9, 160/2-12-2 + 2 -2, 4 + 2, 0x0000);
  tftfillrect(80+6 + 1, 128/2+6 + 10, 160/2-12-2 -2, 4, 8936264);
  tftfillrect(80+6 + 1, 128/2+6 + 10, (((100*pokemon_health)/max_hp)*(160/2-12-2 -2))/100, 4,  0x17C0); 
}

void drawScene(){
  tftfill(10025880);
  
  tftfillellipse(117, 48, 40, 12, 3329330);
  tftdrawellipse(117, 48, 40, 12, 11403055);
  
  tftfillellipse(160-117, 96, 40, 12, 3329330);
  tftdrawellipse(160-117, 96, 40, 12, 11403055);


  drawNameBoxes();

  tftfillrect(0, 96, 160, 32, 0x0000); 
  tftfillrect(2, 98, 156, 28, 8936264);
  tftfillrect(4, 100, 152, 24, 0xFFFF);


  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(7, 103);
  tftprint(my_attack_list[0]); 

  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(7, 114);
  tftprint(my_attack_list[1]); 
  
  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(80, 103);
  tftprint(my_attack_list[2]); 

  tfttextcolor(0x0000, 0xFFFF);
  tftsetcursor(80, 114);
  tftprint(my_attack_list[3]); 
  
}



/* INCORRECT DRAWS */

void drawOltorbBack(int colour){
tftfillcircle(45, 75, 20, 0xFFFF);
tftdrawcircle(45, 75, 20, colour);
tftdrawline(25, 75, 65, 75, colour);
tftdrawline(25, 74, 65, 74, colour);
tftdrawline(25, 73, 65, 73, colour);
tftdrawline(25, 72, 65, 72, colour);
tftdrawline(26, 71, 64, 71, colour);
tftdrawline(26, 70, 64, 70, colour);
tftdrawline(26, 69, 64, 69, colour);
tftdrawline(27, 68, 63, 68, colour);
tftdrawline(27, 67, 63, 67, colour);
tftdrawline(27, 66, 63, 66, colour);
tftdrawline(28, 65, 62, 65, colour);
tftdrawline(29, 64, 61, 64, colour);
tftdrawline(29, 63, 61, 63, colour);
tftdrawline(30, 62, 60, 62, colour);
tftdrawline(31, 61, 59, 61, colour);
tftdrawline(32, 60, 58, 60, colour);
tftdrawline(33, 59, 57, 59, colour);
tftdrawline(35, 58, 55, 58, colour);
tftdrawline(36, 57, 54, 57, colour);
tftdrawline(38, 56, 52, 56, colour);
tftdrawline(41, 55, 49, 55, colour);
}

void drawOltorbFront(int colour){
tftfillcircle(118, 30, 20, 0xFFFF);
tftdrawcircle(118, 30, 20, colour);
tftdrawline(98, 30, 138, 30, colour);
tftdrawline(98, 29, 138, 29, colour);
tftdrawline(98, 28, 138, 28, colour);
tftdrawline(98, 27, 138, 27, colour);
tftdrawline(99, 26, 137, 26, colour);
tftdrawline(99, 25, 137, 25, colour);
tftdrawline(99, 24, 137, 24, colour);
tftdrawline(100, 23, 136, 23, colour);
tftdrawline(100, 22, 136, 22, colour);
tftdrawline(100, 21, 136, 21, colour);
tftdrawline(101, 20, 135, 20, colour);
tftdrawline(102, 19, 134, 19, colour);
tftdrawline(102, 18, 134, 18, colour);
tftdrawline(103, 17, 133, 17, colour);
tftdrawline(104, 16, 132, 16, colour);
tftdrawline(105, 15, 131, 15, colour);
tftdrawline(106, 14, 130, 14, colour);
tftdrawline(108, 13, 128, 13, colour);
tftdrawline(109, 12, 127, 12, colour);
tftdrawline(111, 11, 125, 11, colour);
tftdrawline(114, 10, 122, 10, colour);

}



int strcmp_replacement(char* one, char* two){
  if (one[0] == two[0]){
    return 0;
  }
  else{
    return 1;
  }
}

char *fuckingstirtalk(char *pointer, char *delimiters){
    int break_loop;
    int break_first_loop;
    int count;
    int i;
    int fuck;
    int no_delims_present;
    if(break_this_bitch){
        return -1;
    }
    pointer = saved_pointer;
    break_loop = 0;
    break_first_loop = 0;
    count = 0;
    i = 0;

    while(!break_first_loop){
        no_delims_present = 1;
        fuck = 0;
        i = 0;
        while(i < 4 && fuck == 0){
            if(*pointer == *(delimiters + i)){
                if(*pointer == 0){
                    break_this_bitch = 1;
                    return -1;
                }
                pointer = pointer + 1;
                no_delims_present = 0;
                fuck = 1;
            }
            i = i + 1;
        }

        if(no_delims_present){
            break_first_loop = 1;
        }
    }

    while(!break_loop){
        i = 0;
        while(i < 4){
            if(*(pointer + count) == *(delimiters + i)){
                if(*(pointer + count) == 0){
                    break_this_bitch = 1;
                }
                *(pointer + count) = 0;
                saved_pointer = pointer + count + 1;
                return pointer;
            }
            i = i + 1;
        }
        count = count + 1;
    }
}


void deleteBox(){
  tftdrawrect(78, 112, 65, 11, 0xFFFF);
  tftdrawrect(5, 101, 65, 11, 0xFFFF);
  tftdrawrect(5, 112, 65, 11, 0xFFFF);
  tftdrawrect(78, 101, 65, 11, 0xFFFF);
}

int data_extractor(char* data_array){
  int counter;
  delim[0] = ';';
  delim[1] = '=';
  delim[2] = 0;
  counter = 0;
  saved_pointer = data_array;
  break_this_bitch = 0;
  char_data = fuckingstirtalk(data_array, delim);
  if((*data_array - '0') == 0){
    return 0;
  }
  while (char_data > 0){
    if (counter == 18){
      done = ifroma(char_data);
    }
    else if (counter == 2){
      op_damage = ifroma(char_data);
    }
    else if (counter == 4){
      printtxt(char_data, op_attack_type, 15*sizeof(char), 1);
    }
    else if (counter == 6){
      opp_attack_stat = ifroma(char_data);
    }
    else if (counter == 8){
      op_hp = ifroma(char_data);
    }
    else if (counter == 10){
      printtxt(char_data, op_pokemon, 15*sizeof(char), 1);
      if (!strcmp_replacement(op_pokemon, "Foltorb")){
        drawOltorbFront(0xF800);
      }
      if (!strcmp_replacement(op_pokemon, "Woltorb")){
        drawOltorbFront(0x001F);
      }
      if (!strcmp_replacement(op_pokemon, "Goltorb")){
        drawOltorbFront(0x07E0);
      }
    }
    else if (counter == 12){
      printtxt(char_data, op_attack_name, 15*sizeof(char), 1);
    }
      counter = counter + 1;
      char_data = fuckingstirtalk(data_array, delim);
  }
  deleteBox();
  drawNameBoxes();
  return 1;
}


int get_request_data(){
   printtxt("name=", request_buffer, 200, 1);
   printtxt(names, request_buffer, 200, 2);
   printtxt("&host=", request_buffer, 200, 2);
   printtxt(host, request_buffer, 200, 2);
   printtxt("&player=", request_buffer, 200, 2);
   printnum(player, request_buffer, 200, 2);
   httpget(request_buffer, "/sandbox/sc/team070/request_handler/request_handler.py", response_buffer, 500*sizeof(char));
   return data_extractor(response_buffer);
}

void post_request_data(){
    printtxt("variable_1=", body, body_len, 1);
    printnum(choosen_attack_damage, body, body_len, 2);
    printtxt("&variable_2=", body, body_len, 2);
    printtxt(choosen_attack_type, body, body_len, 2);
    printtxt("&variable_3=", body, body_len, 2);
    printnum(attack_stat, body, body_len, 2);
    printtxt("&variable_4=", body, body_len, 2);
    printnum(pokemon_health, body, body_len, 2);
    printtxt("&variable_5=", body, body_len, 2);
    printtxt(pokemon, body, body_len, 2);
    printtxt("&variable_6=", body, body_len, 2);
    printtxt(choosen_attack_name, body, body_len, 2);
    printtxt("&variable_7=", body, body_len, 2);
    printnum(0, body, body_len, 2);
    printtxt("&name=", body, body_len, 2);
    printtxt(names, body, body_len, 2);
    printtxt("&host=", body, body_len, 2);
    printtxt(host, body, body_len, 2);
    printtxt("&player=", body, body_len, 2);
    printnum(player, body, body_len, 2);
    printtxt("&score=", body, body_len, 2);
    printnum(0, body, body_len, 2);
    printtxt("&done=", body, body_len, 2);
    printnum(done, body, body_len, 2);
    httppost(body, "/sandbox/sc/team070/request_handler/request_handler.py", response_buffer, 500*sizeof(char));
}

int main() {
  printtxt("main", "SERIAL", 0 ,0);
  host = (char*)heapalloc(50*sizeof(char));
  request_buffer = (char*)heapalloc(500*sizeof(char));
  response_buffer = (char*)heapalloc(500*sizeof(char));
  op_attack_type = (char*)heapalloc(10*sizeof(char));
  op_pokemon = (char*)heapalloc(15*sizeof(char));
  op_attack_name = (char*)heapalloc(15*sizeof(char));
  pokemon = (char*)heapalloc(15*sizeof(char));
  pokemon_type = (char*)heapalloc(15*sizeof(char));
  attack_list = (char**)heapalloc(10*sizeof(char*));
  attack_list[0] = (char*)heapalloc(15*sizeof(char));
  attack_list[1] = (char*)heapalloc(15*sizeof(char));
  attack_list[2] = (char*)heapalloc(15*sizeof(char));
  attack_list[3] = (char*)heapalloc(15*sizeof(char));
  attack_list[4] = (char*)heapalloc(15*sizeof(char));
  attack_list[5] = (char*)heapalloc(15*sizeof(char));
  attack_list[6] = (char*)heapalloc(15*sizeof(char));
  attack_list[7] = (char*)heapalloc(15*sizeof(char));
  attack_list[8] = (char*)heapalloc(15*sizeof(char));
  attack_list[9] = (char*)heapalloc(15*sizeof(char));
  my_attack_list = (char**)heapalloc(4*sizeof(char*));
  my_attack_list[0] = (char*)heapalloc(15*sizeof(char));
  my_attack_list[1] = (char*)heapalloc(15*sizeof(char));
  my_attack_list[2] = (char*)heapalloc(15*sizeof(char));
  my_attack_list[3] = (char*)heapalloc(15*sizeof(char));
  attack_type_list = (char**)heapalloc(10*sizeof(char*));
  attack_type_list[0] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[1] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[2] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[3] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[4] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[5] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[6] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[7] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[8] = (char*)heapalloc(15*sizeof(char));
  attack_type_list[9] = (char*)heapalloc(15*sizeof(char));
  my_attack_type_list = (char**)heapalloc(4*sizeof(char*));
  my_attack_type_list[0] = (char*)heapalloc(15*sizeof(char));
  my_attack_type_list[1] = (char*)heapalloc(15*sizeof(char));
  my_attack_type_list[2] = (char*)heapalloc(15*sizeof(char));
  my_attack_type_list[3] = (char*)heapalloc(15*sizeof(char));
  attack_damage_list = (int*)heapalloc(10*sizeof(int));
  my_attack_damage_list = (int*)heapalloc(4*sizeof(int));
  pokemon_list = (char**)heapalloc(3*sizeof(char*));
  pokemon_list[0] = (char*)heapalloc(15*sizeof(char));
  pokemon_list[1] = (char*)heapalloc(15*sizeof(char));
  pokemon_list[2] = (char*)heapalloc(15*sizeof(char));
  body = (char*)heapalloc(500*sizeof(char));
  char_data = (char*)heapalloc(20*sizeof(char));
  delim = (char*)heapalloc(3*sizeof(char));
  printtxt("done allocating", "SERIAL", 0, 0);
  body_len = 500;
  gethostname(host, 50*sizeof(char));
  names = "Pokemon";
  LOOP_PERIOD = 50; 
  primary_timer = 0;
  posting_timer = 0;
  BUTTON_TIMEOUT = 1000;
  RESPONSE_TIMEOUT = 6000;
  IN_BUFFER_SIZE = 200; 
  OUT_BUFFER_SIZE = 200; 
  chars_att = 4;
  chars_def = 5;
  squirt_att = 4;
  squirt_def = 5;
  ven_att = 4;
  ven_def = 5;
  max_hp = 50;
  pokemon_health = max_hp;
  op_hp = max_hp;
  attack_damage = 10;
  op_damage = 0;
  damage = 0;
  done = 2;
  game_done = 0;
  attack_list[0] = "vinewhip";
  attack_list[1] = "razorleaf";
  attack_list[2] = "flareblitz";
  attack_list[3] = "fireblast";
  attack_list[4] = "hydropump";
  attack_list[5] = "bubblebeam";
  attack_list[6] = "leer";
  attack_list[7] = "growl";
  attack_list[8] = "harden";
  attack_list[9] = "howl";
  attack_type_list[0] = "grass";
  attack_type_list[1] = "grass";
  attack_type_list[2] = "fire";
  attack_type_list[3] = "fire";
  attack_type_list[4] = "water";
  attack_type_list[5] = "water";
  attack_type_list[6] = "other";
  attack_type_list[7] = "other";
  attack_type_list[8] = "other";
  attack_type_list[9] = "other";
  attack_damage_list[0] = 10;
  attack_damage_list[1] = 10;
  attack_damage_list[2] = 10;
  attack_damage_list[3] = 10;
  attack_damage_list[4] = 10;
  attack_damage_list[5] = 10;
  attack_damage_list[6] = 0;
  attack_damage_list[7] = 0;
  attack_damage_list[8] = 0;
  attack_damage_list[9] = 0;
  pokemon_list[0] = "Foltorb";
  pokemon_list[1] = "Woltorb";
  pokemon_list[2] = "Goltorb";
  pokemon_count = 0;
  pokemon_attack_count = 0;
  didsetplayer = 0;
  didsetpokemon = 0;
  didsetpokemonattack = 0;
  tftfill(0x0000);
  didsetplayer = 0;
  printtxt("Foe Joe", op_pokemon, 15*sizeof(char), 1);
  tftprint("Please set your player number. A for 0. B for 1.");
  while(didsetplayer == 0){
    if(!buttonread(16)){
        while(!buttonread(16));
        didsetplayer = 1;
        player = 0;
        state = 1;
    }
    else if(!buttonread(5)){
        while(!buttonread(5));
        didsetplayer = 1;
        player = 1;
        state = 0;
    }
  }
  opponents_turn = player;
  old_value_16 = values_16;
  old_value_5 = values_5;
  button_timer = getmillis();
  tftfill(0x0000);
  while(didsetpokemon == 0){
    values_16 = buttonread(16);
    tftsetcursor(0,0);
    tftprint("which pokemon do you want?");
    if (!values_16 && old_value_16){
      tftfill(0x0000);
      if (pokemon_count == 3){
        pokemon_count = 1;
      }
      else{
        pokemon_count = (pokemon_count + 1);
      }
      button_timer = getmillis();
      tftsetcursor(0, 20);
      tftprint(pokemon_list[pokemon_count - 1]);
    }
    else if (getmillis() - button_timer  >= BUTTON_TIMEOUT && pokemon_count != 0){
      pokemon = pokemon_list[pokemon_count-1];
      didsetpokemon = 1;
    }
    old_value_16 = values_16;
  }
    what_1 = randomint(0,10);
    what_2 = randomint(0,10);
    what_3 = randomint(0,10);
    what_4= randomint(0,10);
    while (what_2 == what_1){
      what_2 = randomint(0,10);
    }
    while (what_3 == what_2 || what_3 == what_1){
      what_3 = randomint(0,10);
    }
    while (what_4 == what_3 || what_4 == what_2 || what_4 == what_1){
      what_4 = randomint(0,10);
    }
    
    my_attack_list[0] = attack_list[what_1];
    my_attack_list[1] = attack_list[what_2];
    my_attack_list[2] = attack_list[what_3];
    my_attack_list[3] = attack_list[what_4];
    my_attack_type_list[0] = attack_type_list[what_1];
    my_attack_type_list[1] = attack_type_list[what_2];
    my_attack_type_list[2] = attack_type_list[what_3];
    my_attack_type_list[3] = attack_type_list[what_4];
    my_attack_damage_list[0] = attack_damage_list[what_1];
    my_attack_damage_list[1] = attack_damage_list[what_2];
    my_attack_damage_list[2] = attack_damage_list[what_3];
    my_attack_damage_list[3] = attack_damage_list[what_4];
    drawScene();
    if (!strcmp_replacement(pokemon, "Foltorb")){
      printtxt("fire", pokemon_type, 15*sizeof(char), 1);
      attack_stat = chars_att;
      defense_stat = chars_def;
      drawOltorbBack(0xF800);
    }
    else if (!strcmp_replacement(pokemon, "Woltorb")){
      printtxt("water", pokemon_type, 15*sizeof(char), 1);
      attack_stat = squirt_att;
      defense_stat = squirt_def;
      drawOltorbBack(0x001F);
    }
    else if (!strcmp_replacement(pokemon, "Goltorb")){
      printtxt("grass", pokemon_type, 15*sizeof(char), 1);
      attack_stat = ven_att;
      defense_stat = ven_def;
      drawOltorbBack(0x07E0);
    }
    
    while (!game_done){
      if (opponents_turn){
        if (get_request_data()){
          opponents_turn = 0;
        }
        while(getmillis()-timer < 1000){
        }
        timer = getmillis();
      }
      else{
        if (done == player){
          game_done = 1;
          tftsetcursor(2,30);
          tftprint("You win!");
        }
        else if(state == 0){
          if (!strcmp_replacement(op_attack_name, "leer")){
            if (defense_stat == 1){
              defense_stat = 1;
            }
            else {
              defense_stat = defense_stat - 1;
            }
          }
          else if (!strcmp_replacement(op_attack_name, "growl")){
            if (attack_stat == 1){
              attack_stat = 1;
            }
            else{
              attack_stat = attack_stat - 1;
            }
          }
          else{
            damage = op_damage;
            damage = (((100*opp_attack_stat)/defense_stat) * damage)/100;
            if ((strcmp_replacement(pokemon_type,"fire") == 0 && strcmp_replacement(op_attack_type,"water") == 0) || (strcmp_replacement(pokemon_type,"water") == 0 && strcmp_replacement(op_attack_type,"grass") == 0) || (strcmp_replacement(pokemon_type,"grass") == 0 && strcmp_replacement(op_attack_type,"fire") == 0)){
              damage = 2 * damage;
            }
            else if ((strcmp_replacement(op_attack_type,"fire") == 0 && strcmp_replacement(pokemon_type,"water") == 0) || (strcmp_replacement(op_attack_type,"water") == 0 && strcmp_replacement(pokemon_type,"grass") == 0) || (strcmp_replacement(op_attack_type,"grass") == 0 && strcmp_replacement(pokemon_type,"fire") == 0)){
              damage = 50 * damage;
              damage = damage/100;
            }
            x = randomint(60,100);
            damage = (damage * x)/100;
            pokemon_health = pokemon_health - damage;
            if (pokemon_health <= 0){
              pokemon_health = 0;
              done = !player;
              tftsetcursor(2,30);
              tftprint("You lose.");
              game_done = 1;
              post_request_data();
            }
            drawNameBoxes();
          }
          state = 1;
        }else if (state == 1){
          pokemon_attack_count = 0;
          didsetpokemonattack = 0;
          while(didsetpokemonattack == 0){
            values_16 = buttonread(16);
            if (!values_16 && old_value_16){
              if (pokemon_attack_count == 4){
                pokemon_attack_count = 1;
                tftdrawrect(78, 112, 65, 11, 0xFFFF);
                tftdrawrect(5, 101, 65, 11, 0xF800);
              }
              else{
                pokemon_attack_count = (pokemon_attack_count + 1);
                if (pokemon_attack_count == 1){
                  tftdrawrect(5, 101, 65, 11, 0xF800);
                }
                if (pokemon_attack_count == 2){
                  tftdrawrect(5, 101, 65, 11, 0xFFFF);
                  tftdrawrect(5, 112, 65, 11, 0xF800); 
                }
                if (pokemon_attack_count == 3){
                  tftdrawrect(5, 112, 65, 11, 0xFFFF);
                  tftdrawrect(78, 101, 65, 11, 0xF800);
                }
                if (pokemon_attack_count == 4){
                  tftdrawrect(78, 101, 65, 11, 0xFFFF);
                    tftdrawrect(78, 112, 65, 11, 0xF800);
                }
              }
              button_timer = getmillis();
            }
            else if (getmillis() - button_timer  >= BUTTON_TIMEOUT && pokemon_attack_count != 0){
                if (pokemon_attack_count == 1){
                  tftdrawrect(5, 101, 65, 11, 0x011F);;
                }
                if (pokemon_attack_count == 2){
                  tftdrawrect(5, 112, 65, 11, 0x011F);
                }
                if (pokemon_attack_count == 3){
                  tftdrawrect(78, 101, 65, 11, 0x011F);
                }
                if (pokemon_attack_count == 4){
                    tftdrawrect(78, 112, 65, 11, 0x011F);
                }
              choosen_attack_name = my_attack_list[pokemon_attack_count - 1];
              choosen_attack_type = my_attack_type_list[pokemon_attack_count - 1];
              choosen_attack_damage = my_attack_damage_list[pokemon_attack_count - 1];
              didsetpokemonattack = 1;
            }
            old_value_16 = values_16;
            }   
            if (!strcmp_replacement(choosen_attack_name,"harden")){
              defense_stat = defense_stat + 1;
            }
            else if (!strcmp_replacement(choosen_attack_name,"howl")){
              attack_stat = attack_stat + 1;
            }  
            post_request_data();
            opponents_turn = 1;
            state = 0;
        }
      }
    }
    return 0;
  }
