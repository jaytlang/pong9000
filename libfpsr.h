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
 *      Feel free to make an enum to help with this. FPSR is simple.
 *        
 *  REQS: Serial.begin() somewhere in your code.
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
void   heapfree(int ptr);

#endif /* libfpsr.h */
