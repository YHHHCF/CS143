#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

#include <set>
#include <list>
#include <map>
#include <stack>
#include <vector>

enum Basicness {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class Environment;
typedef Environment *Environmentp;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
    List<CgenNode> *nds;
    ostream& str;

    // key is class typeID
    // value is a vector of attr objectIDs for attribute insertion order
    std::map<Symbol, std::vector<Symbol> > attribute_order;
    // key is class typeID
    // value is a map of objectIDs to attribute Features
    std::map<Symbol, std::map<Symbol, Feature> > attribute_table;
    // key is class typeID
    // value is a vector of methodIDs for method insertion order
    std::map<Symbol, std::vector<Symbol> > method_order;
    // key is class typeID
    // value is a map of methodIDs to method Features
    std::map<Symbol, std::map<Symbol, Feature> > method_table;

    std::map<int, CgenNodeP> tag_table; // find class_node from tag

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
    void code_attr_and_dispatch_table(Environmentp envp);
    void code_parentTab();
    void code_protObj();

// The following method emit code for object initializer
    void code_object_initializer(Environmentp envp);

// The following method emid code for class methods
    void code_class_methods(Environmentp envp);

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

///////////////////////////////////////////////////////////////////////
//
// Helper Functions
//
///////////////////////////////////////////////////////////////////////
bool isInt(Symbol typeID) {
    return strcmp(typeID->get_string(), "Int") == 0;
}

bool isString(Symbol typeID) {
    return strcmp(typeID->get_string(), "String") == 0;
}

bool isBool(Symbol typeID) {
    return strcmp(typeID->get_string(), "Bool") == 0;
}

bool is_SELF_TYPE(Symbol typeID) {
    return strcmp(typeID->get_string(), "SELF_TYPE") == 0;
}

bool equal(Symbol typeID1, Symbol typeID2) {
    return strcmp(typeID1->get_string(), typeID2->get_string()) == 0;
}

class Environment
{
private:
    // self object
    int so = -1;

    // an array of class typeID, ordered in tag
    std::vector<Symbol> class_typeIDs;

    // key is class tag, val is an array of attrs' objectIDs
    std::map<int, std::vector<Symbol> > tag_attrs;

    // a symbol table for all objectIDs in the current scope
    SymbolTable<Symbol, int> *env_objectIDs = new SymbolTable<Symbol, int>();

    // key is class tag, val is a map of methodID to method label
    // where a method label is the typeID.methodID in disptable
    // typeID is the implement/override class's typeID
    std::map<int, std::map<Symbol, Feature> > tag_methods;

public:
    int get_tag(Symbol typeID) {
        for (long unsigned i = 0; i < class_typeIDs.size(); ++i) {
            if (equal(typeID, class_typeIDs[i])) {
                return i;
            }
        }
        return -1;
    }

    void set_so (int tag) {
        this->so = tag;
    }

    int get_so() {
        return this->so;
    }

    void update_class_typeIDs(std::vector<Symbol> typeIDs) {
        this->class_typeIDs = typeIDs;
    }

    void update_tag_attrs(std::map<int, std::vector<Symbol> > input_tag_attrs) {
        this->tag_attrs = input_tag_attrs;
    }

    void update_tag_methods(std::map<int, std::map<Symbol, Feature> > input_tag_methods) {
        this->tag_methods = input_tag_methods;
    }

    // input 1: tag of the object
    // input 2: methodID to search
    // return: a label of typeID (of the methodID's implementation) dot methodID
    char *get_method_label(int tag, Symbol methodID) {
        Feature method = this->tag_methods[tag][methodID];
        char *implement_typeID = new char[strlen(method->get_implement_typeID()->get_string())];
        strcpy(implement_typeID, method->get_implement_typeID()->get_string());
        char *ret = strcat(implement_typeID, METHOD_SEP);
        ret = strcat(ret, methodID->get_string());
        return ret;
    }

    // print class_typeIDs for debug
    void print_class_typeIDs() {
        printf("============Environment class_typeIDs start============\n");
        printf("There are %lu classes\n", class_typeIDs.size());
        for (unsigned long i = 0; i < class_typeIDs.size(); ++i) {
            printf("Class %lu: %s\n", i, class_typeIDs[i]->get_string());
        }
        printf("=============Environment class_typeIDs end=============\n");
    }

    // print tag_methods for debug
    void print_tag_methods() {
        printf("============Environment tag_mathods start============\n");
        printf("There are %lu classes\n", this->tag_methods.size());
        for (unsigned long i = 0; i < this->tag_methods.size(); ++i) {
            printf("Class %lu: ", i);
            for (auto entry : this->tag_methods[i]) {
                printf("%s.%s ", entry.second->get_implement_typeID()->get_string(), \
                    entry.first->get_string());
            }
            printf("\n");
        }
        printf("=============Environment tag_mathods end=============\n");
    }

    // print tag_attrs for debug
    void print_tag_attrs() {
        printf("============Environment tag_attrs start============\n");
        printf("There are %lu classes\n", this->tag_attrs.size());
        for (unsigned long i = 0; i < this->tag_attrs.size(); ++i) {
            printf("Class %lu: ", i);
            for (unsigned long j = 0; j < this->tag_attrs[i].size(); ++j) {
                printf("%s ", this->tag_attrs[i][j]->get_string());
            }
            printf("\n");
        }
        printf("=============Environment tag_attrs end=============\n");
    }
};
