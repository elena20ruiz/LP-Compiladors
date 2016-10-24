/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: lego.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

//****************************************************************************//
//*****************************ESTRUCTURES************************************//
//****************************************************************************//

typedef struct {
  int x,y; //punt cantonada superior esquerra
  int h,w; //DIMENSIONS
  vector<string> amunt;
} tblock;

typedef struct {
  int n,m;
  vector< vector<int> > height;
  map<string, tblock> blocks;
} Graella;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

//****************************************************************************//
//****************************FUNCIONS EVALUATIVES****************************//
//****************************************************************************//
//void definirTaulell(AST* a);
//void executeListInstrucctions(AST *a);

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr,int ttype, char *textt);
#define zzSET_SIZE 4
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[29]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"SPACE",
	/* 03 */	"GRID",
	/* 04 */	"DEF",
	/* 05 */	"NUM",
	/* 06 */	"ID",
	/* 07 */	"ASSIG",
	/* 08 */	"PO",
	/* 09 */	"PC",
	/* 10 */	"COM",
	/* 11 */	"CLAU1",
	/* 12 */	"CLAU2",
	/* 13 */	"LT",
	/* 14 */	"BT",
	/* 15 */	"ENDEF",
	/* 16 */	"WEST",
	/* 17 */	"SOUTH",
	/* 18 */	"NORTH",
	/* 19 */	"EST",
	/* 20 */	"MOVE",
	/* 21 */	"POP",
	/* 22 */	"PUSH",
	/* 23 */	"PLACE",
	/* 24 */	"AT",
	/* 25 */	"HEIGHT",
	/* 26 */	"WHILE",
	/* 27 */	"FITS",
	/* 28 */	"AND"
};
SetWordType zzerr1[4] = {0x40,0x0,0x10,0x6};
SetWordType zzerr2[4] = {0x40,0x1,0x80,0x0};
SetWordType setwd1[29] = {0x0,0x7b,0x0,0x0,0x6a,0x0,0xa6,
	0x0,0x80,0x0,0x0,0x0,0x28,0x0,0x0,
	0x28,0x0,0x0,0x0,0x0,0x26,0x0,0x0,
	0x0,0x0,0x26,0x26,0x0,0x0};
SetWordType zzerr3[4] = {0xd2,0x90,0x10,0x6};
SetWordType zzerr4[4] = {0x0,0x0,0x60,0x0};
SetWordType setwd2[29] = {0x0,0xdf,0x0,0x0,0xdf,0x0,0xdf,
	0x0,0x0,0x20,0x0,0x0,0xdf,0x10,0x10,
	0xdf,0x0,0x0,0x0,0x0,0xdf,0x0,0x0,
	0x0,0x0,0xdf,0xdf,0x0,0x20};
SetWordType zzerr5[4] = {0x0,0x60,0x0,0x0};
SetWordType zzerr6[4] = {0x0,0x0,0x0,0xa};
SetWordType zzerr7[4] = {0x0,0x0,0xf,0x0};
SetWordType zzerr8[4] = {0x40,0x1,0x0,0x0};
SetWordType setwd3[29] = {0x0,0x12,0x0,0x0,0x12,0x40,0x12,
	0x0,0x0,0x2c,0x20,0x0,0x12,0x0,0x0,
	0x12,0x0,0x0,0x0,0x0,0x12,0x81,0x81,
	0x0,0x0,0x12,0x12,0x0,0x8};
