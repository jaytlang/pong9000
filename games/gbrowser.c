/* Game browser
 * -------------
 * This gets called out upon zero exit code from any game
 * or upon the bootstrap run. Uses the database to get
 * a list of games and provides entry points to each
 */

char *gameslist, *progress, *name, *host;

int
gameshow()
{
    int ctr;

    tftfill(0x0000);
    *name = 0;
    *host = 0;

    /* host first */
    progress++;
    ctr++;

    while(*progress != '\''){
        printchr(*progress, host, 50, 2);
        progress++;
        ctr++;
    }
   
    tftprint("Host: ");
    tftprint(host);
    tftprint("\n\n");         
   
    /* now for the game id */
    progress = progress + 4;
    while(*progress != '\''){
        printchr(*progress, name, 50, 2);
        progress++;
        ctr++;
    }

    tftprint("Game: ");
    tftprint(name);
    tftprint("\n");
    progress++;
   
    /* button blocking */
    printtxt("Entering spin", "SERIAL", 0, 0);
    while(1){
        if(!buttonread(16)){
             while(buttonread(16)){};    
             return ctr;
        }
        if(!buttonread(5)){
            while(buttonread(5)){};
            return 0;
        }
    } 
    return ctr;
}

int
main()
{
    int sstat, gameslistsize;
    
    gameslistsize = 500;
    gameslist = (char*)heapalloc(gameslistsize);
    name = (char*)heapalloc(50);
    host = (char*)heapalloc(50);

    httpget("what=whatever", "/sandbox/sc/team070/request_handler/request_handler.py", \
        gameslist, 500);
    
    printtxt(gameslist, "SERIAL", 0, 0); 

    progress = gameslist + 1;
    sstat = 0;
    *host = 0; 
    *name = 0;

    while(1){
        /* increment */
        if(*progress == ','){
            progress = progress + 2;
            printtxt("Comma things", "SERIAL", 0, 0);
        }
        else if(*progress == ']'){
            progress = gameslist;
            progress++;
            printtxt("Looping back", "SERIAL", 0, 0);
        }
        /* don't show the bootstrap */
        if(*progress == '\'' && progress[1] == '0' && progress[2] == '\''){
            printtxt("found the bootstrap lmao", "SERIAL", 0, 0);
            progress = progress + 16;
            sstat = -1;
        }
        /* run the game show. ret == 1 -> download */
        if(sstat >= 0){
            printtxt("Showing game", "SERIAL", 0, 0);
            printtxt(progress, "SERIAL", 0, 0);
            sstat = gameshow();
            if(!sstat){
                /* co-opt the gameslist
                 * this will get freed down the line
                 */
                printtxt("game_name=", gameslist, 500, 1);
                printtxt(name, gameslist, 500, 2);
                printtxt("&host=", gameslist, 500, 2);
                printtxt(host, gameslist, 500, 2);
                heapfree(name);
                heapfree(host);
                printtxt("Preparing to printout...", "SERIAL", 0, 0);
                printtxt(gameslist, "SERIAL", 0, 0);
                exit(gameslist);
            }
        } else sstat = 0;
    }
    return -1;
}
