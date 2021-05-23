#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

#include <set>
#include <list>
#include <map>
#include <stack>

enum Basicness {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
    List<CgenNode> *nds;
    ostream& str;

    std::map<Symbol, std::map<Symbol, Feature> > attribute_table;
    std::map<Symbol, std::map<Symbol, Feature> > method_table;
    int _max_tag; // current max tag

// The following methods emit code for
// constants and global declarations.
    void code_global_data();
    void code_global_text();
    void code_bools(int);
    void code_select_gc();
    void code_constants();

// The following methods emit code for
// _parentTab, _attrTabTab, _dispTab, _protObj
    void code_name_and_obj_table();
    void code_attr_and_dispatch_table();
    void code_parentTab();
    void code_protObj();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.
    void install_basic_classes();
    void install_class(CgenNodeP nd);
    void install_classes(Classes cs);
    void build_inheritance_tree();
    void set_relations(CgenNodeP nd);
public:
    CgenClassTable(Classes, ostream& str);
    void code();
    CgenNodeP root();
    void print_CgenClassTable();
    void print_inheritance_graph();
    void print_attribute_table();
    bool isInt(Symbol typeID);
    bool isString(Symbol typeID);
    bool isBool(Symbol typeID);
};


class CgenNode : public class__class {
private: 
    CgenNodeP parentnd;         // Parent of class
    List<CgenNode> *children;   // Children of class
    Basicness basic_status;     // `Basic' if class is basic, `NotBasic' otherwise
    int tag;                    // Tag of class

public:
    CgenNode(Class_ c,
        Basicness bstatus,
        CgenClassTableP class_table);

    void add_child(CgenNodeP child);
    List<CgenNode> *get_children() { return children; }
    void set_parentnd(CgenNodeP p);
    CgenNodeP get_parentnd() { return parentnd; }
    int basic() { return (basic_status == Basic); }
    void set_tag(int tag) { this->tag = tag; }
    int get_tag() { return this->tag; }
};

class BoolConst 
{
private: 
    int val;
public:
    BoolConst(int);
    void code_def(ostream&, int boolclasstag);
    void code_ref(ostream&) const;
};
