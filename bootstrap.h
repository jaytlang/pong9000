/*  Bootstrap():
 *  Called out of the main FPSR setup(). Configures peripherals given obj. references in
 *  libfpsr.h. Ideally, the rest of the setup program is never reached.
 *  Preparation with issuing an instruction to compile and run via FPSR.
 *  
 *  Reentry():
 *  This is re-entered every time an application switch occurs. Called with the
 *  appropriate destination, this set of functions should load in the relevant dest
 *  and slate for runtime...
 */

#ifndef bootstrap_h
#define bootstrap_h

#include "libfpsr.h"
#include "testbench.h"

#define WIFISSID "deltanet2.4"
#define PASSPHRASE "lanofthecats"
#define FPSRALLOC 20*1024
#define LITTLEFPSRALLOC 10*1024

void bootstrap(char* dbuffer, int sizeofdbuffer);
void restrap(char* nsrc, char* dbuffer, int sizeofdbuffer);

#endif /* bootstrap.h */
