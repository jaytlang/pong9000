/* FPSR Standard Library
 * JTL 2020
 *  
 * Each of these functions shows up in the main sketch as an instruction.
 * This is a rushed way of doing it but it is fine for now.
 * In that sense, it isn't really a libc - more of an extended ISA.
 *  
 * ...libc sounds cooler.
 * 
 * NOTE: THIS IS NOT COMPILED BY FPSR. Stick to style but use 
 *    any and all language features you would like.
 */

#ifndef libfpsr_h
#define libfpsr_h

/* Future includes go here */
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <math.h>
#include <mpu6050_esp32.h>

#define HOSTNAME "jaytlang"

extern TFT_eSPI maintft;
extern MPU6050 mainimu;

/* Printline.
 *  DESC: Interfaces with Serial and prints to monitor, or other strings. 
 *      Overloaded to support str and double types.
 *      Put "Serial" in for dest to output to console.
 *      These functions are safe from dumb 6.08'ers like myself.
 *      
 *      We don't use printf because these functions allow a greater degree
 *      of string manipulation by replacing strcat, strcpy, and printf with
 *      safer functions. Note that printflt utilizes the FPU's unpack opt.
 *      
 *      Specify a source and a destination. Source can be a dbl or a string.
 *      Flags is either 0, which prints to the serial console,
 *        ...1, which overwrites
 *        ...or 2, which concats
 *        
 *      Feel free to make an enum to help with this. FPSR is too simple to
 *      do a lot of this stuff, sorry it's very non-UNIXy.
 *        
 *  REQS: Serial.begin() is done in the bootstrap.
 */

int    printtxt(const char* src, char* dest, int sizedest, int flag);
int    printchr(const char value, char* dest, int sizedest, int flag);
int    printnum(const int value, char* dest, int sizedest, int flag);

/* Heap allocation.
 * DESC: Our VM doesn't have a heap, and also doesn't support static
 *    initialization. We rely on the host heap to do things - this just
 *    makes sense and protects against stack smashing by incompetent folks
 *    like me. Also, there's no bss segment, because we don't do any
 *    pythonic dynamic allocation when we can just go off the bat.
 *    
 *   `The host heap is used to deal with program things, because they
 *    can get beeeg and need to last beyond the lifetime of the execution
 *    of a given function. So. Pls use the data segment when you can.
 * 
 *    However, we include a data segment. This is used for STATIC
 *    initialization of arrays and (!) STRING LITERALS. The stack
 *    is for literally everything else. 
 *    
 * REQS: None. Don't f it up, and please free.
 */
void*  heapalloc(const int sizealloc);
void   heapfree(void* ptr);

/* TFT operations.
 * DESC: These largely shadow existing methods in the TFT library, sans
 *    the class prefix. Implementation consists of one liners for the most
 *    part, but there are some semantic differences where necessary.
 *    
 *    TFT is pre-instantiated with a rotation value of '1'. 
 *    RE. coloration, I expect two-byte hexadecimal color codes instead 
 *    of macro defs since FPSR has no preprocessor.
 *    
 *    Init will be called for you and rotation is preset to one. For
 *    more information about what is "done for you" see bootstrap.h.
 *    
 * REQS: Init is called for you. Change the rotation and call fill.
 */
void  tftprint(char *toprint);
void  tftrotation(int amt);
void  tfttextcolor(int foreground, int background);
void  tfttextsize(int sz);

void  tftdrawcircle(int xcoord, int ycoord, int radius, int color);
void  tftdrawellipse(int xcoord, int ycoord, int w, int h, int color);
void  tftdrawrect(int xcoord, int ycoord, int w, int h, int color);
void  tftdrawline(int xs, int ys, int xe, int ye, int color);
void  tftdrawpixel(int xcoord, int ycoord, int color);

void  tftfill(int color);
void  tftfillcircle(int xcoord, int ycoord, int radius, int color);
void  tftfillrect(int xcoord, int ycoord, int w, int h, int color);
void  tftfillellipse(int xcoord, int ycoord, int w, int h, int color);


void  tftsetcursor(int xcoord, int ycoord);

/* MPU6050 operations.
 * DESC: These shadow existing MPU6050 library calls.
 *    The imureadaccel function takes in a pointer to destination 
 *    buffer of length 3, corresponding to x, y, and z acceleration values. 
 *    These are scaled by a factor of 1000 from our float values in-class,
 *    and multiplied by the IMU acceleration resolution (which is refreshed
 *    automatically). Thus, the most accurate integer approximation
 *    is obtained.
 *    
 * REQS: Take this scaling into account when coding your game!
 */
void  imureadaccel(int *intbuf);

/* Button reading
 * DESC: Exactly what you think it is. Returns active low.
 *    Pins 16 and 5 are configured by the bootstrap for reading
 *    whenever you want them (INPUT_PULLUP). If this needs to
 *    change, let me know so I can yeet it over. In the
 *    meantime, the library checks for correct pin usage and
 *    will fail out with a -1 if you give it something bad.
 *    
 * REQS: Buttons are inited in bootstrap. 
 */
int   buttonread(int pin);

/* Internet connection
 * DESC: Supports get() and post() requests.
 *    Both of these requests need an input, a destination server, and
 *    an allocated output buffer + its size to make sure we don't go
 *    over its required length.
 *    
 *    Note that heap space might be an issue. If your code breaks,
 *    tweak the call to fpsr to minimize the required space. When
 *    fpsr gives you a cryptic error or crashes, not enough space
 *    allocated is to blame (assuming you've tested your code)
 *
 *    These semantics can be mildly confusing. An example is:
 *    httpget("myvar=1&ur=mom", "/sandbox/sc/myendpoint.py", outbuf, sizeofoutbuf);
 *    
 * REQS: Specify a destination, and talk to Jessie / make sure it's
 *    hooked into the server to give you reasonable feedback.
 */
void  httpget(char* input, char* serverpath, char* output, int sizeofoutput);
void  httppost(char* input, char* serverpath, char* output, int sizeofoutput);

/* Math
 * DESC: Basic math features you would expect from math.h, but integer
 *    only. Cosine and sine accept angles in degrees and give you
 *    results scaled by a factor of 1000. Abs does what you think
 *    it does, and sqrt will round off per C typecast rules.
 *        
 * REQS: Take scaling into account in game code!   
 */
int   absvalue(int input);
int   cosine(int angle);
int   sine(int angle);
int   root(int input);


/* Other common operations
 * DESC: These are simple -- too simple to warrant their own section.
 *    getmillis() returns exactly what you think it does...more functions
 *    to come down the road!
 *    
 * REQS: None.
 */
int   getmillis();


/* Hostname operations.
 *  DESC: Enables an ESP32 in the C runtime to figure out who
 *     it is. Highly useful regardless of context.
 *     
 *     Additionaly, one can use updatehostname() to alias your hostname
 *     to someone else's. This enables you to download a game running
 *     on their server and masquerade in order to achieve proper database
 *     semantics and enable multiplayer.. Use of this call in the true game 
 *     environment is highly discouraged and will likely be disabled in 
 *     a further push.
 *     
 *  REQS: A heap-allocated buf to hold the result in.
 */
void  gethostname(char* buf, int sizeofbuf);
void  updatehostname(char* buf, int sizeofbuf);

#endif /* libfpsr.h */
