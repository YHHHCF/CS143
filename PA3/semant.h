#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include <map>
#include <set>
#include <list>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

extern int semant_debug;

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.

class ClassTable {
private:
  int semant_errors;

  // inheritance_map stores inheritance information
  // key is a typeID, value is its children's typeIDs
  std::map<Symbol, std::set<Symbol> > inheritance_map;

  // class_map is used to get the Symbol's Class_
  // key is a typeID, value is a Class_
  std::map<Symbol, Class_> class_map;
  
  // Used to manage current scope for naming_and_scoping_DFS; maps an attribute's objectID to typeID
  SymbolTable<Symbol, Symbol> *curr_scope_vars = new SymbolTable<Symbol, Symbol>();
    
  // Keeps track of the scope for each class
  // key is a typeID, value is a set of methodIDs
  std::map<Symbol, std::set<Symbol> > method_table;

  void install_basic_classes();
  ostream& error_stream;

public:
  ClassTable(Classes);

  void add_class(Class_ c) {
    Symbol parent_typeID = c->get_parent_typeID();
    Symbol typeID = c->get_typeID();

    // Error 1: Cannot inherit from Bool, Int, String
    if (parent_typeID->equal_string("Bool", 4)) {
      semant_error(c) << "Class " << typeID << " cannot inherit class Bool.\n";
      ++semant_errors;
      return;
    }
    if (parent_typeID->equal_string("Int", 3)) {
      semant_error(c) << "Class " << typeID << " cannot inherit class Int.\n";
      ++semant_errors;
      return;
    }
    if (parent_typeID->equal_string("String", 6)) {
      semant_error(c) << "Class " << typeID << " cannot inherit class String.\n";
      ++semant_errors;
      return;
    }

    // Error 2: Cannot add already defined class
    if (this->class_map.count(typeID)) {
      semant_error(c) << "Class " << typeID << " was previously defined.\n";
      ++semant_errors;
      return;
    }

    // Add the class to idtable
    idtable.add_string(typeID->get_string());

    // Add the class to class_map
    this->class_map[typeID] = c;

    // Add the class and its parent to inheritance map
    if (this->inheritance_map.count(parent_typeID)) {
      this->inheritance_map[parent_typeID].insert(typeID);
    } else {
      std::set<Symbol> set;
      set.insert(typeID);
      this->inheritance_map[parent_typeID] = set;
    }
    return;
  }

  // Check acyclic and single inheritance
  // return true if the inheritance_map is good
  void check_inheritance_map() {
    std::list<Symbol> typeID_pool; // a temp list for BFS
    std::set<Symbol> typeID_traversed; // record the nodes that has been traversed

    typeID_traversed.insert(idtable.lookup_string("Object"));
    typeID_pool.push_back(idtable.lookup_string("Object"));

    while(typeID_pool.size()) {
      int size = typeID_pool.size();
      for (int i = 0; i < size; ++i) {
        Symbol typeID = typeID_pool.front();
        typeID_pool.pop_front();
        auto children_typeIDs = this->inheritance_map[typeID];

        if (semant_debug) {
          printf("Traversing inheritance_map: %s with %lu childrens\n", typeID->get_string(), children_typeIDs.size());
        }

        for (auto iter = children_typeIDs.begin(); iter != children_typeIDs.end(); ++iter) {
          typeID_traversed.insert(*iter);
          typeID_pool.push_back(*iter);
        }
      }
    }

    // Error 3: Inherit from undefined class
    // If any class's parent is not defined then we can find this by checking
    // The parents from inheritance_map, whether all of them are in class_map
    // If a class is a parent in inheritance_map but not in class_map
    // Then it has Error 3, the only exception is Object which has not parent
    std::set<Symbol> error3_set; // Record in Error 3 and used for Error 4

    for (auto typeID_typeIDs : this->inheritance_map) {
      Symbol parent_typeID = typeID_typeIDs.first;
      if (!this->class_map.count(parent_typeID)) {
        auto children_typeIDs = typeID_typeIDs.second;
        for (auto iter = children_typeIDs.begin(); iter != children_typeIDs.end(); ++iter) {
          Symbol typeID = *iter;
          if (!typeID->equal_string("Object", 6)) {
            error3_set.insert(typeID);
            Class_ c = this->class_map[typeID];
            semant_error(c) << "Class " << typeID << " inherits from an undefined class " << parent_typeID << ".\n";
            ++semant_errors;
          }
        }
      }
    }

    // Error 4: Cyclic inheritance
    // If any class in class_map is not in the traversed list above
    // Then it is not reachable from Object
    // Then it must be either Error 3 or Error 4 (if not in error3_set)
    // See test/inherit_bad_cycle.cl for details
    for (auto typeID_class : this->class_map) {
      Symbol typeID = typeID_class.first;
      if (!typeID_traversed.count(typeID) && !error3_set.count(typeID)) {
        Class_ c = typeID_class.second;
        semant_error(c) << "Class " << typeID << ", or an ancestor of " << typeID << ", is involved in an inheritance cycle.\n";
        ++semant_errors;
      }
    }
  }

    // Checks that each variable is named properly and accessed in its own scope
    void check_naming_and_scoping() {
        naming_and_scoping_DFS(class_map[idtable.lookup_string("Object")]);
    }

    // DFS used to traverse through the inheritance tree for naming and scoping section
    void naming_and_scoping_DFS(Class_ c) {
        // Enter a new scope for each new class
        curr_scope_vars->enterscope();
        
        // Checks for conflicts for the naming of attributes and methods
        check_naming(c);

        // Checks for conflicts in the expressions/body of methods and attributes
        check_scoping(c);

        // Enter the DFS to check child classes
        auto children_typeIDs = this->inheritance_map[c->get_typeID()];
        for (auto iter = children_typeIDs.begin(); iter != children_typeIDs.end(); ++iter) {
            naming_and_scoping_DFS(this->class_map[*iter]);
        }

        // Done processing the current scope; exit the scope
        curr_scope_vars->exitscope();
    }

    // Checks naming conflicts and adds them to the current scope
    void check_naming(Class_ c) {
        // Enter the scope
        Features features = c->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i)) {
            Feature curr_feature = features->nth(i);
            if (curr_feature->instanceof("attr_class")) {
                /* ERROR 1: Duplicate Definitions of Attributes*/
                if (curr_scope_vars->probe(curr_feature->get_objectID()) != NULL) {
                    semant_error(c) << "Attribute " << curr_feature->get_objectID() << " is multiply defined in class.\n";
                    ++semant_errors;
                }
                /* ERROR 2: Overriding Attributes */
                else if (curr_scope_vars->lookup(curr_feature->get_objectID()) != NULL) {
                    semant_error(c) << "Attribute " << curr_feature->get_objectID() << " is an attribute of an inherited class.\n";
                    ++semant_errors;
                }
                else {
                    idtable.lookup_string(curr_feature->get_typeID()->get_string());
                    curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(curr_feature->get_typeID()));
                }
            }
            else if (curr_feature->instanceof("method_class")) {
                /* ERROR 3: Duplicate Definitions of Methods in a Class */
                if (method_table[c->get_typeID()].count(curr_feature->get_methodID())) {
                    semant_error(c) << "Method " << curr_feature->get_methodID() << " is multiply defined in class.\n";
                    ++semant_errors;
                }
                else {
                    method_table[c->get_typeID()].insert(curr_feature->get_methodID());
                }
            }
        }
    }

    // Checks scoping conflicts and adds them to the current scope; also processes expressions
    void check_scoping(Class_ c) {
        Features features = c->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i)) {
            Feature curr_feature = features->nth(i);

            // Handles Multiply defined Formals (parameters) in Method Declarations
            if (curr_feature->instanceof("method_class")) {
                Formals formals = curr_feature->get_formals();
                curr_scope_vars->enterscope();
                for (int j = formals->first(); formals->more(j); j = formals->next(j)) {
                    Formal curr_formal = formals->nth(j);
                    /* ERROR 4: Duplicate Definitions of Formals in a Method */
                    if (curr_scope_vars->probe(curr_formal->get_objectID()) != NULL) {
                        semant_error(c) << "Formal Parameter " << curr_formal->get_objectID() << "is multiply defined.\n";
                        ++semant_errors;
                    }
                    curr_scope_vars->addid(curr_formal->get_objectID(), new Symbol(curr_formal->get_typeID()));
                }
                curr_scope_vars->exitscope();
            }

            Expression expr = curr_feature->get_expression();
            check_expression(c, expr);
        }
    }

    void check_expression(Class_ c, Expression expr) {
        if (expr->instanceof("let_class")) {
            curr_scope_vars->enterscope();
            // Any Let Expression checking done here
            curr_scope_vars->addid(expr->get_objectID(), new Symbol(expr->get_typeID()));
            check_expression(c, expr->get_init_expression()); // TODO: verify type of this
            check_expression(c, expr->get_body_expression());
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("typcase_class")) {
            curr_scope_vars->enterscope();
            Cases c = expr->get_cases();
            for (int i = c->first(); c->more(i); i = c->next(i)) {
                Case curr_case = c->nth(i);
                curr_scope_vars->enterscope();
                curr_scope_vars->addid(curr_case->get_objectID(), new Symbol(curr_case->get_typeID()));
                curr_scope_vars->exitscope();
            }
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("dispatch_class")) {
            // dispatch
            curr_scope_vars->enterscope();

            // Step 1: check whether each argument is a legal expression
            Expressions arguments = expr->get_arg_expressions();
            for (int i = arguments->first(); arguments->more(i); i = arguments->next(i)) {
              Expression argument = arguments->nth(i);
              check_expression(c, argument);
            }

            // dispatch on self object, check if expr is a self object
            if (expr->get_expression()->instanceof("object_class") && expr->get_expression()->get_objectID()->equal_string("self", 4)) {
              // Step 2: check method in current class
              if (method_table[c->get_typeID()].count(expr->get_methodID())) {
                // TODO: sth else in type checking?
              } else {
                semant_error(c) << "Dispatch to undefined method " << expr->get_methodID() << ".\n";
                ++semant_errors;
              }

            }
            // dispatch not on self object
            else {
              // Step 2: check the dispatch expression
              check_expression(c, expr->get_expression());

              // Step 3: TODO: check method in type part, we don't know the type of the dispatch expression now
              
            }
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("static_dispatch_class")) {
            // static dispatch

            // Step 1: check whether each argument is a legal expression
            Expressions arguments = expr->get_arg_expressions();
            for (int i = arguments->first(); arguments->more(i); i = arguments->next(i)) {
              Expression argument = arguments->nth(i);
              check_expression(c, argument);
            }

            // Step 2: check the dispatch expression
            check_expression(c, expr->get_expression());

            // Step 3: TODO: check static dispatch typeID

            // Step 4: TODO: check method in type part
            // If the static dispatch has that method
            if (method_table[expr->get_typeID()].count(expr->get_methodID())) {
              // TODO: sth else in type checking?
            } else {
              semant_error(c) << "Static dispatch to undefined method " << expr->get_methodID() << ".\n";
              ++semant_errors;
            }
        }
    }

  // Print the inheritance graph for debug
  void print_inheritance_map() {
    printf("========Print inheritance_map Start=========\n");
    for (auto item : this->inheritance_map) {
      printf("%s <- ", item.first->get_string());
      auto set = item.second;
      for (auto iter = set.begin(); iter != set.end(); ++iter) {
        printf("[%s] ", (*iter)->get_string());
      }
      printf("\n");
    }
    printf("=========Print inheritance_map End==========\n");
    return;
  }

  // Print the inheritance graph for debug
  void print_class_map() {
    printf("========Print class_map Start=========\n");
    for (auto item : this->class_map) {
      printf("%s\n", item.first->get_string());
    }
    printf("=========Print class_map End==========\n");
    return;
  }

  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
};


#endif

