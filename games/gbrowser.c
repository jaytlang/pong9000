/* Game browser
 * -------------
 * This gets called out upon zero exit code from any game
 * or upon the bootstrap run. Uses the database to get
 * a list of games and provides entry points to each
 */

char *gameslist, *progress, *name, *host;

void
decorative()
{
    tftfill(0x0000);
    tftprint("=== GAME BROWSER YEEET ===\n");
    tftprint("--------------------------");
    tftprint("\n\n\n");
    tftprint("- A to proceed, B to run -\n");
    tftprint("--------------------------\n\n\n");
    return;
}

int
gameshow()
{
    decorative();
    *name = 0;
    *host = 0;

    /* host first */
    progress++;

    while(*progress != '\''){
        printchr(*progress, host, 50, 2);
        progress++;
    }
   
    tftprint("Host: ");
    tftprint(host);
    tftprint("\n\n");         
   
    /* now for the game id */
    progress = progress + 4;
    while(*progress != '\''){
        printchr(*progress, name, 50, 2);
        progress++;
    }

    tftprint("Game: ");
    tftprint(name);
    tftprint("\n");
    progress++;
   
    /* button blocking */
    printtxt("Entering spin", "SERIAL", 0, 0);
    while(1){
        if(!buttonread(16)){
             while(!buttonread(16)){};    
             return 1;
        }
        if(!buttonread(5)){
            while(!buttonread(5)){};
            return 0;
        }
    } 
}

int
main()
{
    int sstat, gameslistsize;
    
    gameslistsize = 7500*sizeof(char);

    gameslist = (char*)heapalloc(gameslistsize);
    name = (char*)heapalloc(50);
    host = (char*)heapalloc(50);

    tftprint("Loading...");

    httpget("what=whatever", "/sandbox/sc/team070/request_handler/request_handler.py", \
        gameslist, gameslistsize);
    
    printtxt(gameslist, "SERIAL", 0, 0); 

    progress = gameslist + 1;
    sstat = 1;
    *host = 0; 
    *name = 0;

    while(1){
        /* increment */
        if(sstat == -1) sstat = 1;
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
        if(sstat == 1){
            printtxt("Showing game", "SERIAL", 0, 0);
            printtxt(progress, "SERIAL", 0, 0);
            sstat = gameshow();

        }
        if(sstat == 0){
            tftfill(0x0000);
            tftprint("== SETTING YOUR GAME UP ==");
            tftprint("== Rebooting when done! ==\n\n");
            tftprint("Realiasing hostname...\n");

            updatehostname(host, 50);
            exit(1);
        }
    }
    return -1;
}
