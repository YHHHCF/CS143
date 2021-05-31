
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
        arg,
        arg2,
        Bool,
        concat,
        cool_abort,
        copy,
        Int,
        in_int,
        in_string,
        IO,
        length,
        Main,
        main_meth,
        No_class,
        No_type,
        Object,
        out_int,
        out_string,
        prim_slot,
        self,
        SELF_TYPE,
        Str,
        str_field,
        substr,
        type_name,
        val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
    { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
    { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
    // spim wants comments to start with '#'
    os << "# start of generated code\n";

    initialize_constants();
    CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

    os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
    s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
        << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
    s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
        << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
    emit_partial_load_address(dest,s);
    b.code_ref(s);
    s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
    emit_partial_load_address(dest,s);
    str->code_ref(s);
    s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
    emit_partial_load_address(dest,s);
    i->code_ref(s);
    s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{ s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_attrtable_ref(Symbol sym, ostream& s)
{ s << sym << ATTRTAB_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
    emit_label_ref(l,s);
    s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
    s << BEQZ << source << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
    s << BEQ << src1 << " " << src2 << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
    s << BNE << src1 << " " << src2 << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
    s << BLEQ << src1 << " " << src2 << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
    s << BLT << src1 << " " << src2 << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
    s << BLT << src1 << " " << imm << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
    s << BGT << src1 << " " << imm << " ";
    emit_label_ref(label,s);
    s << endl;
}

static void emit_branch(int l, ostream& s)
{
    s << BRANCH;
    emit_label_ref(l,s);
    s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
    emit_store(reg,0,SP,str);
    emit_addiu(SP,SP,-WORD_SIZE,str);
}

static void emit_pop(char *reg, ostream& str)
{
    emit_addiu(SP,SP,WORD_SIZE,str);
    emit_load(reg,0,SP,str);
}

static void emit_popn(int n, ostream& str)
{
    emit_addiu(SP,SP,WORD_SIZE*n,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
    emit_push(ACC, s);
    emit_move(ACC, SP, s); // stack end
    emit_move(A1, ZERO, s); // allocate nothing
    s << JAL << gc_collect_names[cgen_Memmgr] << endl;
    emit_addiu(SP,SP,4,s);
    emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
    if (source != (char*)A1) emit_move(A1, source, s);
    s << JAL << "_gc_check" << endl;
}

// emit the code to print an int for debug
static void emit_int_debug(int x, ostream &s) {
    emit_push(ACC, s);
    emit_load_imm(ACC, x, s);
    emit_push(ACC, s);
    emit_jalr("IO.out_int", s);
    emit_popn(1, s);
    emit_pop(ACC, s);
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
    s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
    IntEntryP lensym = inttable.add_int(len);

    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);  s  << LABEL                                             // label
        << WORD << stringclasstag << endl                                 // tag
        << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
        << WORD;


    /***** Add dispatch information for class String ******/
        s << STRINGNAME << DISPTAB_SUFFIX << endl;              // dispatch table
        s << WORD;  lensym->code_ref(s);  s << endl;            // string length
    emit_string_constant(s,str);                                // ascii string
    s << ALIGN;                                                 // align to word
}

// return true for "_prim_slot", "SELF_TYPE", "_no_class"
bool string_special(StringEntry *str) {
    if (strcmp(str->get_string(), "_prim_slot") == 0) {
        return true;
    }
    if (strcmp(str->get_string(), "SELF_TYPE") == 0) {
        return true;
    }
    if (strcmp(str->get_string(), "_no_class") == 0) {
        return true;
    }
    return false;
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{
    for (List<StringEntry> *l = tbl; l; l = l->tl()) {
        // Do not care about special strings for codegen
        if (!string_special(l->hd())) {
            l->hd()->code_def(s,stringclasstag);
        }
    }
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
    s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);  s << LABEL                                // label
        << WORD << intclasstag << endl                      // class tag
        << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
        << WORD; 

    /***** Add dispatch information for class Int ******/
        s << INTNAME << DISPTAB_SUFFIX << endl;             // dispatch table
        s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
    for (List<IntEntry> *l = tbl; l; l = l->tl())
        l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
    s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);  s << LABEL                                  // label
        << WORD << boolclasstag << endl                       // class tag
        << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
        << WORD;

    /***** Add dispatch information for class Bool ******/
        s << BOOLNAME << DISPTAB_SUFFIX << endl;;             // dispatch table
        s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
    Symbol main    = idtable.lookup_string(MAINNAME);
    Symbol string  = idtable.lookup_string(STRINGNAME);
    Symbol integer = idtable.lookup_string(INTNAME);
    Symbol boolc   = idtable.lookup_string(BOOLNAME);

    str << "\t.data\n" << ALIGN;
    //
    // The following global names must be defined first.
    //
    str << GLOBAL << CLASSNAMETAB << endl;
    str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
    str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
    str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
    str << GLOBAL; falsebool.code_ref(str);  str << endl;
    str << GLOBAL; truebool.code_ref(str);   str << endl;
    str << GLOBAL << INTTAG << endl;
    str << GLOBAL << BOOLTAG << endl;
    str << GLOBAL << STRINGTAG << endl;
    
    // Keep track of _max_tag, or the number of classes, starting from 0
    str << GLOBAL << MAXTAG << endl;

    // Make the class_objTab, parentTab, and attrTabTab
    str << GLOBAL << CLASSOBJTAB << endl;
    str << GLOBAL << CLASSPARENTTAB << endl;
    str << GLOBAL << CLASSATTRTABTAB << endl;

    // Create a prototype object, init function, and attrTab for each class
    for (List<CgenNode> *l = nds; l; l=l->tl()) {
        Symbol curr_class = l->hd()->get_typeID();
        str << GLOBAL; emit_protobj_ref(curr_class, str); str << endl;
        str << GLOBAL; emit_init_ref(curr_class, str); str << endl;
        str << GLOBAL; emit_attrtable_ref(curr_class, str); str << endl;
    }

    //
    // We also need to know the tag of the Int, String, and Bool classes
    // during code generation.
    //
    str << INTTAG << LABEL
        << WORD << probe(Int)->get_tag() << endl;
    str << BOOLTAG << LABEL 
        << WORD << probe(Bool)->get_tag() << endl;
    str << STRINGTAG << LABEL 
        << WORD << probe(Str)->get_tag() << endl;    
}

//***************************************************
//
//  Emit code for the Class Name Table, the Class
//  Object Table, and the Max Tag
//
//***************************************************

void CgenClassTable::code_name_and_obj_table()
{
    
    // Class Name Table
    str << CLASSNAMETAB << LABEL;

    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        CgenNodeP curr_class_node = this->tag_table[tag];
        StringEntryP classNameSym = stringtable.lookup_string( \
            curr_class_node->get_typeID()->get_string());
        str << WORD; classNameSym->code_ref(str); str << endl;
    }
    
    // Class Object Table
    str << CLASSOBJTAB << LABEL;
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class = this->tag_table[tag]->get_typeID();
        str << WORD;
        emit_protobj_ref(curr_class, str);
        str << endl;
        str << WORD;
        emit_init_ref(curr_class, str);
        str << endl;
    }

    // Keep track of _max_tag, or the number of classes, starting from 0
    str << MAXTAG << LABEL;
    str << WORD << this->_max_tag << endl;
}


//***************************************************
//
//  Emit code for the Attribute and Dispatch Tables
//
//***************************************************

void CgenClassTable::code_attr_and_dispatch_table(Environmentp envp)
{
    // code for class_attrTabTab
    str << CLASSATTRTABTAB << LABEL;

    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class = this->tag_table[tag]->get_typeID();
        str << WORD;
        emit_attrtable_ref(curr_class, str);
        str << endl;
    }

    // Update attribute_table and method_table using BFS
    std::list<CgenNodeP> node_pool; // a pool for BFS
    node_pool.push_back(probe(Object)); // start from Object

    std::map<int, std::map<Symbol, Feature> > env_tag_methods; // for env
    std::map<int, std::vector<Symbol> > env_tag_attrs; // for env

    while (node_pool.size()) {
        int size = node_pool.size();
        for (int i = 0; i < size; ++i) {
            CgenNodeP curr_node = node_pool.front();
            node_pool.pop_front();
            
            // maps and orders for curr_node
            std::vector<Symbol> curr_attr_order;
            std::map<Symbol, Feature> curr_attr_map;
            std::vector<Symbol> curr_method_order;
            std::map<Symbol, Feature> curr_method_map;

            // Process features from Object to curr_node
            // TODO: optimize this to process only one parent node
            std::stack<CgenNodeP> node_stack; // a stack to store nodes
            CgenNodeP curr_parent = curr_node;

            // push all nodes to parent_stack
            while (curr_parent->get_tag() != probe(No_class)->get_tag()) {
                node_stack.push(curr_parent);
                curr_parent = curr_parent->get_parentnd();
            }

            // pop all parents from parent_stack
            while (!node_stack.empty()) {
                curr_parent = node_stack.top();
                node_stack.pop();
                Features pfeatures = curr_parent->get_features();
                for (int i = pfeatures->first(); pfeatures->more(i); i = pfeatures->next(i)) {
                    Feature curr_pfeature = pfeatures->nth(i);
                    if (curr_pfeature->instanceof("attr_class")) {
                        // attr cannot redefine, so add it directly
                        curr_attr_order.push_back(curr_pfeature->get_objectID());
                        curr_attr_map[curr_pfeature->get_objectID()] = curr_pfeature;
                    } else if (curr_pfeature->instanceof("method_class")) {
                        // record only when the methodID is first added
                        if (!curr_method_map.count(curr_pfeature->get_methodID())) {
                            curr_method_order.push_back(curr_pfeature->get_methodID());
                        }
                        // update method's implement-typeID to the least node which implements the method
                        curr_pfeature->set_implement_typeID(curr_parent->get_typeID());
                        curr_method_map[curr_pfeature->get_methodID()] = curr_pfeature;
                    }
                }
            }

            attribute_order[curr_node->get_typeID()] = curr_attr_order;
            attribute_table[curr_node->get_typeID()] = curr_attr_map;
            method_order[curr_node->get_typeID()] = curr_method_order;
            method_table[curr_node->get_typeID()] = curr_method_map;
            env_tag_attrs[curr_node->get_tag()] = curr_attr_order; // for env
            env_tag_methods[curr_node->get_tag()] = curr_method_map; // for env
            
            // add childrens to node_pool
            List<CgenNode> *children_nodes = curr_node->get_children();
            for (List<CgenNode> *l = children_nodes; l; l = l->tl()) {
                node_pool.push_back(l->hd());
            }
        }
    }
    envp->update_tag_attrs(env_tag_attrs); // update for environment
    envp->update_tag_methods(env_tag_methods); // update for environment

    // code for _attrTab
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class_typeID = this->tag_table[tag]->get_typeID();
        emit_attrtable_ref(curr_class_typeID, str);
        str << LABEL;

        auto curr_attr_map = this->attribute_table[curr_class_typeID];
        auto curr_attr_order = this->attribute_order[curr_class_typeID];

        for (long unsigned i = 0; i < curr_attr_order.size(); ++i) {
            Symbol attr_objectID = curr_attr_order[i];
            Symbol attr_typeID = curr_attr_map[attr_objectID]->get_typeID(); // attr declared typeID
            str << WORD << probe(attr_typeID)->get_tag() << endl;
        }
    }

    // code for _dispTab
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class_typeID = this->tag_table[tag]->get_typeID();
        emit_disptable_ref(curr_class_typeID, str);
        str << LABEL;

        auto curr_method_map = this->method_table[curr_class_typeID];
        auto curr_method_order = this->method_order[curr_class_typeID];

        for (long unsigned i = 0; i < curr_method_order.size(); ++i) {
            Symbol methodID = curr_method_order[i];
            Symbol implement_typeID = curr_method_map[methodID]->get_implement_typeID();
            str << WORD;
            emit_method_ref(implement_typeID, methodID, str);
            str << endl;
        }
    }

    if (cgen_debug) {
        print_attribute_table();
    }
}

//***************************************************
//
//  Emit code for the Prototype Objects
//
//***************************************************

void CgenClassTable::code_protObj()
{
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        str << WORD << GARBAGE_COLLECTOR_TAG << endl;                                    // Garbage Collector Tag
        Symbol curr_class_typeID = this->tag_table[tag]->get_typeID();
        emit_protobj_ref(curr_class_typeID, str);                                        // Prototype Object
        str << LABEL;
        str << WORD << probe(curr_class_typeID)->get_tag() << endl;                      // Class Tag
        int object_size = attribute_table[curr_class_typeID].size() + DEFAULT_OBJFIELDS;
        str << WORD << object_size << endl;                                              // Object Size
        str << WORD;
        emit_disptable_ref(curr_class_typeID, str);                                      // Dispatch Table
        str << endl;

        auto curr_attr_order = attribute_order[curr_class_typeID];                       // Attributes
        auto curr_attr_map = attribute_table[curr_class_typeID];
        for (long unsigned i = 0; i < curr_attr_order.size(); ++i) {
            Symbol attr_objID = curr_attr_order[i];
            Symbol attr_typeID = curr_attr_map[attr_objID]->get_typeID();
            if (isInt(attr_typeID)) {
                str << WORD; inttable.lookup_string("0")->code_ref(str); str << endl;
            } else if (isString(attr_typeID)) {
                str << WORD; stringtable.lookup_string("")->code_ref(str); str << endl;
            } else if (isBool(attr_typeID)) {
                str << WORD << DEFAULT_BOOL << endl;
            } else {
                str << WORD << EMPTYSLOT << endl;
            }
        }
    }
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
    // Heap
    str << GLOBAL << HEAP_START << endl
        << HEAP_START << LABEL 
        << WORD << 0 << endl
        << "\t.text" << endl
        << GLOBAL;
    emit_init_ref(idtable.add_string("Main"), str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("Int"),str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("String"),str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("Bool"),str);
    str << endl << GLOBAL;
    emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
    str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
    falsebool.code_def(str,boolclasstag);
    truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
    //
    // Generate GC choice constants (pointers to GC functions)
    //
    str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
    str << "_MemMgr_INITIALIZER:" << endl;
    str << WORD << gc_init_names[cgen_Memmgr] << endl;
    str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
    str << "_MemMgr_COLLECTOR:" << endl;
    str << WORD << gc_collect_names[cgen_Memmgr] << endl;
    str << GLOBAL << "_MemMgr_TEST" << endl;
    str << "_MemMgr_TEST:" << endl;
    str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
    //
    // Add constants that are required by the code generator.
    //
    stringtable.add_string("");
    inttable.add_string("0");

    if (cgen_debug) {
        printf("===========print String Table Start===========\n");
        stringtable.print();
        printf("============print String Table End============\n");
        printf("===========print Int Table Start===========\n");
        inttable.print();
        printf("============print Int Table End============\n");
        // printf("===========print ID Table Start===========\n");
        // idtable.print();
        // printf("============print ID Table End============\n");
    }

    stringtable.code_string_table(str, probe(Str)->get_tag());
    inttable.code_string_table(str, probe(Int)->get_tag());
    code_bools(probe(Bool)->get_tag());

    if (cgen_debug) {
        printf("===========print String Table Start===========\n");
        stringtable.print();
        printf("============print String Table End============\n");
        printf("===========print Int Table Start===========\n");
        inttable.print();
        printf("============print Int Table End============\n");
        // printf("===========print ID Table Start===========\n");
        // idtable.print();
        // printf("============print ID Table End============\n");
    }
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
    enterscope();
    if (cgen_debug) cout << "Building CgenClassTable" << endl;
    install_basic_classes();
    install_classes(classes);
    build_inheritance_tree();

    if (cgen_debug) {
        print_CgenClassTable();
        print_inheritance_graph();
    }

    code();
    exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
    //curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
    addid(No_class,
        new CgenNode(class_(No_class,No_class,nil_Features(),filename),
            Basic,this));
    addid(SELF_TYPE,
        new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
            Basic,this));
    addid(prim_slot,
        new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
            Basic,this));
    
    probe(No_class)->set_tag(-3);
    probe(SELF_TYPE)->set_tag(-2);
    probe(prim_slot)->set_tag(-1);
    this->_max_tag = -1;

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
    install_class(
    new CgenNode(
        class_(Object, 
        No_class,
        append_Features(
            append_Features(
            single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
            single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
            single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
        filename),
        Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
    install_class(
        new CgenNode(
        class_(IO, 
                Object,
                append_Features(
                append_Features(
                append_Features(
                single_Features(method(out_string, single_Formals(formal(arg, Str)),
                    SELF_TYPE, no_expr())),
                single_Features(method(out_int, single_Formals(formal(arg, Int)),
                    SELF_TYPE, no_expr()))),
                single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
                single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
        filename),	    
        Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
    install_class(
        new CgenNode(
        class_(Int, 
            Object,
                single_Features(attr(val, prim_slot, no_expr())),
            filename),
        Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
        new CgenNode(
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
        Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
    install_class(
        new CgenNode(
        class_(Str, 
                Object,
                append_Features(
                append_Features(
                append_Features(
                append_Features(
                single_Features(attr(val, Int, no_expr())),
                single_Features(attr(str_field, prim_slot, no_expr()))),
                single_Features(method(length, nil_Formals(), Int, no_expr()))),
                single_Features(method(concat, 
                    single_Formals(formal(arg, Str)),
                    Str, 
                    no_expr()))),
                single_Features(method(substr, 
                        append_Formals(single_Formals(formal(arg, Int)), 
                                single_Formals(formal(arg2, Int))),
                    Str, 
                    no_expr()))),
            filename),
            Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
    Symbol name = nd->get_typeID();

    if (probe(name)) {
        return;
    }

    // Set the tag for this node (class)
    this->_max_tag += 1;
    nd->set_tag(this->_max_tag);
    this->tag_table[this->_max_tag] = nd;

    // The class name is legal, so add it to the list of classes
    // and the symbol table.
    nds = new List<CgenNode>(nd,nds);
    addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
    for(int i = cs->first(); cs->more(i); i = cs->next(i))
        install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
    for(List<CgenNode> *l = nds; l; l = l->tl())
        set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
    CgenNode *parent_node = probe(nd->get_parent_typeID());
    nd->set_parentnd(parent_node);
    parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
    children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
    assert(parentnd == NULL);
    assert(p != NULL);
    parentnd = p;
}

//
// CgenClassTable::code_parentTab
// generate code for class_parentTab
//
void CgenClassTable::code_parentTab() {
    str << CLASSPARENTTAB << LABEL;

    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        CgenNodeP curr_class_node = this->tag_table[tag];
        str << WORD << curr_class_node->get_parentnd()->get_tag() << endl;
    }
}

//
// CgenClassTable::code_object_initializer
// generate code for object intializers (first pass)
//
void CgenClassTable::code_object_initializer(Environmentp envp) {
    std::vector<Symbol> typeIDs; // for env

    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class_typeID = this->tag_table[tag]->get_typeID();
        typeIDs.push_back(curr_class_typeID); // for env
        envp->set_so(tag); // set envp's so
        envp->enter_scope();
        envp->enter_class(); // set envp
        emit_init_ref(curr_class_typeID, str);
        str << LABEL;

        emit_push(FP, str); // store old fp
        emit_push(ACC, str); // store ACC (will be used when evaluate init_expr)
        emit_move(FP, SP, str); // fp points to one word below arguments
        emit_push(RA, str); // store ra

        // init all attrs in order
        std::map<Symbol, Feature> attr_map = this->attribute_table[curr_class_typeID];
        std::vector<Symbol> attr_order = this->attribute_order[curr_class_typeID];
        for (long unsigned i = 0; i < attr_order.size(); ++i) {
            Symbol attr_objID = attr_order[i];
            Feature attr_typeID = attr_map[attr_objID];
            Expression init_expr = attr_typeID->get_expression();
            if (!init_expr->instanceof("no_expr_class")) { // with init
                init_expr->code(envp, str); // code the init expression
                emit_store(ACC, DEFAULT_OBJFIELDS + i, SELF, str); // store
            } else { // default init
                emit_partial_load_address(ACC, str);
                emit_protobj_ref(curr_class_typeID, str); // get protObj ptr
                str << endl;
                emit_load(ACC, DEFAULT_OBJFIELDS + i, ACC, str); // load default init value from protObj
                emit_store(ACC, DEFAULT_OBJFIELDS + i, SELF, str); // store default value
            }
        }

        emit_pop(RA, str); // restore ra
        emit_pop(ACC, str); // restore ACC register
        emit_pop(FP, str); // restore fp
        
        emit_return(str);
        envp->exit_scope();
    }
    envp->update_class_typeIDs(typeIDs); // for env
}

//
// CgenClassTable::code_class_methods
// generate code for class methods (second pass)
//
void CgenClassTable::code_class_methods(Environmentp envp) {
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        Symbol curr_class_typeID = this->tag_table[tag]->get_typeID();
        envp->set_so(tag); // set so for environment
        envp->enter_scope();
        envp->enter_class(); // set envp variables

        // Do not override provided method code for basic classes
        if (!tag_table[tag]->basic()) {
            std::map<Symbol, Feature> class_methods = this->method_table[curr_class_typeID];

            for (auto method : class_methods) {
                Feature curr_method = method.second;

                // We only process methods defined in this class to prevent double counting
                if (equal(curr_method->get_implement_typeID(), curr_class_typeID)) {

                    if (cgen_debug) {
                        printf("code_class_methods: %s -> %s\n", curr_class_typeID->get_string(), \
                        curr_method->get_methodID()->get_string());
                    }

                    // enter the environment with formals
                    envp->enter_scope();
                    std::vector<Symbol> formal_objIDs;

                    for (int f = curr_method->get_formals()->first(); curr_method->get_formals()->more(f); \
                        f = curr_method->get_formals()->next(f)) {
                        Formal curr_formal = curr_method->get_formals()->nth(f);
                        formal_objIDs.push_back(curr_formal->get_objectID());
                    }
                    envp->update_formal_objectIDs(formal_objIDs);

                    envp->enter_method();

                    // Emit label for the Method
                    emit_method_ref(curr_class_typeID, curr_method->get_methodID(), str);
                    str << LABEL;

                    // Move the frame pointer to the stack pointer
                    emit_move(FP, SP, str);

                    // Store return address for later use
                    emit_push(RA, str);
                    
                    // Execute Code of Method
                    Expression method_expr = curr_method->get_expression();
                    method_expr->code(envp, str);
                
                    // Restore return address
                    emit_pop(RA, str);

                    // Pop n arguments
                    emit_popn(curr_method->get_formals()->len(), str);
                    
                    // Go back to return address
                    emit_return(str);

                    envp->exit_scope();
                }
            }
        }
        envp->exit_scope();
    }
}

void CgenClassTable::code()
{
    Environment env;

    if (cgen_debug) cout << "coding global data" << endl;
    code_global_data();

    if (cgen_debug) cout << "choosing gc" << endl;
    code_select_gc();

    if (cgen_debug) cout << "coding constants" << endl;
    code_constants();

    if (cgen_debug) cout << "coding class name and object tables" << endl;
    code_name_and_obj_table();

    if (cgen_debug) cout << "coding parent table" << endl;
    code_parentTab();

    if (cgen_debug) cout << "coding attirbute and dispatch tables" << endl;
    code_attr_and_dispatch_table(&env);

    if (cgen_debug) cout << "coding prototype objects" << endl;
    code_protObj();

    if (cgen_debug) cout << "coding global text" << endl;
    code_global_text();

    if (cgen_debug) cout << "coding object initializer" << endl;
    code_object_initializer(&env);

    if (cgen_debug) {
        env.print_tag_attrs();
        env.print_tag_methods();
        env.print_class_typeIDs();
        // env.test_env_objectIDs();
    }

    if (cgen_debug) cout << "coding class methods" << endl;
    code_class_methods(&env);
}

CgenNodeP CgenClassTable::root()
{
    return probe(Object);
}

// print methods for debugging

void CgenClassTable::print_CgenClassTable() {
    printf("========Print CgenClassTable Start=========\n");
    this->dump();
    printf("=========Print CgenClassTable End==========\n");
}

void CgenClassTable::print_inheritance_graph() {
    printf("========Print InheritanceGraph Start=========\n");
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        CgenNodeP curr_class_node = this->tag_table[tag];
        printf("%s (%d) : ", curr_class_node->get_typeID()->get_string(), curr_class_node->get_tag());

        List<CgenNode> *children = curr_class_node->get_children();
        for (List<CgenNode> *child = children; child; child = child->tl()) {
            printf("%s (%d); ", child->hd()->get_typeID()->get_string(), child->hd()->get_tag());
        }
        printf("\n");
    }
    printf("=========Print InheritanceGraph End==========\n");
}

void CgenClassTable::print_attribute_table() {
    printf("========Print AttributeTable Start=========\n");
    for (int tag = 0; tag <= this->_max_tag; ++tag) {
        CgenNodeP curr_class_node = this->tag_table[tag];
        Symbol curr_class_typeID = curr_class_node->get_typeID();
        printf("%s : ", curr_class_typeID->get_string());
        auto curr_attr_map = this->attribute_table[curr_class_typeID];
        auto curr_attr_order = this->attribute_order[curr_class_typeID];

        for (long unsigned i = 0; i < curr_attr_order.size(); ++i) {
            Symbol attr_objID = curr_attr_order[i];
            Symbol typeID = curr_attr_map[attr_objID]->get_typeID();
            printf("[%s : %s] ", (attr_objID)->get_string(), typeID->get_string());
        }
        printf("\n");
    }
    printf("=========Print AttributeTable End==========\n");
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
    class__class((const class__class &) *nd),
    parentnd(NULL),
    children(NULL),
    basic_status(bstatus)
{
    stringtable.add_string(name->get_string()); // Add class name to string table
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

// Helper functions for expr part
// push arguments for disptach / static dispatch
void push_arguments(Expressions args, Environmentp envp, ostream &s) {
    for (int i = args->len() - 1; i >= 0; --i) {
        args->nth(i)->code(envp, s);
        emit_push(ACC, s);
    }
}

// evaluate e0 and return e0_tag
int eval_expr(Environmentp envp, Expression expr, ostream &s) {
    int expr_tag;
    if (is_SELF_TYPE(expr->get_type())) {
        emit_move(ACC, SELF, s);
        expr_tag = envp->get_so();
    } else {
        expr->code(envp, s); // now v0 is in ACC
        // find v0 = X(a1 = la1, ... am = lam)
        expr_tag = envp->get_tag(expr->get_type()); // get tag of e0
    }
    return expr_tag;
}


// Expr part
void assign_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug assign_class\n");
    }
    Symbol curr_objectID = this->get_objectID();

    if (!envp->contains(curr_objectID)) {
        if (cgen_debug) {
            printf("Error: objectID not defined, should be handled by type checker.\n");
        }
    }

    // evaluate e1, the eval result is in ACC
    this->get_expression()->code(envp, s);

    if (envp->is_attr(curr_objectID)) {
        // attribute
        int offset = envp->get_attr_offset(curr_objectID) + DEFAULT_OBJFIELDS;
        emit_store(ACC, offset, SELF, s);
    } else if (envp->is_formal(curr_objectID)) {
        // formal
        int offset = envp->get_formal_offset(curr_objectID) + 1;
        emit_store(ACC, offset, FP, s);
    } else {
        // variable
        int offset = envp->get_var_offset(curr_objectID);
        emit_store(ACC, -offset, FP, s);
    }
}

void static_dispatch_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug static_dispatch_class\n");
    }

    // Step 1: prepare the stack
    // push fp on stack
    emit_push(FP, s);
    push_arguments(this->get_arg_expressions(), envp, s);

    // Step 2: evaluate e0
    // evaluate e0 -> v0
    Expression e0 = this->get_expression();
    int e0_tag = eval_expr(envp, e0, s);
    int static_tag = envp->get_tag(this->get_typeID());

    // Step 3: find method definition and jal
    char *method_label = envp->get_method_label(static_tag, this->get_methodID());
    envp->set_so(e0_tag);
    emit_jal(method_label, s);

    // Step 4: restore old fp
    emit_pop(FP, s);

    if (cgen_debug) {
        printf("debug static_dispatch_class end\n");
    }
}

void dispatch_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug dispatch_class\n");
    }
    // Step 1: prepare the stack
    // push fp on stack
    emit_push(FP, s);

    // evaluate arguments e1, e2, ..., en -> v1, v2, ..., vn
    // and push vn, vn-1, ..., v2, v1 to stack
    push_arguments(this->get_arg_expressions(), envp, s);
    
    // Step 2: evaluate e0
    // evaluate e0 -> v0
    Expression e0 = this->get_expression();
    int e0_tag = eval_expr(envp, e0, s);

    // Step 3: find method definition from the dispatch table of X and jal to it
    char *method_label = envp->get_method_label(e0_tag, this->get_methodID());
    envp->set_so(e0_tag);
    emit_jal(method_label, s);

    // Step 4: restore old fp
    emit_pop(FP, s);

    if (cgen_debug) {
        printf("debug dispatch_class end\n");
    }
}

void cond_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug cond_class\n");
    }
    
    Expression cond_expr = this->get_pred_expression();
    Expression then_expr = this->get_then_expression();
    Expression else_expr = this->get_else_expression();
    
    cond_expr->code(envp, s); // evaulated the conditional
    emit_load(ACC, DEFAULT_OBJFIELDS, ACC, s); // Put value of bool into ACC

    int label_1 = envp->get_label_idx(); // false
    int label_2 = envp->get_label_idx(); // continued code
    emit_beqz(ACC, label_1, s); // if false (equal to 0), branch to false code
    then_expr->code(envp, s); // true code
    emit_branch(label_2, s); // jump to continued, common code 
    
    // Emit two branches
    emit_label_def(label_1, s); // false code
    else_expr->code(envp, s);
    
    emit_label_def(label_2, s); // continued, common code
}

void loop_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug loop_class\n");
    }
    
    /*int loop_start = envp->get_label_idx();
    int loop_end = envp->get_label_idx();
        
    emit_label_def(loop_start, s);  // loop start
    Expression pred_expr = this->get_pred_expression();
    pred_expr->code(envp, s);               // pred result in ACC

    emit_load(ACC, DEFAULT_OBJFIELDS, ACC, s); // Put value 0 or 1 into ACC, depending if true or false
    emit_beq(ACC, ZERO, loop_end, s);          // branch to loop_end if ACC = 0

    Expression body_expr = this->get_body_expression();
    body_expr->code(envp, s);
    emit_branch(loop_start, s); // jump back to loop start to check pred once again

    emit_label_def(loop_end, s);  // loop end*/
}

void typcase_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug typcase_class\n");
    }
}

void block_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug block_class\n");
    }

    Expressions exprs = this->get_body_expressions();
    for (int i = exprs->first(); exprs->more(i); i = exprs->next(i)) {
        Expression curr_expr = exprs->nth(i);
        curr_expr->code(envp, s);
    }
}

void let_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug let_class\n");
    }
}

void plus_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug plus_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    s << JAL << "Object.copy" << endl;
    emit_load(T3, DEFAULT_OBJFIELDS, ACC, s); // Put value of expr2 from Object into T3

    emit_pop(T2, s); // expr1 popped to T2
    emit_load(T2, DEFAULT_OBJFIELDS, T2, s); // Put value of expr1 from Object into T2
    
    emit_add(T1, T2, T3, s); // final value in ACC
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // Plus the value into the copied object for return
}

void sub_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug sub_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    s << JAL << "Object.copy" << endl;
    emit_load(T3, DEFAULT_OBJFIELDS, ACC, s); // Put value of expr2 from Object into T3

    emit_pop(T2, s); // expr1 popped to T2
    emit_load(T2, DEFAULT_OBJFIELDS, T2, s); // Put value of expr1 from Object into T2
    
    emit_sub(T1, T2, T3, s); // final value in ACC
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // Plus the value into the copied object for return
}

void mul_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug mul_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    s << JAL << "Object.copy" << endl;
    emit_load(T3, DEFAULT_OBJFIELDS, ACC, s); // Put value of expr2 from Object into T3

    emit_pop(T2, s); // expr1 popped to T2
    emit_load(T2, DEFAULT_OBJFIELDS, T2, s); // Put value of expr1 from Object into T2
    
    emit_mul(T1, T2, T3, s); // final value in ACC
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // Plus the value into the copied object for return
}

void divide_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug divide_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    s << JAL << "Object.copy" << endl;
    emit_load(T3, DEFAULT_OBJFIELDS, ACC, s); // Put value of expr2 from Object into T3

    emit_pop(T2, s); // expr1 popped to T2
    emit_load(T2, DEFAULT_OBJFIELDS, T2, s); // Put value of expr1 from Object into T2
    
    emit_div(T1, T2, T3, s); // final value in ACC
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // Plus the value into the copied object for return
}

void neg_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug neg_class\n");
    }

    Expression expr = this->get_expression();
    expr->code(envp, s);
    s << JAL << "Object.copy" << endl;
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s); // Put value of expr from Object into T1
    emit_neg(T1, T1, s);
    
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // Plus the value into the copied object for return
}

void lt_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug lt_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    emit_move(T3, ACC, s); // expr2 moved to T3
    emit_pop(T2, s); // expr1 popped to T2
    
    int label_1 = envp->get_label_idx(); // true
    int label_2 = envp->get_label_idx(); // continued
    emit_blt(T2, T3, label_1, s); // if true, branch to true code
    emit_load_bool(ACC, BoolConst(false), s); // false code
    emit_branch(label_2, s); // jump to continued, common code 

    // Emit two branches
    emit_label_def(label_1, s); // true code
    emit_load_bool(ACC, BoolConst(true), s);
    
    emit_label_def(label_2, s); // continued, common code
}

void eq_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug eq_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC

    emit_move(T2, ACC, s); // expr2 moved from ACC to T2
    emit_pop(T1, s); // expr1 popped off stack to T1

    if (isInt(expr1->get_type()) || isBool(expr1->get_type()) || isString(expr1->get_type())) {
    
        emit_load_bool(ACC, BoolConst(true), s);
        emit_load_bool(A1, BoolConst(false), s);

        s << JAL << "equality_test" << endl;
    }
    else {
        int label_1 = envp->get_label_idx(); // true
        int label_2 = envp->get_label_idx(); // continued
        emit_beq(T1, T2, label_1, s); // if true, branch to true code
        emit_load_bool(ACC, BoolConst(false), s); // false code
        emit_branch(label_2, s); // jump to continued, common code 

        // Emit two branches
        emit_label_def(label_1, s); // true code
        emit_load_bool(ACC, BoolConst(true), s);
    
        emit_label_def(label_2, s); // continued, common code
    }
}

void leq_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug leq_class\n");
    }

    Expression expr1 = this->get_expression1();
    expr1->code(envp, s);
    emit_push(ACC, s);
    
    Expression expr2 = this->get_expression2();
    expr2->code(envp, s); // expr2 evaluated to ACC
    emit_move(T3, ACC, s); // expr2 moved to T3
    emit_pop(T2, s); // expr1 popped to T2
    
    int label_1 = envp->get_label_idx(); // true
    int label_2 = envp->get_label_idx(); // continued
    emit_bleq(T2, T3, label_1, s); // if true, branch to true code
    emit_load_bool(ACC, BoolConst(false), s); // false code
    emit_branch(label_2, s); // jump to continued, common code 

    // Emit two branches
    emit_label_def(label_1, s); // true code
    emit_load_bool(ACC, BoolConst(true), s);
    
    emit_label_def(label_2, s); // continued, common code
}

void comp_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug comp_class\n");
    }

    Expression expr = this->get_expression();
    expr->code(envp, s); // evaulated the conditional
    emit_load(ACC, DEFAULT_OBJFIELDS, ACC, s); // Put value of bool into ACC

    int label_1 = envp->get_label_idx(); // false
    int label_2 = envp->get_label_idx(); // continued code
    emit_beqz(ACC, label_1, s); // if false (equal to 0), branch to false code
    emit_load_bool(ACC, BoolConst(false), s); // true code, should output opposite
    emit_branch(label_2, s); // jump to continued, common code
    
    emit_label_def(label_1, s);
    emit_load_bool(ACC, BoolConst(true), s); // false code, should output opposite
    
    emit_label_def(label_2, s); // continued, common code
}

void int_const_class::code(Environmentp envp, ostream& s)  
{
    if (cgen_debug) {
        printf("debug int_const_class: %s\n", token->get_string());
    }
    //
    // Need to be sure we have an IntEntry *, not an arbitrary Symbol
    //
    emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(Environmentp envp, ostream& s)
{
    if (cgen_debug) {
        printf("debug string_const_class: %s\n", token->get_string());
    }
    emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(Environmentp envp, ostream& s)
{
    if (cgen_debug) {
        printf("debug bool_const_class: %d\n", val);
    }
    emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug new__class: %s\n", this->get_typeID()->get_string());
    }
    // Get the tag of type to create
    Symbol T0 = this->get_typeID();
    if (is_SELF_TYPE(this->get_typeID())) {
        T0 = envp->get_typeID(envp->get_so());
    }

    // load address of protObj of the type to ACC
    emit_partial_load_address(ACC, s);
    emit_protobj_ref(T0, s);
    s << endl;

    // Call Object.copy to get a copy of protObj
    emit_jal("Object.copy", s);

    // Call init to initialize the object
    emit_partial_load_address(T1, s);
    emit_init_ref(T0, s);
    s << endl;
    emit_jalr(T1, s);
}

void isvoid_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug isvoid_class\n");
    }

    Expression expr = this->get_expression();
    expr->code(envp, s); // Expression in ACC

    emit_load_imm(T1, EMPTYSLOT, s); // 0 (void) in T1
 
    int label_1 = envp->get_label_idx(); // true
    int label_2 = envp->get_label_idx(); // continued
    emit_beq(ACC, T1, label_1, s); // if true, branch to true code
    emit_load_bool(ACC, BoolConst(false), s); // false code
    emit_branch(label_2, s); // jump to continued, common code

    // Emit two branches
    emit_label_def(label_1, s); // true code
    emit_load_bool(ACC, BoolConst(true), s);
    
    emit_label_def(label_2, s); // continued, common code
}

void no_expr_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug no_expr_class\n");
    }
    emit_load_imm(ACC, EMPTYSLOT, s);
}

void object_class::code(Environmentp envp, ostream &s) {
    if (cgen_debug) {
        printf("debug object_class: %s\n", this->get_objectID()->get_string());
    }
    Symbol curr_objectID = this->get_objectID();

    if (!envp->contains(curr_objectID)) {
        if (cgen_debug) {
            printf("Error: objectID not defined, should be handled by type checker.\n");
        }
    }

    if (is_SELF_TYPE(this->get_type())) {
        emit_load(ACC, 0, SELF, s);
        return;
    }

    if (envp->is_attr(curr_objectID)) {
        // attribute
        int offset = envp->get_attr_offset(curr_objectID) + DEFAULT_OBJFIELDS;
        emit_load(ACC, offset, SELF, s);
    } else if (envp->is_formal(curr_objectID)) {
        // formal
        int offset = envp->get_formal_offset(curr_objectID) + 1;
        emit_load(ACC, offset, FP, s); // formals / arguments are above fp
    } else {
        // variable
        int offset = envp->get_var_offset(curr_objectID);
        emit_load(ACC, -offset, FP, s); // variables are stored below fp
    }
}
// bottom
