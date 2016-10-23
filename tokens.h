#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: lego.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define SPACE 2
#define GRID 3
#define DEF 4
#define NUM 5
#define ID 6
#define ASSIG 7
#define PO 8
#define PC 9
#define COM 10
#define CLAU1 11
#define CLAU2 12
#define LT 13
#define BT 14
#define ENDEF 15
#define WEST 16
#define SOUTH 17
#define NORTH 18
#define EST 19
#define MOVE 20
#define POP 21
#define PUSH 22
#define PLACE 23
#define AT 24
#define HEIGHT 25
#define WHILE 26
#define FITS 27
#define AND 28

#ifdef __USE_PROTOS
void lego(AST**_root);
#else
extern void lego();
#endif

#ifdef __USE_PROTOS
void grid(AST**_root);
#else
extern void grid();
#endif

#ifdef __USE_PROTOS
void ops(AST**_root);
#else
extern void ops();
#endif

#ifdef __USE_PROTOS
void defs(AST**_root);
#else
extern void defs();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void def(AST**_root);
#else
extern void def();
#endif

#ifdef __USE_PROTOS
void action(AST**_root);
#else
extern void action();
#endif

#ifdef __USE_PROTOS
void mov(AST**_root);
#else
extern void mov();
#endif

#ifdef __USE_PROTOS
void loop(AST**_root);
#else
extern void loop();
#endif

#ifdef __USE_PROTOS
void height(AST**_root);
#else
extern void height();
#endif

#ifdef __USE_PROTOS
void fit(AST**_root);
#else
extern void fit();
#endif

#ifdef __USE_PROTOS
void where(AST**_root);
#else
extern void where();
#endif

#ifdef __USE_PROTOS
void pp(AST**_root);
#else
extern void pp();
#endif

#ifdef __USE_PROTOS
void pp2(AST**_root);
#else
extern void pp2();
#endif

#ifdef __USE_PROTOS
void cond(AST**_root);
#else
extern void cond();
#endif

#ifdef __USE_PROTOS
void req(AST**_root);
#else
extern void req();
#endif

#ifdef __USE_PROTOS
void make(AST**_root);
#else
extern void make();
#endif

#ifdef __USE_PROTOS
void coord(AST**_root);
#else
extern void coord();
#endif

#ifdef __USE_PROTOS
void dir(AST**_root);
#else
extern void dir();
#endif

#ifdef __USE_PROTOS
void type(AST**_root);
#else
extern void type();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType setwd2[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd3[];
