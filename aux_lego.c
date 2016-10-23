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

int getIndexAmunt(string id1, string id2) {

  int i = 0;
  while (i < g.blocks[id1].amunt.size()) {
    if (g.blocks[id1].amunt[i] == id2) return i;
    ++i;
  }
  return -1; //No esta
}

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
  int x,y,dx,dy;
  x = g.blocks[id_abaix].x;
  y = g.blocks[id_abaix].y;
  dx = g.blocks[id_amunt].h;
  dy = g.blocks[id_amunt].w;

  //Si AMUNT es MES GRAN que ABAIX -> NO POT
  if (dx > g.blocks[id_abaix].h || dy > g.blocks[id_abaix].w) return false;

  //Si al BLOC D'ABAIX POT COLOCARSE
  if(modificarMapa(x,dx,y,dy)) {
    //S'aplicaPUSH
    if (g.blocks[id_amunt].x != 0) {
      borrarPosicio(g.blocks[id_amunt].x,dx,g.blocks[id_amunt].y,dy);
    }
    //Noves coordenades
    g.blocks[id_amunt].x = x;
    g.blocks[id_amunt].y = y;
    //Nova assignacio
    g.blocks[id_abaix].amunt.push_back(id_amunt);
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
        g.blocks[idB1] = block_nou;
      }
    //FILL 2
    string idB2;
    cout << "GI" << endl;
    if((a->right)->kind == "id") idB2 = (a->right)->text;
    else {

      int type2 = 0;
      if ((a->right)->kind == "POP") type2 = 1;
      cout << "GI2" << endl;
      idB2 = evaluarPUSHPOP(child(a,0),type2);
      cout << "GI2" << endl;
    }
    cout << "VOLEM FER PP DE "<< idB1 << " SOBRE "<< idB2 << endl;
    //REALITZAR PUSH O POP
    if(idB2 != "") {
      if(type == 0){
        cout << "VOLEM FER PUSH DE "<< idB1 << " SOBRE "<< idB2 << endl;
        if (aplicarPUSH(idB1,idB2))return idB2;

      }
      else {
        cout << "VOLEM FER POP DE "<< idB1 << " SOBRE "<< idB2 << endl;
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
    else cout << "NO S'HA REALITZAT L'ACCIO PUSH/POP" << endl;
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
          cout << "No s'ha pogut" << endl;
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
