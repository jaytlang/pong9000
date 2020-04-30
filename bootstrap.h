/*  Bootstrap():
 *  Called out of the main FPSR setup(). Configures peripherals given obj. references in
 *  libfpsr.h. Ideally, the rest of the setup program is never reached.
 *  Preparation with issuing an instruction to compile and run via FPSR.
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
#define CODESIZE 20000

#define REMOTE "/sandbox/sc/team070/request_handler/request_handler.py"

void bootstrap(char* dbuffer, int sizeofdbuffer);

#endif /* bootstrap.h */
