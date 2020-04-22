/* Game browser
 * -------------
 * This gets called out upon zero exit code from any game
 * or upon the bootstrap run. Uses the database to get
 * a list of games and provides entry points to each
 */

int
gameshow(char* progress)
{
    char *name;

    tftfill(0x0000);
    name = (char*)heapalloc(50);
    *name = 0;

    /* host first */
    while(*progress != '\'')
        printchr(*progress, name, 2, 50);
   
    tftprint("Host: ");
    tftprint(name);
    tftprint("\n");         
    *name = 0; 
   
    /* now for the game id */
    progress = progress + 4;
    while(*progress != '\'')
        printchr(*progress, name, 2, 50);

    tftprint("Game: ");
    tftprint(name);
    tftprint("\n");
    heapfree(name);
   
    /* button blocking */
    while(1){
        if(!buttonread(16)) return 0;
        if(!buttonread(5)) return 1;
    } 
    return 0;
}

int
main()
{
    char *gameslist, *progress;
    int gameslistsize, sstat;
    
    gameslistsize = 500;
    gameslist = (*char)heapalloc(gameslistsize);
    httpget("what=whatever", "/sandbox/sc/team070/request_handler.py", \
        gameslist, 500);
    
    printtxt(gameslist, "SERIAL", 0, 0); 
    if(*++gameslist == ']'){
        tftprint("PANIC: EMPTY GAMES LIST");
        while(1){};
    }
    progress = gameslist;
    sstat = 0;

    while(1){
        /* increment */
        if(*progress == ',') progress = progress + 2;
        else if(*progress == ']') progress = gameslist;
        /* don't show the bootstrap */
        if(*progress == '\'' && progress[1] == '0' && progress[2] == '\'')
            progress = progress + 3;
        /* run the game show. ret == 1 -> download */
        sstat = gameshow(progress);
        /*
        if(sstat){
        }
        */
    }
    return -1;
}
