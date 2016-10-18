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
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

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

ANTLRChar *zztokens[19]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"SPACE",
	/* 03 */	"GRID",
	/* 04 */	"NUM",
	/* 05 */	"ID",
	/* 06 */	"ASSIG",
	/* 07 */	"PO",
	/* 08 */	"PC",
	/* 09 */	"COM",
	/* 10 */	"WEST",
	/* 11 */	"SOUTH",
	/* 12 */	"NORTH",
	/* 13 */	"EST",
	/* 14 */	"MOVE",
	/* 15 */	"POP",
	/* 16 */	"PUSH",
	/* 17 */	"PLACE",
	/* 18 */	"AT"
};
SetWordType zzerr1[4] = {0xa0,0x0,0x2,0x0};
SetWordType zzerr2[4] = {0x0,0x80,0x1,0x0};
SetWordType zzerr3[4] = {0xa0,0x0,0x0,0x0};
SetWordType setwd1[19] = {0x0,0xeb,0x0,0x0,0x0,0xf6,0x0,
	0x10,0x0,0x0,0x0,0x0,0x0,0x0,0xe6,
	0x0,0x0,0x0,0x0};
SetWordType zzerr4[4] = {0x0,0x3c,0x0,0x0};
SetWordType setwd2[19] = {0x0,0x3c,0x0,0x0,0x40,0x3c,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c,
	0x2b,0x2b,0x0,0x20};
