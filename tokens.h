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
#define NUM 4
#define ID 5
#define ASSIG 6
#define PO 7
#define PC 8
#define COM 9
#define WEST 10
#define SOUTH 11
#define NORTH 12
#define EST 13
#define MOVE 14
#define POP 15
#define PUSH 16
#define PLACE 17
#define AT 18

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
void decl(AST**_root);
#else
extern void decl();
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
void type(AST**_root);
#else
extern void type();
#endif

#ifdef __USE_PROTOS
void pp2(AST**_root);
#else
extern void pp2();
#endif

#ifdef __USE_PROTOS
void id(AST**_root);
#else
extern void id();
#endif

#ifdef __USE_PROTOS
void mov(AST**_root);
#else
extern void mov();
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

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType setwd2[];
