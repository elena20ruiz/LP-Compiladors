#header
<<
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
>>

<<
#include <cstdlib>
#include <cmath>

//global structures
AST *root;


//****************************************************************************//
//*******************************TOKEN INFORMATION****************************//
//****************************************************************************//

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {

  if (type == ID) {
    attr->kind = "id";
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



int main() {
  root = NULL;
  ANTLR(lego(&root), stdin);
  ASTPrint(root);
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

grid: GRID^ NUM NUM;
ops: (instruction)* <<#0=createASTlist(_sibling);>>;
defs: (def)* <<#0=createASTlist(_sibling);>>;


//OPERACIONS
instruction: op | mov | loop | req;
//---Instruccions
    req: fit
        | height ((LT^|BT^) NUM)*;

    mov: MOVE^ ID dir NUM;
    height: HEIGHT^ PO! ID PC!;
    loop: WHILE^ PO! cond PC! make;
    fit: FITS^ PO! ID COM! coord COM! NUM PC!;
    op: decl | ID;

//---Asignaciones
    decl: ID ASSIG^ (where | pp);
    where: PLACE^ PO! coord PC! AT! PO! coord PC!;
    pp: type (PUSH^| POP^) pp2;
    pp2: ID (POP^ ID|PUSH^ ID)*;

//DECLARACIO DE FUNCTIONS
def: DEF^ ID ops ENDEF!;

//AUXILARS

//----------LOOP
            cond: req (AND^ req)*;
            make: CLAU1! decl CLAU2! <<#0=createASTlist(_sibling);>>;

//----------POSICIONS
            coord: NUM COM! NUM <<#0=createASTlist(_sibling);>>;
            dir: NORTH | SOUTH | EST | WEST;

type: ID | PO! coord PC!;
