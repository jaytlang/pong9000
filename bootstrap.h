/*  Bootstrap():
 *  Called out of the main FPSR setup(). Configures peripherals given obj. references in
 *  libfpsr.h. Ideally, the rest of the setup program is never reached.
 *  Preparation with issuing an instruction to compile and run via FPSR.
 *
 *  Here, CODESIZE represents the amount of characters long your code is. It's used
 *  to allocate a buffer during the very beginning of execution to hold the big
 *  HTTP response with the code to be run through FPSR. FPSRALLOC and LITTLEFPSRALLOC
 *  are used in various ways for VM memory - check FPSR to see exactly this, but TL;DR...
 *
 *  - 30kb/15kb is a safe spread for FPSRALLOC / LITTLEFPSRALLOC
 *  - 50kb/25kb offers more code, at the cost of some stability (apparently)
 *  
 * 
 */

#ifndef bootstrap_h
#define bootstrap_h

#include "libfpsr.h"
#include "testbench.h"

#define WIFISSID "deltanet2.4"
#define PASSPHRASE "lanofthecats"
#define FPSRALLOC 30*1024
#define LITTLEFPSRALLOC 15*1024
#define CODESIZE 22000

#define REMOTE "/sandbox/sc/team070/request_handler/request_handler.py"

void bootstrap(char* dbuffer, int sizeofdbuffer);

#endif /* bootstrap.h */
