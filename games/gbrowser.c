/* Game browser
 * -------------
 * This gets called out upon zero exit code from any game
 * or upon the bootstrap run. Uses the database to get
 * a list of games and provides entry points to each
 */

int
gameshow(char* progress, char* name, char* host)
{
    tftfill(0x0000);
    *name = 0;
    *host = 0;
    
    /* host first */
    while(*progress != '\'')
        printchr(*progress, host, 2, 50);
   
    tftprint("Host: ");
    tftprint(host);
    tftprint("\n");         
   
    /* now for the game id */
    progress = progress + 4;
    while(*progress != '\'')
        printchr(*progress, name, 2, 50);

    tftprint("Game: ");
    tftprint(name);
    tftprint("\n");
   
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
    char *gameslist, *progress, *name, *host;
    int gameslistsize, sstat;
    
    gameslistsize = 500;
    gameslist = (*char)heapalloc(gameslistsize);
    name = (*char)heapalloc(50);
    host = (*char)heapalloc(50);

    httpget("what=whatever", "/sandbox/sc/team070/request_handler.py", \
        gameslist, 500);
    
    printtxt(gameslist, "SERIAL", 0, 0); 
    if(gameslist[1] == ']'){
        tftprint("PANIC: EMPTY GAMES LIST");
        while(1){};
    }
    progress = gameslist + 1;
    sstat = 0;
    *host = 0; *name = 0;

    while(1){
        /* increment */
        if(*progress == ',') progress = progress + 2;
        else if(*progress == ']') progress = gameslist;
        /* don't show the bootstrap */
        if(*progress == '\'' && progress[1] == '0' && progress[2] == '\'')
            progress = progress + 3;
        /* run the game show. ret == 1 -> download */
        sstat = gameshow(progress);
        if(sstat){
            /* co-opt the gameslist
             * this will get freed down the line
             */
            printtxt("game_name=", gameslist, 1, 500);
            printtxt(name, gameslist, 2, 500);
            printtxt("&host=", gameslist, 2, 500);
            printtxt(host, gameslist, 2, 500);
            heapfree(name);
            heapfree(host);
            exit(gameslist);
        }
    }
    return -1;
}
