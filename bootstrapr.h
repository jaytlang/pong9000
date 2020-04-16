
/* A string that is the compiler. 
 * Attempting to bootstrap. 
 */
 
 #ifndef bootstraptest_h
 #define bootstraptest_h
 
 #include "libfpsr.h"

 char* selsort =
 "void selsort(int *a, int size){"
 "int imin, i, j, tmp;"
 "i = 0;"
 "while(i < size-1){"
 "imin = i;"
 "j = i+1;"
 "while(j < size){"
 "if(a[j] < a[imin]) imin = j;"
 "j++;"
 "}"
 "tmp = a[imin];"
 "a[imin] = a[i];"
 "a[i++] = tmp;"
 "}}"
 "int main(){"
 "int *a, i;"
 "i = 0;"
 "a = heapalloc(6*sizeof(int));"
 "a[0] = 45; a[1] =  4; a[2] = 2; a[3] = 1; a[4] = 0; a[5] = -33"
 "selsort(a, 6);"
 "while(i < 6)"
 "printnum(a[i++], \"SERIAL\", 0, 0);"
 "return 0;}";
 
 
 char* commentdebug = "/* This is a comment */"
               "int main() { \n"
               "  int a;"
               "  a = 3;"
               "  a++;  "
               "  printnum(a, \"SERIAL\", 0, 0);"
               "  return 0;"
               "}";

 char* looptest = "int main() {\n"
                  "int a, rsum;\n"
                  "rsum = 0;\n"
                  "while(a < 60 && rsum < 1000){\n"
                  "   ++a;"
                  "   rsum = rsum + a;"
                  "   if(a == 30) printtxt(\"HALFWAY THEERREE\", \"SERIAL\", 0, 0);"
                  "}"
                  "printtxt(\"60th triangular no. is\", \"SERIAL\", 0, 0);"
                  "printnum(rsum, \"SERIAL\", 0, 0);"
                  "return 0;"
                  "}";

 char* dereftest = "int main() { \n"
                   "char* src;\n"
                   "src = (char*) heapalloc(40*sizeof(char));\n"
                   "printtxt(\"testmepls\", src, 40, 1);\n"
                   "while(*src != 0 && *src != 'p'){\n"
                   "printchr(*src++, \"SERIAL\", 0, 0);\n"
                   "}\n"
                   "return 0;\n"
                   "}\n";
                  
 char* henlosrc = "int main() { \n"
                     "  int a, b;\n\n"
                     "  a = 3;\n b = 4;\n a = a + b;\n\n"
                     "  printtxt(\"Hello world\", \"SERIAL\", 0, 0);\n\n"
                     "  return 0;\n\n"
                     "}\n";

 #endif /* bootstrapr.h */
