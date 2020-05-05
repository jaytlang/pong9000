/*
   FPSR: Fairly Pointless Stack Representation
   A Hybrid C JIT execution environment for the ESP32
   JTL 2020

   Similar to the c4 family of tiny C compilers. This one
   is a bit longer, because it's verbose, and desires the following:
   - Extreme portability -> executability on the ESP32
   - Honed C library, designed for safety and ISA reduction 
   - Fast compilation and playable games; but readable code

   Implements a heavily modified CISC instruction set.

   Principles for modifying this code:
   - This code MUST retain the ability to compile itself.
   - What's a float? Use integers for this.
   - No preprocessor. Standard library works by default.
   - Link outside library definitions sparingly. The libc is demarcated below, in runtime().
      - The libc should stay small to eliminate complexity.
   - For now, no floats. No for loops thus far. Everything else goes..
   - Malloc big arrays on the heap instead of declaring them globally.
   - We might have to tweak the ESP32 stack size if we see overflows.

    STYLE: https://man.cat-v.org/plan_9/6/style.
    The code should be the documentation. FPSR doesn't understand //.

    TODO: Does this code need the .ino extension? Hehe...

*/

#include "bootstrap.h"

char *importsrc;
char exitstr[100];

/* FPSR STARTS HERE - copy pasting from here to setup will work to bootstrap */

int     currtoken;
char   *src;
int     linenumber;
int    *mainptr;

int     tokenvalue;
int    *currentidentifier;
int    *symboltable;

int    *textsegment;
int    *stacksegment;
char   *datasegment;
int     bpindex;

int basetype;
int type;

int *pc, *bp, *sp, ax;

/* types */
enum{ CHAR, INT, PTR };

enum{ LC, LI, LN, SC, SN, PSH, JMP, BEQZ, BNEZ, CALL, NFRM, \
      POP, RET, LEA, OR, XOR, AND, EQU, NEQ, LST, LEQ, GTT, \
      GEQ, SHL, SHR, ADD, SUB, MUL, DIV, MOD, EXIT, HALC, \
      HFRE, MCMP, PRTT, PRTC, PRTN, TPR, TROT, TTXC, TTXS, \
      TFS, TDC, TDR, BTRD, GET, POST, HSTN, SHST, MILS, ABS, \
      COS, SIN, TDL, TDP, TFC, TSCU, IRA, TFR, TDE, TFE, SQRT, \
      ATOI, RNG };

enum{ TOKEN, HASH, NAME, TYPE, CLASS, VALUE, STYPE, SCLASS, SVAL, IDSIZE };

enum{ Num = 128, Fun, Lib, Glo, Loc, Id, \
      Char, Else, Enum, If, Int, Return, Sizeof, While, \
      Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, \
              Add, Sub, Mul, Div, Mod, Inc, Dec, Brac };

void
lexer()
{
  char* startposition;
  int hash;

  while(currtoken = *src){
    ++src;

    if(currtoken == '\n'){
      ++linenumber;
    }
    
    else if(currtoken == '#'){
      while(*src != 0 && *src != '\n'){
        src++;
      }
    }

    else if((currtoken >= 'a' && currtoken <= 'z') || (currtoken >= 'A' && currtoken <= 'Z') \
             || (currtoken == '_')){
              
      startposition = src - 1;
      hash = currtoken;
      while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') \
              || (*src >= '0' && *src <= '9') || (*src == '_')){
        hash = hash * 147 + *src;
        src++;
      }
      
      currentidentifier = symboltable;
      while(currentidentifier[TOKEN]){
        if(currentidentifier[HASH] == hash && memcmp((char*)currentidentifier[NAME],\
            startposition, src - startposition) == 0){
          currtoken = currentidentifier[TOKEN];
          return;      
        }
        currentidentifier = currentidentifier + IDSIZE;
      }

      currentidentifier[NAME] = (int)startposition;
      currentidentifier[HASH] = hash;
      currtoken = currentidentifier[TOKEN] = Id;
      return;
    }

    else if(currtoken >= '0' && currtoken <= '9'){
      tokenvalue = currtoken - '0';
      
      if(tokenvalue > 0){
        while(*src >= '0' && *src <= '9')
          tokenvalue = tokenvalue*10 + *src++ - '0';
      }else{
        if(*src == 'x' || *src == 'X'){
          currtoken = *++src;
          
          while((currtoken >= '0' && currtoken <= '9') || (currtoken >= 'a' && currtoken <= 'f') || \
                 (currtoken >= 'A' && currtoken <= 'F')) {
            tokenvalue = tokenvalue * 16 + (currtoken & 15) + (currtoken >= 'A' ? 9 : 0);
            currtoken = *++src;
          }
          
        }else{
          while(*src >= '0' && *src <= 7) tokenvalue = tokenvalue*8 + *src++ - '0';
        }
      }
      currtoken = Num;
      return;
    }

    else if(currtoken == '"' || currtoken == '\''){
      startposition = datasegment;
      
      while(*src != 0 && *src != currtoken){
        tokenvalue = *src++;
        
        if(tokenvalue == '\\'){
          tokenvalue = *src++;
          if(tokenvalue == 'n') tokenvalue = '\n';
          else if(tokenvalue == 'r') tokenvalue = '\r';
        }

        if(currtoken == '"') *datasegment++ = tokenvalue;
      }
      printchr(tokenvalue, "SERIAL", 0, 0);

      src++;
      if(currtoken == '"') tokenvalue = (int)startposition;
      else{
        currtoken = Num;
        printtxt("parsed individual char", "SERIAL", 0, 0);
      }
      return;
    }

    else if(currtoken == '/'){
      if(*src == '*'){
        while(!(*src == '*' && src[1] == '/')){
          if(*src == '\n') linenumber++;
          ++src;
        }
        src = src + 2;
      }else{
        currtoken = Div;
        return;
      }
    }

    else if(currtoken == '='){
      if(*src == '='){
        src++;
        currtoken = Eq;
      }
      else currtoken = Assign;
      return;
    }

    else if(currtoken == '+'){
      if(*src == '+'){
        src++;
        currtoken = Inc;
      }
      else currtoken = Add;
      return;
    }

    else if(currtoken == '-'){
      if(*src == '-'){
        src++;
        currtoken = Dec;
      }
      else currtoken = Sub;
      return;
    }

    else if(currtoken == '!'){
      if(*src == '='){
        src++;
        currtoken = Ne;
      }
      return;
    }

    else if(currtoken == '<'){
      if(*src == '='){
        src++;
        currtoken = Le;
      }
      else if(*src == '<'){
        src++;
        currtoken = Shl;
      }
      else{
        currtoken = Lt;
      }
      return;
    }

    else if(currtoken == '>'){
      if(*src == '='){
        src++;
        currtoken = Ge;
      }
      else if(*src == '>'){
        src++;
        currtoken = Shr;
      }
      else{
        currtoken = Gt;
      }
      return;
    }

    else if(currtoken == '|'){
      if(*src == '|'){
        src++;
        currtoken = Lor;
      }
      else currtoken = Or;
      return;
    }

    else if(currtoken == '&'){
      if(*src == '&'){
        src++;
        currtoken = Lan;
      }
      else currtoken = And;
      return;
    }

    else if(currtoken == '^'){
      currtoken = Xor;
      return;
    }

    else if(currtoken == '%'){
      currtoken = Mod;
      return;
    }

    else if(currtoken == '*'){
      currtoken = Mul;
      return;
    }

    else if(currtoken == '['){
      currtoken = Brac;
      return;
    }

    else if(currtoken == '?'){
      currtoken = Cond;
      return;
    }
    else if(currtoken == '~' || currtoken == ';' || currtoken == '{' || currtoken == '}' || \
            currtoken == '(' || currtoken == ')' || currtoken == ']' || currtoken == ',' || \
            currtoken == ':'){

      return;
    }   
  }
}

void
mvn(int vtoken)
{
  /* Match, Verify, Next. */
  if(currtoken == vtoken) lexer();
  else{
    printtxt("Syntax Error: Expected different token than which was received. Line:", "SERIAL", 0, 0);
    printnum(linenumber, "SERIAL", 0, 0);
    exit(-1);
  }
  return;
}

void
expression(int lvl)
{
  int *id;
  int tmp;
  int *addr;

  printtxt("Processing expr", "SERIAL", 0, 0);
{ 
  if(!currtoken){
    printtxt("Token EOF at line: ", "SERIAL", 0, 0);
    printnum(linenumber, "SERIAL", 0, 0);
    exit(-1);
  }

  if(currtoken == Num){
    mvn(Num);

    *++textsegment = LI;
    *++textsegment = tokenvalue;
    type = INT;
  }

  else if(currtoken == '"'){
    *++textsegment = LI;
    *++textsegment = tokenvalue;

    mvn('"');
    while(currtoken == '"') mvn('"');
    datasegment = (char*)(((int)datasegment +  sizeof(int)) & (-sizeof(int)));
    type = PTR;
  }

  else if(currtoken == Sizeof){
    mvn(Sizeof);
    mvn('(');
    type = INT;

    if(currtoken == Int) mvn(Int);
    else if(currtoken == Char){
      mvn(Char);
      type = CHAR;
    }

    while(currtoken == Mul){
      mvn(Mul);
      type = type + PTR;
    }

    mvn(')');

    *++textsegment = LI;
    *++textsegment = (type == CHAR) ? sizeof(char) : sizeof(int);
    type = INT;
  }

  else if(currtoken == Id){
    mvn(Id);
    id = currentidentifier;

    if(currtoken == '('){
      mvn('(');

      tmp = 0;
      while(currtoken != ')'){
        expression(Assign);
        *++textsegment = PSH;
        tmp++;

        if(currtoken == ',') mvn(',');
      }
      mvn(')');

      if(id[CLASS] == Lib){
        *++textsegment = id[VALUE];
        printtxt("Syslib fxn linked with idvalue", "SERIAL", 0, 0);
      }
      else if(id[CLASS] == Fun){
        *++textsegment = CALL;
        *++textsegment = id[VALUE];
      }else{
        printtxt("Bad function call at line: ", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }

      if(tmp > 0){
        *++textsegment = POP;
        *++textsegment = tmp;
      }
      type = id[TYPE];
      
    }
    
    else if(id[CLASS] == Num){
      *++textsegment = LI;
      *++textsegment = id[VALUE];
      type = INT;
    }

    else{
      if(id[CLASS] == Loc){
        *++textsegment = LEA;
        *++textsegment = bpindex - id[VALUE];
      }
      else if(id[CLASS] == Glo){
        *++textsegment = LI;
        *++textsegment = id[VALUE];
      }
      else{
        printtxt("Undefined variable..line: ", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }

      type = id[TYPE];
      *++textsegment = (type == Char) ? LC : LN;
    }
  }

  else if(currtoken == '('){
    mvn('(');
    if(currtoken == Int || currtoken == Char){
      printtxt("At cast", "SERIAL", 0, 0);
      tmp = (currtoken == Char) ? CHAR : INT;
      mvn(currtoken);
      while(currtoken == Mul){
        mvn(Mul);
        tmp = tmp + PTR;
      }

      mvn(')');
      expression(Inc);
      type = tmp;
      
    }else{
      expression(Assign);
      mvn(')');
    }
  }

  else if(currtoken == Mul){
    mvn(Mul);
    expression(Inc);
    printtxt("Dereference", "SERIAL", 0, 0);

    if(type >= PTR) type = type - PTR;
    else{
      printtxt("That's a bad dereference, u suck, line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }

    *++textsegment = (type == CHAR) ? LC : LN;
  }

  else if(currtoken == And){
    mvn(And);
    expression(Inc);

    if(*textsegment  == LC || *textsegment == LN){
      textsegment--;
    }else{
      printtxt("Bad address-get of the thing at line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }

    type = type + PTR;
  }

  else if(currtoken == '!'){
    mvn('!');
    expression(Inc);

    *++textsegment = PSH;
    *++textsegment = LI;
    *++textsegment = 0;
    *++textsegment = EQU;
    type = INT;
  }

  else if(currtoken == '~'){
    mvn('~');
    expression(Inc);

    *++textsegment = PSH;
    *++textsegment = LI;
    *++textsegment = -1;
    *++textsegment = XOR;

    type = INT;
  }

  else if(currtoken == Add){
    mvn(Add);
    expression(Inc);
    type = INT;
  }

  else if(currtoken == Sub){
    mvn(Sub);

    if(currtoken == Num){
      *++textsegment = LI;
      *++textsegment = -tokenvalue;
      mvn(Num);
    }else{
      *++textsegment = LI;
      *++textsegment = -1;
      *++textsegment = PSH;
      expression(Inc);
      *++textsegment = MUL;
    }

    type = INT;
  }

  else if(currtoken == Inc || currtoken == Dec){
    printtxt("Properly parsed inc prefix", "SERIAL", 0, 0);
    tmp = currtoken;
    mvn(currtoken);
    expression(Inc);

    if(*textsegment == LC){
      *textsegment = PSH;
      *++textsegment = LC;
    }
    else if(*textsegment == LN){
      *textsegment = PSH;
      *++textsegment = LN;
    }
    else{
      printtxt("Bad values pre-increment at line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }
    
    *++textsegment = PSH;
    *++textsegment = LI;
    *++textsegment = (type > PTR) ? sizeof(int) : sizeof(char);
    *++textsegment = (tmp == Inc) ? ADD : SUB;
    *++textsegment = (type  == CHAR) ? SC : SN;
    
  }else{
    printtxt("Bad expr", "SERIAL", 0, 0);
    printnum(linenumber, "SERIAL", 0, 0);
    exit(-1);
  }
}
{  
  while(currtoken >= lvl){
    tmp = type;

    if(currtoken == Assign){
      mvn(Assign);
      if(*textsegment == LC || *textsegment == LN){
        *textsegment = PSH;
      }
      else{
        printtxt("crappy lvalue in assignment, missed a LC/LN..", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }
      expression(Assign);
      type = tmp;
      *++textsegment = (type == CHAR) ? SC : SN;
      
    }

    else if(currtoken == Cond){
      mvn(Cond);
      *++textsegment = BEQZ;
      addr = ++textsegment;
      expression(Assign);
      if(currtoken == ':') mvn(':');
      else{
        printtxt("missing colon in ternary conditional at line: ", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
      }
      *addr = (int)(textsegment + 3);
      *++textsegment = JMP;
      addr = ++textsegment;
      expression(Cond);
      *addr = (int)(textsegment + 1);
    }

    else if(currtoken == Lor){
      mvn(Lor);
      *++textsegment = BNEZ;
      addr = ++textsegment;
      expression(Lan);
      *addr = (int)(textsegment + 1);
      type = INT;
    }
    else if(currtoken == Lan){
      printtxt("HERE AT AND", "SERIAL", 0, 0);
      mvn(Lan);
      *++textsegment = BEQZ;
      addr = ++textsegment;
      expression(Or);
      *addr = (int)(textsegment + 1);
      type = INT;
    }

    else if(currtoken == Or){
      mvn(Or);
      *++textsegment = PSH;
      expression(Xor);
      *++textsegment = OR;
      type = INT;
    }

    else if(currtoken == Xor){
      mvn(Xor);
      *++textsegment = PSH;
      expression(And);
      *++textsegment = XOR;
      type = INT;
    }
    
    else if(currtoken == And){
      mvn(And);
      *++textsegment = PSH;
      expression(Eq);
      *++textsegment = AND;
      type = INT;
    }

    else if(currtoken == Eq){
      mvn(Eq);
      *++textsegment = PSH;
      expression(Ne);
      *++textsegment = EQU;
      type = INT;
    }
    
    else if(currtoken == Ne){
      printtxt("Neq", "SERIAL", 0, 0);
      mvn(Ne);
      *++textsegment = PSH;
      expression(Lt);
      *++textsegment = NEQ;
      type = INT;
    }

    else if(currtoken == Lt){
      mvn(Lt);
      *++textsegment = PSH;
      expression(Shl);
      *++textsegment = LST;
      type = INT;
    }

    else if(currtoken == Gt){
      mvn(Gt);
      *++textsegment = PSH;
      expression(Shl);
      *++textsegment = GTT;
      type = INT;
    }

    else if(currtoken == Le){
      mvn(Le);
      *++textsegment = PSH;
      expression(Shl);
      *++textsegment = LEQ;
      type = INT;
    }

    else if(currtoken == Ge){
      mvn(Ge);
      *++textsegment = PSH;
      expression(Shl);
      *++textsegment = GEQ;
      type = INT;
    }

    else if(currtoken == Shl){
      mvn(Shl);
      *++textsegment = PSH;
      expression(Add);
      *++textsegment = SHL;
      type = INT;
    }

    else if(currtoken == Shr){
      mvn(Shr);
      *++textsegment = PSH;
      expression(Add);
      *++textsegment = SHR;
      type = INT;
    }

    else if(currtoken == Add){
      mvn(Add);
      *++textsegment = PSH;
      expression(Mul);
      type = tmp;

      if(type > PTR){
        *++textsegment = PSH;
        *++textsegment = LI;
        *++textsegment = sizeof(int);
        *++textsegment = MUL;
      }
      *++textsegment = ADD;
    }

    else if(currtoken == Sub){
      mvn(Sub);
      *++textsegment = PSH;
      expression(Mul);
      if(tmp > PTR && tmp == type){
        *++textsegment = SUB;
        *++textsegment = PSH;
        *++textsegment = LI;
        *++textsegment = sizeof(int);
        *++textsegment = DIV;
        type = INT;
      }
      else if(tmp > PTR){
        *++textsegment = PSH;
        *++textsegment = LI;
        *++textsegment = sizeof(int);
        *++textsegment = MUL;
        *++textsegment = SUB;
        type = tmp;
      }
      else{
        *++textsegment = SUB;
        type = tmp;
      }
    }

    else if(currtoken == Mul){
      mvn(Mul);
      *++textsegment = PSH;
      expression(Inc);
      *++textsegment = MUL;
      type = tmp;
    }

    else if(currtoken == Div){
      mvn(Div);
      *++textsegment = PSH;
      expression(Inc);
      *++textsegment = DIV;
      type = tmp;
    }

    else if(currtoken == Mod){
      mvn(Mod);
      *++textsegment = PSH;
      expression(Inc);
      *++textsegment = DIV;
      type = tmp;
    }

    else if(currtoken == Inc || currtoken == Dec){
      if(*textsegment == LN){
        *textsegment = PSH;
        *++textsegment = LN;
      }
      else if(*textsegment == LC){
        *textsegment = PSH;
        *++textsegment = LC;
      }
      else{
        printtxt("bad increment value (postfix)", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL",  0, 0);
        exit(-1);
      }

      *++textsegment = PSH;
      *++textsegment = LI;
      *++textsegment = (type > PTR) ? sizeof(int) : sizeof(char);
      *++textsegment = (currtoken == Inc) ? ADD : SUB;
      *++textsegment = (type == CHAR) ? SC : SN;
      *++textsegment = PSH;
      *++textsegment = LI;
      *++textsegment = (type > PTR) ? sizeof(int) : sizeof(char);
      *++textsegment = (currtoken == Inc) ? SUB : ADD;
      mvn(currtoken);
    }
      
    else if(currtoken == Brac){
      mvn(Brac);
      *++textsegment = PSH;
      expression(Assign);
      mvn(']');

      if(tmp > PTR){
        *++textsegment = PSH;
        *++textsegment = LI;
        *++textsegment = sizeof(int);
        *++textsegment = MUL;
      }
      else if(tmp < PTR){
        printtxt("Pointer type needed in brackets at line: ", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }

      type = tmp - PTR;
      *++textsegment = ADD;
      *++textsegment = (type == CHAR) ? LC : LN;
    }
    else{
      printtxt("Compiler error for some wack reason at line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    } 
  } 
}
}


void
statement()
{
  int *opta, *optb;

  printtxt("Processing statement", "SERIAL", 0, 0);

  
  if(currtoken == If){
    printtxt("Processing if statement..", "SERIAL", 0, 0);
    mvn(If);
    mvn('(');
    expression(Assign);
    mvn(')');
    printtxt("Ready to get if statement body", "SERIAL", 0, 0);
    
    *++textsegment = BEQZ;
    opta = ++textsegment;

    statement();
    
    if(currtoken == Else){
      mvn(Else);
      *opta = (int)(textsegment + 3);
      *++textsegment = JMP;
      opta = ++textsegment;
      statement();
    }
    *opta = (int)(textsegment + 1);
  }

  else if(currtoken == While){
    mvn(While);
    opta = textsegment + 1;
    mvn('(');
    printtxt("Parsing while loop, matched first paren", "SERIAL", 0, 0);
    expression(Assign);
    printtxt("Got inner expr", "SERIAL", 0, 0);
    mvn(')');
    printtxt("Matched outgoing paren", "SERIAL", 0, 0);
    
    *++textsegment = BEQZ;
    optb = ++textsegment;
    statement();

    *++textsegment = JMP;
    *++textsegment = (int)opta;
    *optb =  (int)(textsegment + 1);
  }

  else if(currtoken == Return){
    mvn(Return);

    if(currtoken != ';') expression(Assign);
    mvn(';');

    *++textsegment = RET;
  }

  else if(currtoken == '{'){
    mvn('{');
    while(currtoken != '}') statement();
    mvn('}');
  }

  else if(currtoken == ';') mvn(';');
  else{
    expression(Assign);
    mvn(';');
  }
}

void
enumdeclaration()
{
  int i;
  printtxt("Processing enum declaration", "SERIAL", 0, 0);


  i = 0;
  while(currtoken != '}'){
    if(currtoken != Id){
      printtxt("Illegal identifier in enum, line:", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }
    lexer();
    if(currtoken == Assign){
      lexer();
      if(currtoken != Num){
        printtxt("Illegal enum initializer, dummy. Line:", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }
      i = tokenvalue;
      lexer();
    }

    currentidentifier[CLASS] = Num;
    currentidentifier[TYPE] = INT;
    currentidentifier[VALUE] = i++;

    if(currtoken == ',') lexer();
  }
}

void
functionparameter()
{
  int type, params;

  printtxt("Processing function parameter", "SERIAL", 0, 0);
  params = 0;
  while(currtoken != ')'){
    type = INT;
    if(currtoken == Int) mvn(Int);
    else if(currtoken == Char){
      type = CHAR;
      mvn(Char);
    }

    while(currtoken == Mul){
      mvn(Mul);
      type = type + PTR;
    }

    if(currtoken != Id){
      printtxt("Illegal parameter declaration! Line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }
    if(currentidentifier[CLASS] == Loc){
      printtxt("Duplicate parameter in function call..line: ", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }

    mvn(Id);

    currentidentifier[SCLASS] = currentidentifier[CLASS];
    currentidentifier[CLASS] = Loc;
    currentidentifier[STYPE] = currentidentifier[TYPE];
    currentidentifier[TYPE] = type;
    currentidentifier[SVAL] = currentidentifier[VALUE];
    currentidentifier[VALUE] = params++;

    if(currtoken == ',') mvn(',');
  }
  bpindex = params + 1;
}

void
functionbody()
{
  int localposition;
  int type;
  printtxt("Processing function body", "SERIAL", 0, 0);


  localposition = bpindex;

  while(currtoken == Int || currtoken == Char){
    basetype = (currtoken == Int) ? INT : CHAR;
    mvn(currtoken);

    while(currtoken != ';'){
      type = basetype;
      while(currtoken == Mul){
        mvn(Mul);
        type = type + PTR;
      }

      if(currtoken != Id){
        printtxt("Bad local declaration at line: ", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }
      if(currentidentifier[CLASS] == Loc){
        printtxt("Duplicate local declaration at line :", "SERIAL", 0, 0);
        printnum(linenumber, "SERIAL", 0, 0);
        exit(-1);
      }

      mvn(Id);
      currentidentifier[SCLASS] = currentidentifier[CLASS];
      currentidentifier[CLASS] = Loc;
      currentidentifier[STYPE] = currentidentifier[TYPE];
      currentidentifier[TYPE] = type;
      currentidentifier[SVAL] = currentidentifier[VALUE];
      currentidentifier[VALUE] = ++localposition;

      if(currtoken == ',') mvn(',');
    }

    mvn(';');
  }

  *++textsegment = NFRM;
  *++textsegment = localposition - bpindex;

  while(currtoken != '}') statement();
  printtxt("Finished processing fxn body", "SERIAL", 0, 0);
  *++textsegment = RET;
}

void
functiondeclaration()
{

  printtxt("Processing function decl", "SERIAL", 0, 0);
  mvn('(');
  functionparameter();
  mvn(')');
  mvn('{');
  functionbody();

  currentidentifier = symboltable;
  while(currentidentifier[TOKEN]){
    if(currentidentifier[CLASS] == Loc){
      currentidentifier[CLASS] = currentidentifier[SCLASS];
      currentidentifier[TYPE] = currentidentifier[STYPE];
      currentidentifier[VALUE] = currentidentifier[SVAL];
    }
    currentidentifier = currentidentifier + IDSIZE;
  }
}

void
globaldeclaration()
{
  int type, i;

  printtxt("Processing global decl", "SERIAL", 0, 0);
  basetype = INT;

  if(currtoken == Enum){
    mvn(Enum);
    if(currtoken != '{') mvn(Id);
    if(currtoken == '{'){
      mvn('{');
      enumdeclaration();
      mvn('}');
    }

    mvn(';');
    return; 
  }

  if(currtoken == Int) mvn(Int);
  
  else if(currtoken == Char){
    mvn(Char);
    basetype = CHAR;
  }

  while(currtoken != ';' && currtoken != '}'){
    type = basetype;
    while(currtoken == Mul){
      mvn(Mul);
      type = type + PTR;
    }

    if(currtoken != Id){
      printtxt("Bad global declaration at line:", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }

    if(currentidentifier[CLASS]){
      printtxt("Duplicate global declaration found at line:", "SERIAL", 0, 0);
      printnum(linenumber, "SERIAL", 0, 0);
      exit(-1);
    }

    mvn(Id);
    currentidentifier[TYPE] = type;

    if(currtoken == '('){
      currentidentifier[CLASS] = Fun;
      currentidentifier[VALUE] = (int)(textsegment+1);
      functiondeclaration();
    }
    else{
      currentidentifier[CLASS] = Glo;
      currentidentifier[VALUE] = (int)datasegment;
      datasegment = datasegment + sizeof(int);
    }

    if(currtoken == ','){
      mvn(',');
    }
  }
  lexer();
}

void
parser()
{
  lexer();
  printtxt("Runs underway", "SERIAL", 0, 0);
  while(currtoken != 0){
    globaldeclaration();
  }
  printtxt("Finished processing code", "SERIAL", 0, 0);
  return;
}

int
execvm()
{
  int opcode;
  tftfill(0x0000);

  while(1){
    opcode = *pc++;
    if(opcode == LI){
      ax = *pc++;
    }
    else if(opcode == LC){
      ax = *(char*) ax; 
    }
      
    else if(opcode == LN){
      ax = *(int*) ax; 
    }
    else if(opcode == SC)
      ax = *(char*)*sp++ = ax; 
    else if(opcode == SN)
      ax = *(int*)*sp++ = ax; 

    else if(opcode == PSH){
      *--sp = ax; 
    }
    else if(opcode == JMP)
      pc = (int*)*pc; 
      
    else if(opcode == BEQZ)
      pc = ax ? pc + 1 : (int*)*pc;
    else if(opcode == BNEZ)
      pc = ax ? (int*)*pc : pc + 1; 

    else if(opcode == CALL){
      *--sp = (int)(pc + 1); 
      pc = (int*)*pc;
    }
    
    else if(opcode == NFRM){
      *--sp = (int)bp;
      bp = sp; 
      sp = sp - *pc++; 
    }
    
    else if(opcode == POP)
      sp = sp + *pc++; 
      
    else if(opcode == RET){
      sp = bp; 
      bp = (int*)*sp++;
      pc = (int*)*sp++; 
    }
    
    else if(opcode == LEA){
      ax = (int)(bp + *pc++); 

    }
    else if(opcode == OR)
      ax = *sp++ | ax; 
    else if(opcode == XOR)
      ax = *sp++ ^ ax; 
    else if(opcode == AND)
      ax = *sp++ & ax; 
    else if(opcode == EQU)
      ax = *sp++ == ax; 
    else if(opcode == NEQ)
      ax = *sp++ != ax; 

    else if(opcode == LST)
      ax = *sp++ < ax; 
    else if(opcode == LEQ)
      ax = *sp++ <= ax; 
    else if(opcode == GTT)
      ax = *sp++ > ax; 
    else if(opcode == GEQ)
      ax = *sp++ >= ax; 
      
    else if(opcode == SHL)
      ax = *sp++ << ax; 
    else if(opcode == SHR)
      ax = *sp++ >> ax;      
    else if(opcode == ADD)
      ax = *sp++ + ax; 
    else if(opcode == SUB)
      ax = *sp++ - ax; 
    else if(opcode == MUL)
      ax = *sp++ * ax; 
    else if(opcode == DIV)
      ax = *sp++ / ax; 
    else if(opcode == MOD)
      ax = *sp++ % ax; 

    else if(opcode == EXIT)
      return *sp; 
    else if(opcode == HALC)
      ax = (int)heapalloc(*sp); 
    else if(opcode == HFRE)
      heapfree((void*)ax);
    else if(opcode == MCMP)
      ax = memcmp((char*)sp[2], (char*)sp[1], *sp);
      
    else if(opcode == PRTT){
      ax = printtxt((char*)sp[3], (char*)sp[2], sp[1], *sp); 
    }
    else if(opcode == PRTC)
      ax = printchr((char)sp[3], (char*)sp[2], sp[1], *sp);
    else if(opcode == PRTN)
      ax = printnum(sp[3], (char*)sp[2], sp[1], *sp); 
    else if(opcode == TPR)
      tftprint((char*)*sp);
    else if(opcode == TROT)
      tftrotation(*sp);
    else if(opcode == TTXC)
      tfttextcolor(sp[1], *sp);
    else if(opcode == TTXS)
      tfttextsize(*sp);
    else if(opcode == TFS)
      tftfill(*sp);
    else if(opcode == TDC)
      tftdrawcircle(sp[3], sp[2], sp[1], *sp);
    else if(opcode == TDR)
      tftdrawrect(sp[4], sp[3], sp[2], sp[1], *sp);
    else if(opcode == BTRD)
      ax = buttonread(*sp);
    else if(opcode == GET)
      httpget((char*)sp[3], (char*)sp[2], (char*)sp[1], *sp);
    else if(opcode == POST)
      httppost((char*)sp[3], (char*)sp[2], (char*)sp[1], *sp);
    else if(opcode == HSTN)
      gethostname((char*)sp[1], *sp);
    else if(opcode == SHST)
      updatehostname((char*)sp[1], *sp);
    else if(opcode == MILS)
      ax = getmillis();
    else if(opcode == ABS)
      ax = absvalue(*sp);
    else if(opcode == COS)
      ax = cosine(*sp);
    else if(opcode == SIN)
      ax = sine(*sp);
    else if(opcode == TDL)
      tftdrawline(sp[4], sp[3], sp[2], sp[1], *sp);
    else if(opcode == TDP)
      tftdrawpixel(sp[2], sp[1], *sp);
    else if(opcode == TFC)
      tftfillcircle(sp[3], sp[2], sp[1], *sp);
    else if(opcode == TSCU)
      tftsetcursor(sp[1], *sp);
    else if(opcode == IRA)
      imureadaccel((int*)*sp);
    else if(opcode == TFR)
      tftfillrect(sp[4], sp[3], sp[2], sp[1], *sp);
    else if(opcode == TDE)
      tftdrawellipse(sp[4], sp[3], sp[2], sp[1], *sp);
    else if(opcode == TFE)
      tftfillellipse(sp[4], sp[3], sp[2], sp[1], *sp);
    else if(opcode == SQRT)
      ax = root(*sp);
    else if(opcode == ATOI)
      ax = ifroma((char*)*sp);
    else if(opcode == RNG)
      ax = randomint(sp[1], *sp);

    else{
      printtxt("Unknown instruction", "SERIAL", 0, 0);
      printnum(opcode, "SERIAL", 0, 0);
      return -1;
    }
  }
}

void
execprep()
{
  int i;
  
  src = "char else enum if int return sizeof while "
        "exit heapalloc heapfree memcmp printtxt printchr "
        "printnum tftprint tftrotation tfttextcolor "
        "tfttextsize tftfill tftdrawcircle tftdrawrect "
        "buttonread httpget httppost gethostname updatehostname "
        "getmillis absvalue cosine sine tftdrawline "
        "tftdrawpixel tftfillcircle tftsetcursor imureadaccel "
        "tftfillrect tftdrawellipse tftfillellipse root ifroma "
        "randomint void main";

  i = Char;
  while(i <= While){
    lexer();
    currentidentifier[TOKEN] = i++;
  }

  i = EXIT;
  while(i <= RNG){
    lexer();
    currentidentifier[CLASS] = Lib;
    currentidentifier[TYPE] = INT;
    currentidentifier[VALUE] = i++;
    printtxt((char*)currentidentifier[NAME], "SERIAL", 0, 0);
    printnum(currentidentifier[VALUE], "SERIAL", 0, 0);
  }

  lexer();
  currentidentifier[TOKEN] = Char;
  lexer();
  mainptr = currentidentifier;

  tftprint("Ate initializer\n");
  return;
}

int
fpsr(char* srcstr, int bpsize, int spsize)
{
  int *tmp, ret;
  tftprint("Compile time!\n");

  if ((textsegment = (int*)heapalloc(bpsize)) < 0) return -1;
  printtxt("All good", "SERIAL", 0, 0);
  if ((datasegment = (char*)heapalloc(bpsize)) < 0) return -1;
  printtxt("All good", "SERIAL", 0, 0);
  if ((stacksegment = (int*)heapalloc(spsize)) < 0) return -1;
  printtxt("All good", "SERIAL", 0, 0);
  if ((symboltable = (int*)heapalloc(spsize)) < 0) return -1;
  printtxt("All good", "SERIAL", 0, 0);
  tftprint("Memory alloc all good\n");
  
  execprep();

  src = srcstr; /* TRIUMPH!! */
  tftprint("Configuring stack\n");
  
  sp = (int*)((int)stacksegment + spsize);
  ax = 0;
  *--sp = 0;
  *--sp = EXIT;
  *--sp = PSH;
  tmp = sp;
  *--sp = (int)srcstr;
  *--sp = bpsize;
  *--sp = spsize;
  *--sp = (int)tmp;

  tftprint("Compiling\n");

  linenumber = 1;

  parser();
  tftprint("Finding main\n");

  if (!(pc = (int*)mainptr[VALUE])) {
    printtxt("Main not defined!", "SERIAL", 0, 0);
    exit(-1);
  }

  return execvm();
}


void
setup() 
{
  int st;
  importsrc = (char*)heapalloc(CODESIZE);
  bootstrap(importsrc, CODESIZE);
  st = fpsr(importsrc, FPSRALLOC, LITTLEFPSRALLOC);
  if(st) ESP.restart();
}

void loop() {}
