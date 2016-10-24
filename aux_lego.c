#header
<<
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
>>

<<
#include <cstdlib>
#include <cmath>


typedef struct {
  string nom;
  AST *f;
}FUNCIO;

//global structures
AST *root;
Graella g;
vector<FUNCIO> FUNCIONS;
int num = 0; //ASSIGNAR NOM A NOUS BLOCKS

int fit_is;
int altura_fit;

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
  cout << endl;
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


int x_inicial, y_inicial;

bool capBlock(int inix, int iniy, int fix, int fiy, int altura) {

  for(int i = inix; i < inix+fix; ++i) {
    for(int j = iniy; j < iniy+fiy; ++j) {
      if(g.height[i][j] != (altura)) {
        return false;
      }
    }
  }
  x_inicial = inix;
  y_inicial = iniy;
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

bool HiHaEspai(string id, int dx, int dy, int altura) {
    int x,y;
    int xF, yF;
    if (altura == -1)  {
      int x = g.blocks[id].x;
      int y = g.blocks[id].y;
      altura = g.height[x-1][y-1];
    }
    if(fit_is == 1) altura = altura_fit -1;


    x = g.blocks[id].x; //x inicial
    y = g.blocks[id].y; //y inicial
    xF = (x + g.blocks[id].h - dx); //Xinicial + Xfinal - dimBlock
    yF = (y + g.blocks[id].w - dy);
    for(int i = (x-1); i < xF; ++i) {
      for(int j = (y-1); j < yF; ++j) {
        if(capBlock(i,j,dx,dy,altura))  {
          return true;
        }
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

i

bool estaFuncio(string funcio, AST *a, int &i) {
  if(a == NULL) return false;
  else if (child(a,i)->text == funcio) return true;
  return findFuncio(funcio,a,i+1);
}

int getIndexFuncio(string funcio) {
  AST *arrel_definicions = child(root,2); //arrel_funcio = list
  int i = 0;
  if (estaFuncio(funcio,child(arrel_definicions,0),i)) return i;
  else return -1;
}

AST getFuncio(int index) {
  AST *arrel_definicions = child(root,2);
  return child(arrel_definicions,index);
}
int getIndexAmunt(string id1, string id2) {
  int i = 0;
  while (i < g.blocks[id1].amunt.size()) {
    if (g.blocks[id1].amunt[i] == id2) return i;
    ++i;
  }
  return -1; //No esta
}

void llegirLlista(AST *a, int &aux1, int &aux2) {
  aux1 = atoi((child(a,0)->text).c_str());
  aux2 = atoi((child(a,1)->text).c_str());
}

string NumberToString ( int Number )
{
  std::string s;
  std::stringstream out;
  out << Number;
  s = out.str();
  return s;
}


//****************************************************************************//
//***************************APLICACIO DE FUNCIONS****************************//
//****************************************************************************//

bool aplicarPOP(string id_amunt, string id_abaix) {
  int index = getIndexAmunt(id_abaix,id_amunt); //Mira si id_amunt esta en id_abaix
  if(index != -1){ //Si esta amunt

    borrarPosicio(g.blocks[id_amunt].x,g.blocks[id_amunt].h,
                   g.blocks[id_amunt].y,g.blocks[id_amunt].w); //Borrar del mapa
    //Borrar assignacions
    int i = getIndexAmunt(id_abaix, id_amunt);
    g.blocks[id_abaix].amunt.erase(g.blocks[id_abaix].amunt.begin() + i);
   return true;
  }
  else {
      cout << "NO ES POT REALITZAR EL POP" << endl;
      return false;
  }
}

bool aplicarPUSH(string id_amunt, string id_abaix) {
  printMap();

  int dx,dy;
  dx = g.blocks[id_amunt].h;
  dy = g.blocks[id_amunt].w;

  //Si AMUNT es MES GRAN que ABAIX -> NO POT
  if (dx > g.blocks[id_abaix].h || dy > g.blocks[id_abaix].w) return false;
  //Si al BLOC D'ABAIX POT COLOCARSE

  if(HiHaEspai(id_abaix,dx,dy,-1)) {

    if(modificarMapa(x_inicial+1,dx,y_inicial+1,dy)) { //VARIABLES GLOBALS _inicial
      //S'aplicaPUSH

      if (g.blocks[id_amunt].x != 0)
        borrarPosicio(g.blocks[id_amunt].x,dx,g.blocks[id_amunt].y,dy);

      //Noves coordenades
      g.blocks[id_amunt].x = x_inicial+1;
      g.blocks[id_amunt].y = y_inicial+1;

      //Nova assignacio
      g.blocks[id_abaix].amunt.push_back(id_amunt);

      return true;
    }
  }
  else {
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

int evaluarHeight(AST *a) {
  if(g.blocks.find(a->text) != g.blocks.end()) {
      int x = g.blocks[a->text].x;
      int y = g.blocks[a->text].y;
      cout << "L'altura de "<< a->text << " es: " << g.height[x-1][y-1] << endl;
      return g.height[x-1][y-1];
  }
  cout << "NO EXISTEIX BLOCK AMB AQUEST ID" << endl;
  return -1;
}

bool evaluarFIT(AST *a) {
  fit_is = 1;
  string id = child(a,0)->text;
  int dx = 0;
  int dy = 0;
  llegirLlista(child(a,1),dx,dy);
  int altura = atoi((child(a,2)->text).c_str());
  altura_fit = altura;
  if(HiHaEspai(id,dx,dy,altura-1)) {
    return true;
  }
  else return false;
}

bool esCompleixIgualtat(AST* a) {
  string op = a->kind;
  a = child(a,0);
  int cond1, cond2;

  cond1 = evaluarHeight(child(a,0));
  if(cond1 == -1) return false;
  cond2 = atoi(((a->right)->text).c_str());

  if( op == ">" && (cond1 > cond2)) return true;
  else if (op == "<" && (cond1 < cond2)) return true;
  return false;
}

bool evaluarCondicio(AST *a) { //AND
  return esCompleixIgualtat(child(a,0))&&esCompleixIgualtat(child(a,1));
}


//----------------------------------------------------------------------------//
//-----------------------EVALUACIO TIPUS OPERACIONS---------------------------//
//----------------------------------------------------------------------------//


string evaluarPUSHPOP(AST *a, int type) {
    string idB1;
    //FILL 1
    if(a->kind == "id") idB1 = a->text;
    else {
        int dx,dy;
        llegirLlista(a, dx, dy);
        //Assigna ID nou
        num += 1;
        idB1 = "aux" + NumberToString(num);
        //Declarar nou block per a la graella
        tblock block_nou;
        block_nou.h = dx; block_nou.w = dy;
        block_nou.x = 0; block_nou.y = 0;
        g.blocks[idB1] = block_nou;
      }
    //FILL 2
    string idB2;
    if((a->right)->kind == "id") idB2 = (a->right)->text;
    else {
      int type2 = 0;
      if ((a->right)->kind == "POP") type2 = 1;
      idB2 = evaluarPUSHPOP(child(a->right,0),type2);
    }
    //REALITZAR PUSH O POP
    if(idB2 != "") {
      if(type == 0){
        if (aplicarPUSH(idB1,idB2))return idB2;
        }
      else {
        if(aplicarPOP(idB1,idB2)) return idB2;
      }
    }
    return "";
}



void evaluarAccio(AST *a, int &aux1, int &aux2, string &id_afectat){
  string tt = a->kind;
  if(a == NULL) return;
  if(tt == "list") {
    llegirLlista(a,aux1,aux2);
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
    printMap();
  }

  else if (tt == "PUSH" || tt == "POP") {
    int type = 0;
    if (tt == "POP") type = 1;
    string id_nouBlock = evaluarPUSHPOP(child(a,0), type);
    if(id_nouBlock != "") {
      g.blocks[id_afectat] = g.blocks[id_nouBlock];
      if(id_afectat != id_nouBlock) {
        g.blocks[id_nouBlock].x = 0;
        g.blocks[id_nouBlock].y = 0;
        vector<string> vbuit;
        g.blocks[id_nouBlock].amunt = vbuit;
      }
    }
    printMap();
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
      else cout << "No s'ha pogut fer MOVE" << endl;
    }
    else cout << "No s'ha pogut fer MOVE" << endl;
}


//----------------------------------------------------------------------------//
//---------------------------EVALUACIONS PRINCIPALS---------------------------//
//----------------------------------------------------------------------------//

bool evaluarOperacio(AST *a) {
  if(a == NULL) return true;
  else if(a->kind == "=") {
    int n = 0;
    int n2 = 0;
    string s = "";
    evaluarAccio(child(a,0),n,n2,s);
  }
  else if(a->kind == "MOVE") evaluarMove(child(a,0));
  else if(a->kind == "HEIGHT") evaluarHeight(child(a,0));
  else if(a->kind == "WHILE") {
      //CONDICIO
      while(evaluarOperacio(child(a,0)))
        evaluarOperacio(child(child(a,1),0));
      fit_is = 0;
  }
  else if(a->kind == "FITS") return evaluarFIT(a);
  else if(a->kind == "AND") return evaluarCondicio(a);
  else if (a->kind == "id") {
    int index = getIndexFuncio(a->text);
    if(index != -1) {
      AST *arrel_funcio = getFuncio(index)
      evaluarOperacio(arrel_funcio);
      printMap();
    }
  }
  evaluarOperacio(a->right);
  return true;
}

void definirFuncio(AST *a) {
    //FILL 2: ARBRE REALITZADOR
    FUNCIO F;
    F.nom = child(a,0)->text; //FILL 1 : NOM FUNCIO
    F.f = child(child(a,1),0);
    FUNCIONS.push_back(F);
}

void definirTaulell(AST *a) {

  g.n = atoi((child(a,0)->text).c_str());
  g.m = atoi((child(a,1)->text).c_str());

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
  definirTaulell(child(a,0));
  evaluarOperacio(child(a,1));
}

int main() {
  root = NULL;
  ANTLR(lego(&root), stdin);
  ASTPrint(root);
  executeListInstrucctions(root);
}
>>

#lexclass START
#token SPACE "[\ \n]" << zzskip();>>
#token GRID "GRID"
#token DEF "DEF"
#token NUM "[0-9]+"
#token ID "[A-Z]+[0-9]+"
#token ASSIG "\="
#token PO "\("
#token PC "\)"
#token COM "\,"
#token CLAU1 "\["
#token CLAU2 "\]"
#token LT "\<"
#token BT "\>"
#token ENDEF "ENDEF"

//DIRECTIONS
#token WEST "WEST"
#token SOUTH "SOUTH"
#token NORTH "NORTH"
#token EST "EAST"
#token MOVE "MOVE"

//ACTIONS
#token POP "POP"
#token PUSH "PUSH"

//DEFINITIONS
#token PLACE "PLACE"
#token AT "AT"
#token HEIGHT "HEIGHT"

//LOOP
#token WHILE "WHILE"
#token FITS "FITS"
#token AND "AND"


lego: grid ops defs <<#0=createASTlist(_sibling);>>;


//ESTRUCTURA PRINCIPAL
grid: GRID^ NUM NUM;
ops: (instruction)* <<#0=createASTlist(_sibling);>>;
defs: (def)* <<#0=createASTlist(_sibling);>>;


//1.OPERACIONS
instruction: action | mov | loop | height;

//2.DECLARACIO DE FUNC.
def: DEF^ ID ops ENDEF!;

//---FUNCIONS
    action: ID ASSIG^ (where | pp);
    mov: MOVE^ ID dir NUM;
    loop: WHILE^ PO! cond PC! make;
    height: HEIGHT^ PO! ID PC!;
    fit: FITS^ PO! ID COM! coord COM! NUM PC!;


//----------SOBRE ACCIONS
            where: PLACE^ PO! coord PC! AT! PO! coord PC!;
            pp: type (PUSH^| POP^) pp2;
            pp2: ID (POP^ ID|PUSH^ ID)*;

//----------LOOP
            cond: req (AND^ req)*;
            req: fit | height (LT^|BT^) NUM;
            make: CLAU1! ops CLAU2!;

//----------POSICIONS
            coord: NUM COM! NUM <<#0=createASTlist(_sibling);>>;
            dir: NORTH | SOUTH | EST | WEST;

//----------DADES
            type: ID | PO! coord PC!;
