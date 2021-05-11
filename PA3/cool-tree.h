#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "cool-tree.handcode.h"


// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

#ifdef Program_EXTRAS
   Program_EXTRAS
#endif
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class Class__class : public tree_node {
public:
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;
   virtual Symbol get_typeID() { return nullptr; } // Overriden in class__class
   virtual Symbol get_parent_typeID() { return nullptr; } // Overriden in class__class
   virtual Features get_features() { return nullptr; } // Overriden in class__class

#ifdef Class__EXTRAS
   Class__EXTRAS
#endif
};


// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;
   virtual bool instanceof(char* type) = 0; // Implemented in method_class and attr_class
   virtual Symbol get_methodID() { return nullptr; } // Overriden in method_class
   virtual Formals get_formals() { return nullptr; } // Overriden in method_class
   virtual Symbol get_typeID() { return nullptr; } // Overriden in method_class and attr_class
   virtual Expression get_expression() { return nullptr; } // Overriden in method_class and attr_class
   virtual Symbol get_objectID() { return nullptr; } // Overriden in attr_class

#ifdef Feature_EXTRAS
   Feature_EXTRAS
#endif
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;
   virtual Symbol get_objectID() { return nullptr; } // Overriden in formal_class
   virtual Symbol get_typeID() { return nullptr; } // Overriden in formal_class
   
#ifdef Formal_EXTRAS
   Formal_EXTRAS
#endif
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;
   virtual bool instanceof(char* type) = 0;
   virtual Symbol get_typeID() { return nullptr; }
   virtual Expression get_expression() { return nullptr; }
   virtual Symbol get_methodID() { return nullptr; }
   virtual Expressions get_arg_expressions() { return nullptr; }
   virtual Expression get_pred_expression() { return nullptr; }
   virtual Expression get_then_expression() { return nullptr; }
   virtual Expression get_else_expression() { return nullptr; }
   virtual Expression get_body_expression() { return nullptr; }
   virtual Cases get_cases() { return nullptr; }
   virtual Expressions get_body_expressions() { return nullptr; }
   virtual Symbol get_objectID() { return nullptr; }
   virtual Expression get_init_expression() { return nullptr; }
   virtual Expression get_expression1() { return nullptr; }
   virtual Expression get_expression2() { return nullptr; }
   virtual Symbol get_int_constant() { return nullptr; }
   virtual Boolean get_bool_constant() { return -1; } // use the value here is illegal, so put illegal value here
   virtual Symbol get_string_constant() { return nullptr; }

#ifdef Expression_EXTRAS
   Expression_EXTRAS
#endif
};


// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;

   virtual Symbol get_objectID() { return nullptr; } // Overriden in branch_class
   virtual Symbol get_typeID()  { return nullptr; } // Overriden in branch_class
   virtual Expression get_expression()  { return nullptr; } // Overriden in branch_class

#ifdef Case_EXTRAS
   Case_EXTRAS
#endif
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


// define list phlyum - Features
typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

#ifdef Program_SHARED_EXTRAS
   Program_SHARED_EXTRAS
#endif
#ifdef program_EXTRAS
   program_EXTRAS
#endif
};


// define constructor - class_
class class__class : public Class__class {
protected:
   Symbol name; // typeID
   Symbol parent; // parent_typeID
   Features features;
   Symbol filename;
public:
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) {
      name = a1;
      parent = a2;
      features = a3;
      filename = a4;
   }
   Class_ copy_Class_();

   Symbol get_typeID() {
      return this->name;
   }

   Symbol get_parent_typeID() {
      return this->parent;
   }

   Features get_features() {
      return this->features;
   }

   void dump(ostream& stream, int n);

#ifdef Class__SHARED_EXTRAS
   Class__SHARED_EXTRAS
#endif
#ifdef class__EXTRAS
   class__EXTRAS
#endif
};


// define constructor - method
class method_class : public Feature_class {
protected:
   char* type = "method_class"; // the type of this class
   Symbol name; // methodID
   Formals formals;
   Symbol return_type; // typeID
   Expression expr;
public:
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

   Symbol get_methodID() {
       return name;
   }

   Formals get_formals() {
      return formals;
   }
    
   Symbol get_typeID() {
      return return_type;
   }

   Expression get_expression() {
      return expr;
   }
    
   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef method_EXTRAS
   method_EXTRAS
#endif
};


// define constructor - attr
class attr_class : public Feature_class {
protected:
   char* type = "attr_class";
   Symbol name; // objectID
   Symbol type_decl; // typeID
   Expression init; // expression
public:
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return name;
   }

   Symbol get_typeID() {
      return type_decl;
   }

   Expression get_expression() {
      return init;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef attr_EXTRAS
   attr_EXTRAS
#endif
};


// define constructor - formal
class formal_class : public Formal_class {
protected:
   Symbol name; // objectID
   Symbol type_decl; // typeID
public:
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return name;
   }

   Symbol get_typeID() {
      return type_decl;
   }
   
    
#ifdef Formal_SHARED_EXTRAS
   Formal_SHARED_EXTRAS
#endif
#ifdef formal_EXTRAS
   formal_EXTRAS
#endif
};


// define constructor - branch
class branch_class : public Case_class {
protected:
   Symbol name; // objectID
   Symbol type_decl; // typeID
   Expression expr; // expression
public:
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return name;
   }

   Symbol get_typeID() {
     return type_decl;
   }

   Expression get_expression() {
      return expr;
   }

#ifdef Case_SHARED_EXTRAS
   Case_SHARED_EXTRAS
#endif
#ifdef branch_EXTRAS
   branch_EXTRAS
#endif
};


// define constructor - assign
class assign_class : public Expression_class {
protected:
   char* type = "assign_class";
   Symbol name; // objectID
   Expression expr; // expression
public:
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return name;
   }

   Expression get_expression() {
      return expr;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef assign_EXTRAS
   assign_EXTRAS
#endif
};


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
protected:
   char* type = "static_dispatch_class";
   Expression expr; // expression
   Symbol type_name; // typeID
   Symbol name; // methodID
   Expressions actual; // arg_expressions
public:
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
   }
   
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return expr;
   }

   Symbol get_typeID() {
      return type_name;
   }

   Symbol get_methodID() {
      return name;
   }
   
   Expressions get_arg_expressions() {
      return actual;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }
    
#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef static_dispatch_EXTRAS
   static_dispatch_EXTRAS
#endif
};


// define constructor - dispatch
class dispatch_class : public Expression_class {
protected:
   char* type = "dispatch_class";
   Expression expr; // expression
   Symbol name; // methodID
   Expressions actual; // arg_expressions
public:
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return expr;
   }

   Symbol get_methodID() {
      return name;
   }

   Expressions get_arg_expressions() {
      return actual;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef dispatch_EXTRAS
   dispatch_EXTRAS
#endif
};


// define constructor - cond
class cond_class : public Expression_class {
protected:
   char* type = "cond_class";
   Expression pred; // pred_expression
   Expression then_exp; // then_expression
   Expression else_exp; // else_expression
public:
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

   Expression get_pred_expression() {
      return pred;
   }

   Expression get_then_expression() {
      return then_exp;
   }

   Expression get_else_expression() {
      return else_exp;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef cond_EXTRAS
   cond_EXTRAS
#endif
};


// define constructor - loop
class loop_class : public Expression_class {
protected:
   char* type = "loop_class";
   Expression pred; // pred_expression
   Expression body; // body_expression
public:
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_pred_expression() {
      return pred;
   }

   Expression get_body_expression() {
      return body;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef loop_EXTRAS
   loop_EXTRAS
#endif
};


// define constructor - typcase
class typcase_class : public Expression_class {
protected:
   char* type = "typcase_class";
   Expression expr; // expression
   Cases cases;
public:
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return expr;
   }

   Cases get_cases() {
      return cases;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef typcase_EXTRAS
   typcase_EXTRAS
#endif
};


// define constructor - block
class block_class : public Expression_class {
protected:
   char* type = "block_class";
   Expressions body; // body_expressions
public:
   block_class(Expressions a1) {
      body = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expressions get_body_expressions() {
      return body;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef block_EXTRAS
   block_EXTRAS
#endif
};


// define constructor - let
class let_class : public Expression_class {
protected:
   char* type = "let_class";
   Symbol identifier; // objectID
   Symbol type_decl; // typeID
   Expression init; // init_expression
   Expression body; // body_expression
public:
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return identifier;
   }
    
   Symbol get_typeID() {
      return type_decl;
   }

   Expression get_init_expression() {
      return init;
   }

   Expression get_body_expression() {
      return body;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef let_EXTRAS
   let_EXTRAS
#endif
};


// define constructor - plus
class plus_class : public Expression_class {
protected:
   char* type = "plus_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   plus_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef plus_EXTRAS
   plus_EXTRAS
#endif
};


// define constructor - sub
class sub_class : public Expression_class {
protected:
   char* type = "sub_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   sub_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef sub_EXTRAS
   sub_EXTRAS
#endif
};


// define constructor - mul
class mul_class : public Expression_class {
protected:
   char* type = "mul_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   mul_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef mul_EXTRAS
   mul_EXTRAS
#endif
};


// define constructor - divide
class divide_class : public Expression_class {
protected:
   char* type = "divide_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   divide_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef divide_EXTRAS
   divide_EXTRAS
#endif
};


// define constructor - neg
class neg_class : public Expression_class {
protected:
   char* type = "neg_class";
   Expression e1; // expression
public:
   neg_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return e1;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef neg_EXTRAS
   neg_EXTRAS
#endif
};


// define constructor - lt
class lt_class : public Expression_class {
protected:
   char* type = "lt_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   lt_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef lt_EXTRAS
   lt_EXTRAS
#endif
};


// define constructor - eq
class eq_class : public Expression_class {
protected:
   char* type = "eq_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef eq_EXTRAS
   eq_EXTRAS
#endif
};


// define constructor - leq
class leq_class : public Expression_class {
protected:
   char* type = "leq_class";
   Expression e1; // expression1
   Expression e2; // expression2
public:
   leq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression1() {
      return e1;
   }

   Expression get_expression2() {
      return e2;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef leq_EXTRAS
   leq_EXTRAS
#endif
};


// define constructor - comp
class comp_class : public Expression_class {
protected:
   char* type = "comp_class";
   Expression e1; // expression
public:
   comp_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return e1;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef comp_EXTRAS
   comp_EXTRAS
#endif
};

// define constructor - int_const
class int_const_class : public Expression_class {
protected:
   char* type = "int_const_class";
   Symbol token; // int_constant
public:
   int_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_int_constant() {
      return token;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef int_const_EXTRAS
   int_const_EXTRAS
#endif
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
protected:
   char* type = "bool_const_class";
   Boolean val; // bool_constant
public:
   bool_const_class(Boolean a1) {
      val = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Boolean get_bool_constant() {
      return val;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef bool_const_EXTRAS
   bool_const_EXTRAS
#endif
};


// define constructor - string_const
class string_const_class : public Expression_class {
protected:
   char* type = "string_const_class";
   Symbol token; // string_constant
public:
   string_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_string_constant() {
      return token;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef string_const_EXTRAS
   string_const_EXTRAS
#endif
};


// define constructor - new_
class new__class : public Expression_class {
protected:
   char* type = "new__class";
   Symbol type_name; // typeID
public:
   new__class(Symbol a1) {
      type_name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_typeID() {
      return type_name;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef new__EXTRAS
   new__EXTRAS
#endif
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
protected:
   char* type = "isvoid_class";
   Expression e1; // expression
public:
   isvoid_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Expression get_expression() {
      return e1;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef isvoid_EXTRAS
   isvoid_EXTRAS
#endif
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
protected:
   char* type = "no_expr_class";
public:
   no_expr_class() {
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef no_expr_EXTRAS
   no_expr_EXTRAS
#endif
};


// define constructor - object
class object_class : public Expression_class {
protected:
   char* type = "object_class";
   Symbol name; // objectID
public:
   object_class(Symbol a1) {
      name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Symbol get_objectID() {
      return name;
   }

   bool instanceof(char* type) {
      return strcmp(this->type, type) == 0;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef object_EXTRAS
   object_EXTRAS
#endif
};


// define the prototypes of the interface
Classes nil_Classes();
Classes single_Classes(Class_);
Classes append_Classes(Classes, Classes);
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
Class_ class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
