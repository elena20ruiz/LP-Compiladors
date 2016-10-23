/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt lego.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 4
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>

//global structures
AST *root;
Graella g;

//****************************************************************************//
//*******************************TOKEN INFORMATION****************************//
//****************************************************************************//

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  
  if (type == ID) {
    attr->kind = "id";
    attr->text = text;
  }
  else if(type == NUM) {
    attr->kind = "NUM";
    attr->text = text;
  }
  else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind;
  as->text = attr->text;
  as->right = NULL;
  as->down = NULL;
  return as;
}


/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
  AST *as=new AST;
  as->kind="list";
  as->right=NULL;
  as->down=child;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
  AST *c=a->down;
  for (int i=0; c!=NULL && i<n; i++) c=c->right;
  return c;
}



/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;
  

  if (a->text!="") cout<<a->text;
  else cout<<a->kind;
  cout<<endl;
  
  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
  
  if (i!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
}

/// print AST
void ASTPrint(AST *a)
{
  while (a!=NULL) {
    cout<<" ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}


void printMap() {
  
  for(int i = 0; i < g.n; ++i) {
    
    for(int j = 0; j < g.m; ++j) {
      if(g.height[i][j] == 0) cout << "_";
      else cout << g.height[i][j];
    }
    cout << endl;
  }
  
}

//****************************************************************************//
//-------------------------OPERACIONS DEL JOC---------------------------------//
//----------------------------------------------------------------------------//
void borrarPosicio(int x, int dx, int y ,int dy) {
  
    for(int i = (x-1); i < (x+dx-1); ++i) {
    for(int j = (y-1); j < (y+dy-1); ++j) {
      g.height[i][j] -= 1;
    }
  }
}

bool estaDinsGraella(int x, int y) {
  if(x >= 1 && x <=g.n && y >=1 && y<=g.m) return true;
  return false;
}


bool modificarMapa(int x, int dx, int y, int dy)
{
  int altura = g.height[x-1][y-1];
  vector <vector <int> > height = g.height;
  for(int i = (x-1); i < (x+dx-1); ++i) {
    for(int j = (y-1); j < (y+dy-1); ++j) {
      if(height[i][j] == altura) height[i][j] += 1;
      else return false;
    }
  }
  g.height = height; //POSSAR AL ORIGINAL
  return true;
}


bool esPossibleModificarMapa(string id, tblock &tb) {
  if(estaDinsGraella(tb.x,tb.y)) {
    int espaix = tb.x + tb.h;
    int espaiy = tb.y + tb.w;
    if(estaDinsGraella(espaix,espaiy)) {
      return modificarMapa(tb.x,tb.h,tb.y,tb.w);
    }
  }
  return false;
}

void definirNouBlock(string id, tblock &tb) {
  if (esPossibleModificarMapa(id, tb)) {
    g.blocks[id] = tb;
    cout << "El block " << id << "de dimensions (" << tb.h << "," << tb.w;
    cout << ") s'ha colocat en (" << tb.x << "," << tb.y << ")" << endl;
  }
  else {
    cout << "No es possible colocal el block" << endl;
  }
}

bool aplicarPOP(tblock &tb_amunt, tblock &tb_abaix) {
  
  if(tb_amunt.x == g.blocks[tb_abaix.amunt.back()].x){
    borrarPosicio(tb_amunt.x,tb_amunt.h,tb_amunt.y,tb_amunt.w);
    tb_amunt.x = 0;
    tb_amunt.y = 0;
    return true;
  }
  else {
    cout << "NO ES POT REALITZAR EL POP" << endl;
    return false;
  }
}

bool aplicarPUSH(tblock &tb_amunt, string id_amunt, tblock &tb_abaix) {
  
  int x,y,dx,dy;
  x = tb_abaix.x;
  y = tb_abaix.y;
  dx = tb_amunt.h;
  dy = tb_amunt.w;
  
  if (dx > tb_abaix.h || dy > tb_abaix.w) return false;
  if(modificarMapa(x,dx,y,dy)) { //Si es pot ya s'aplica
    if (tb_amunt.x != 0) borrarPosicio(tb_amunt.x,dx,tb_amunt.y,dy);
    tb_amunt.x = x;
    tb_amunt.y = y;
    if(id_amunt != "") g.blocks[id_amunt] = tb_amunt;
    
    return true;
  }
  else {
  cout << "NO ES POT REALITZAR EL PUSH/POP" << endl;
  return false;
}
}


//----------------------------------------------------------------------------//
//-----------------------EVALUACIO AUXILIARS----------------------------------//
//----------------------------------------------------------------------------//

pair<int,int> parseMov(string dir, int dist){
pair<int,int> p;
p.first = 0;
p.second = 0;
if(dir == "EAST") p.second = dist;
else if(dir == "WEST") p.second = -dist;
else if(dir == "NORTH") p.first = -dist;
else p.first = dist;
return p;
}

void evaluarHeight(AST *a) {
int x = g.blocks[a->text].x;
int y = g.blocks[a->text].y;
cout << "L'altura de "<< a->text << " es: " << g.height[x-1][y-1] << endl;
}

void evaluarLoop(AST *a){

}


//----------------------------------------------------------------------------//
//-----------------------EVALUACIO TIPUS OPERACIONS---------------------------//
//----------------------------------------------------------------------------//

int num = 0;


tblock realitzarAccioPUSHPOP(tblock &tb_ant, string &id_retorn, string &id_act, int type){

  tblock tbuit;
tbuit.x = 0; tbuit.y = 0; tbuit.h = 0; tbuit.w = 0;
if(type == 0) {
if (aplicarPUSH(tb_ant,id_retorn,g.blocks[id_act])) {
g.blocks[id_act].amunt.push_back(id_retorn);
id_retorn = id_act;
return g.blocks[id_act];
}
else cout << "No sha pogut realizar el PUSH" << endl;
}
else {
if(aplicarPOP(tb_ant,g.blocks[id_act])) {
//Afectacios, block id_ret(tb_ant) desapareix del mapa
g.blocks[id_act].amunt.pop_back();
g.blocks[id_retorn] = tb_ant;
id_retorn = id_act;
return g.blocks[id_act];
}
else cout << "No sha pogut realizar el POP" << endl;
}
return tbuit;
}


tblock evaluarPUSHPOP(AST *a, tblock &tb_ant, string &id_retorn, int type) {
tblock tbuit;
tbuit.x = 0; tbuit.y = 0; tbuit.h = 0; tbuit.w = 0;
string node = a->kind;
if(node == "list") {
int aux1 = atoi((child(a,0)->text).c_str());
int aux2 = atoi((child(a,1)->text).c_str());
tb_ant.h = aux1;
tb_ant.w = aux2;
num +=1;
id_retorn = "aux" + num;
return evaluarPUSHPOP(a->right,tb_ant,id_retorn,type);
}
else if (node == "id" && tb_ant.h == 0) {
//ID EXISTEIX
if(g.blocks.find(a->text) != g.blocks.end()) { //Existeix block
tb_ant = g.blocks[a->text];
id_retorn = a->text;
return evaluarPUSHPOP(a->right,tb_ant,id_retorn,type);
}
else {
cout << "NO EXISTEIX BLOCK AMB AQUEST ID" << endl;
return tbuit;
}
}
else if (node == "id") {
if(g.blocks.find(a->text) != g.blocks.end()) { //Existeix block
string id_act = a->text;
return realitzarAccioPUSHPOP(tb_ant,id_retorn,id_act,type);
}
}
else {
tblock t;
t.x = 0; t.y = 0; t.w=0; t.h = 0;
string ant = "";
int ttype = 0;
if(node == "POP") ttype = 1;
tblock block = evaluarPUSHPOP(child(a,0),t,ant,ttype);
cout << tb_ant.x << id_retorn << ant << type << endl;
return realitzarAccioPUSHPOP(tb_ant,id_retorn,ant,type);
}
}


void evaluarAccio(AST *a, int &aux1, int &aux2, string &id_afectat){
string tt = a->kind;
if(a == NULL) return;
if(tt == "list") {
aux1 = atoi((child(a,0)->text).c_str());
aux2 = atoi((child(a,1)->text).c_str());
}

  else if (tt == "id") {
id_afectat = a->text;
evaluarAccio(a->right,aux1,aux2,id_afectat);
}

  else if( tt == "PLACE") {
tblock tb;
evaluarAccio(child(a,0), tb.h, tb.w, id_afectat);
evaluarAccio(child(a,1), tb.x, tb.y, id_afectat);

    definirNouBlock(id_afectat,tb); //OPERACIO
}

  else if (tt == "PUSH" || tt == "POP") {
int type = 0;
if (a->kind == "POP") type = 1;
tblock tb;
tb.x = 0; tb.y = 0; tb.h = 0; tb.w= 0;
string id_retorn;
tblock tb_nou = evaluarPUSHPOP(child(a,0), tb, id_retorn, type);
g.blocks[id_afectat] = tb_nou;
}
}




void evaluarMove(AST *a){

    string id = a->text; //1er element
string dir = (a->right)->kind; //2on element
int dist = atoi((((a->right)->right)->text).c_str()); //3er element

    pair<int,int> moviment = parseMov(dir,dist);
int px = g.blocks[id].x + moviment.first;
int py = g.blocks[id].y + moviment.second;

    int dx = g.blocks[id].h;
int dy = g.blocks[id].w;

    if(estaDinsGraella(px,py) && estaDinsGraella((dx+px),(dy+py))){
if(modificarMapa(px,dx,py,dy)) {
borrarPosicio(g.blocks[id].x,dx,g.blocks[id].y,dy);
g.blocks[id].x = px;
g.blocks[id].y = py;
cout << "S'ha mogut el block" << id << endl;
printMap();
}
else {
cout << "No s'ha pogut2" << endl;
}
}
else {
cout << "No s'ha pogut" << endl;
}
}

void evaluarWhile(AST *a){

}


//----------------------------------------------------------------------------//
//---------------------------EVALUACIONS PRINCIPALS---------------------------//
//----------------------------------------------------------------------------//

void evaluarOperacio(AST *a) {
if(a == NULL) return;
else if(a->kind == "=") {
int n = 0;
int n2 = 0;
string s = "";
evaluarAccio(child(a,0),n,n2,s);
printMap();
}
else if(a->kind == "MOVE") evaluarMove(child(a,0));
else if(a->kind == "HEIGHT") evaluarHeight(child(a,0));
else if(a->kind == "WHILE") evaluarWhile(child(a,0));
evaluarOperacio(a->right);
}

void definirFuncio(AST *a) {


}

void definirTaulell(AST *a) {
g.n = atoi((a->text).c_str());
g.m = atoi(((a->right)->text).c_str());

  vector <vector <int> > map (g.n, vector<int> (g.m));
g.height = map;
for (int i = 0; i < g.n; ++i) {
for(int j = 0; j < g.m; ++j) {
g.height[i][j] = 0;
}
}
cout << "S'ha creat el taulell de " << g.m << "x" << g.n << endl;
printMap();
}

void executeListInstrucctions(AST *a) {
if(a == NULL) return;
else if(a->kind == "GRID") definirTaulell(child(a,0));
else if(a->kind == "list") evaluarOperacio(child(a,0));
else if(a->kind == "DEF") definirFuncio(child(a,0));
executeListInstrucctions(a->right);
}


int main() {
root = NULL;
ANTLR(lego(&root), stdin);
ASTPrint(root);
executeListInstrucctions(child(root,0));
}

void
#ifdef __USE_PROTOS
lego(AST**_root)
#else
lego(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  grid(zzSTR); zzlink(_root, &_sibling, &_tail);
  ops(zzSTR); zzlink(_root, &_sibling, &_tail);
  defs(zzSTR); zzlink(_root, &_sibling, &_tail);
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
grid(AST**_root)
#else
grid(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(GRID); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
ops(AST**_root)
#else
ops(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x4) ) {
      instruction(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x8);
  }
}

void
#ifdef __USE_PROTOS
defs(AST**_root)
#else
defs(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==DEF) ) {
      def(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

void
#ifdef __USE_PROTOS
instruction(AST**_root)
#else
instruction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    action(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==MOVE) ) {
      mov(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==WHILE) ) {
        loop(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==HEIGHT) ) {
          height(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
def(AST**_root)
#else
def(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(DEF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  ops(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDEF);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
action(AST**_root)
#else
action(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==PLACE) ) {
      where(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd1[LA(1)]&0x80) ) {
        pp(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
mov(AST**_root)
#else
mov(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MOVE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  dir(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
loop(AST**_root)
#else
loop(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PO);  zzCONSUME;
  cond(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(PC);  zzCONSUME;
  make(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
height(AST**_root)
#else
height(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(HEIGHT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PO);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PC);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
fit(AST**_root)
#else
fit(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(FITS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PO);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COM);  zzCONSUME;
  coord(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COM);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PC);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
where(AST**_root)
#else
where(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(PLACE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(PO);  zzCONSUME;
  coord(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(PC);  zzCONSUME;
  zzmatch(AT);  zzCONSUME;
  zzmatch(PO);  zzCONSUME;
  coord(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(PC);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

void
#ifdef __USE_PROTOS
pp(AST**_root)
#else
pp(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  type(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==PUSH) ) {
      zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==POP) ) {
        zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  pp2(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

void
#ifdef __USE_PROTOS
pp2(AST**_root)
#else
pp2(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    for (;;) {
      if ( !((setwd2[LA(1)]&0x80))) break;
      if ( (LA(1)==POP) ) {
        zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==PUSH) ) {
          zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else break; /* MR6 code for exiting loop "for sure" */
      }
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
cond(AST**_root)
#else
cond(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  req(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      req(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
req(AST**_root)
#else
req(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==FITS) ) {
    fit(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==HEIGHT) ) {
      height(zzSTR); zzlink(_root, &_sibling, &_tail);
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        if ( (LA(1)==LT) ) {
          zzmatch(LT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==BT) ) {
            zzmatch(BT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp2);
        }
      }
      zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
make(AST**_root)
#else
make(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(CLAU1);  zzCONSUME;
  ops(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(CLAU2);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

void
#ifdef __USE_PROTOS
coord(AST**_root)
#else
coord(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COM);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail);
  (*_root)=createASTlist(_sibling);
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
dir(AST**_root)
#else
dir(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==NORTH) ) {
    zzmatch(NORTH); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==SOUTH) ) {
      zzmatch(SOUTH); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==EST) ) {
        zzmatch(EST); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==WEST) ) {
          zzmatch(WEST); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

void
#ifdef __USE_PROTOS
type(AST**_root)
#else
type(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==PO) ) {
      zzmatch(PO);  zzCONSUME;
      coord(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzmatch(PC);  zzCONSUME;
    }
    else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}
