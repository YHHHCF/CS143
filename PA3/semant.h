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
  // key is class symbols, value is its children's symbols
  std::map<Symbol, std::set<Symbol> > inheritance_map;

  // class_map is used to get the Symbol's Class_
  std::map<Symbol, Class_> class_map;
  
  // Used to manage current scope for naming_and_scoping_DFS; maps attributes to types
  SymbolTable<Symbol, Symbol> *curr_scope_vars = new SymbolTable<Symbol, Symbol>();
    
  // Keeps track of the scope for each class
  std::map<Symbol, std::set<Symbol> > method_table;

  void install_basic_classes();
  ostream& error_stream;

public:
  ClassTable(Classes);

  void add_class(Class_ c) {
    Symbol parent = c->get_parent();
    Symbol name = c->get_name();

    // Error 1: Cannot inherit from Bool, Int, String
    if (parent->equal_string("Bool", 4)) {
      semant_error(c) << "Class " << name << " cannot inherit class Bool.\n";
      ++semant_errors;
      return;
    }
    if (parent->equal_string("Int", 3)) {
      semant_error(c) << "Class " << name << " cannot inherit class Int.\n";
      ++semant_errors;
      return;
    }
    if (parent->equal_string("String", 6)) {
      semant_error(c) << "Class " << name << " cannot inherit class String.\n";
      ++semant_errors;
      return;
    }

    // Error 2: Cannot add already defined class
    if (this->class_map.count(name)) {
      semant_error(c) << "Class " << name << " was previously defined.\n";
      ++semant_errors;
      return;
    }

    // Add the class to class_map
    this->class_map[name] = c;

    // Add the class and its parent to inheritance map
    if (this->inheritance_map.count(parent)) {
      this->inheritance_map[parent].insert(name);
    } else {
      std::set<Symbol> set;
      set.insert(name);
      this->inheritance_map[parent] = set;
    }
    return;
  }

  // Check acyclic and single inheritance
  // return true if the inheritance_map is good
  bool check_inheritance_map() {
    bool correctness = true;

    std::list<Symbol> pool; // a temp list for BFS
    std::set<Symbol> traversed; // record the nodes that has been traversed

    traversed.insert(idtable.lookup_string("Object"));
    pool.push_back(idtable.lookup_string("Object"));

    while(pool.size()) {
      int size = pool.size();
      for (int i = 0; i < size; ++i) {
        Symbol curr = pool.front();
        pool.pop_front();
        auto set = this->inheritance_map[curr];

        if (semant_debug) {
          printf("Traversing inheritance_map: %s with %lu childrens\n", curr->get_string(), set.size());
        }

        for (auto iter = set.begin(); iter != set.end(); ++iter) {
          traversed.insert(*iter);
          pool.push_back(*iter);
        }
      }
    }

    // Error 3: Inherit from undefined class
    // If any class's parent is not defined then we can find this by checking
    // The parents from inheritance_map, whether all of them are in class_map
    // If a class is a parent in inheritance_map but not in class_map
    // Then it has Error 3, the only exception is Object which has not parent
    std::set<Symbol> error3_set; // Record in Error 3 and used for Error 4

    for (auto symbol_symbols : this->inheritance_map) {
      Symbol parent = symbol_symbols.first;
      if (!this->class_map.count(parent)) {
        auto children = symbol_symbols.second;
        for (auto iter = children.begin(); iter != children.end(); ++iter) {
          Symbol name = *iter;
          if (!name->equal_string("Object", 6)) {
            error3_set.insert(name);
            Class_ c = this->class_map[name];
            semant_error(c) << "Class " << name << " inherits from an undefined class " << parent << ".\n";
            ++semant_errors;
            correctness = false;
          }
        }
      }
    }

    // Error 4: Cyclic inheritance
    // If any class in class_map is not in the traversed list above
    // Then it is not reachable from Object
    // Then it must be either Error 3 or Error 4 (if not in error3_set)
    // See test/inherit_bad_cycle.cl for details
    for (auto symbol_class : this->class_map) {
      Symbol name = symbol_class.first;
      if (!traversed.count(name) && !error3_set.count(name)) {
        Class_ c = symbol_class.second;
        semant_error(c) << "Class " << name << ", or an ancestor of " << name << ", is involved in an inheritance cycle.\n";
        ++semant_errors;
        correctness = false;
      }
    }

    return correctness;
  }

    // Checks that each variable is named properly and accessed in its own scope
    void check_naming_and_scoping() {
        naming_and_scoping_DFS(idtable.lookup_string("Object"));
    }

    // DFS used to traverse through the inheritance tree for naming and scoping section
    bool naming_and_scoping_DFS(Class_ c) {
        bool correctness = true;
        
        // Enter a new scope for each new class
        curr_scope_vars->enterscope();
        
        // Checks for conflicts for the naming of attributes and methods
        correctness = correctness && check_naming(c);

        // Checks for conflicts in the expressions/body of methods and attributes
        correctness = correctness && check_scoping(c);

        // Enter the DFS to check child classes
        auto set = this->inheritance_map[c->get_name()];
        for (auto iter = set.begin(); iter != set.end(); ++iter) {
            correctness = correctness && naming_and_scoping_DFS(*iter);
        }

        // Done processing the current scope; exit the scope
        curr_scope_vars->exitscope();

        return correctness;
    }

    // Checks naming conflicts and adds them to the current scope
    bool check_naming(Class_ c) {
        bool correctness = true;
        // Enter the scope
        Features features = c->get_features();
        for (int i = features -> first(); features -> more(i); i = features -> next(i)) {
            Feature curr_feature = features->nth(i);
            if (curr_feature->instanceof("attr_class")) {
                /* ERROR 1: Duplicate Definitions of Attributes*/
                if (curr_scope_vars -> probe(curr_feature -> get_name()) != NULL) {
                    semant_error(c) << "Attribute " << curr_feature -> get_name() << " is multiply defined in class.\n";
                    ++semant_errors;
                    correctness = false;
                }
                /* ERROR 2: Overriding Attributes */
                else if (curr_scope_vars -> lookup(curr_feature -> get_name()) != NULL) {
                    semant_error(c) << "Attribute " << curr_feature -> get_name() << " is an attribute of an inherited class.\n";
                    ++semant_errors;
                    correctness = false;
                }
                else {
                    curr_scope_vars -> addid(curr_feature -> get_name(), new Symbol(curr_feature -> get_type()));
                }
            }
            else if (curr_feature->instanceof("method_class")) {
                /* ERROR 3: Duplicate Definitions of Methods in a Class */
                if (method_table[c->get_name()].count(curr_feature->get_name())) {
                    semant_error(c) << "Method " << curr_feature->get_name() << " is multiply defined in class.\n";
                    ++semant_errors;
                    correctness = false;
                }
                else {
                    method_table[c->get_name()].insert(curr_feature->get_name());
                }
            }
        }
        return correctness;
    }

    // Checks scoping conflicts and adds them to the current scope; also processes expressions
    bool check_scoping(Class_ c) {
        bool correctness = true;
        Features features = c->get_features();
        for (int i = features -> first(); features -> more(i); i = features -> next(i)) {
            Feature curr_feature = features->nth(i);

            // Handles Multiply defined Formals (parameters) in Method Declarations
            if (curr_feature->instanceof("method_class")) {
                Formals formals = curr_feature->get_formals();
                curr_scope_vars->enterscope();
                for (int j = formals -> first(); formals -> more(j); j = formals -> next(j)) {
                    Formal curr_formal = formals->nth(j);
                    /* ERROR 4: Duplicate Definitions of Formals in a Method */
                    if (curr_scope_vars -> probe(curr_formal->get_name()) != NULL) {
                        semant_error(c) << "Formal Parameter " << curr_formal->get_name() << "is multiply defined.\n";
                        ++semant_errors;
                        correctness = false;
                    }
                    curr_scope_vars -> addid(curr_feature -> get_name(), new Symbol(curr_feature -> get_type()));
                }
                curr_scope_vars->exitscope();
            }
            
            Expression expr = curr_feature -> get_expression();
            correctness = correctness && check_expression(c, expr);
        }
        return correctness;
    }

    bool check_expression(Class_ c, Expression expr) {
        bool correctness = true;

        if (expr->instanceof("let_class")) {
            curr_scope_vars->enterscope();

            // Any Let Expression checking done here
            curr_scope_vars->addid(expr->get_name(), new Symbol(expr->get_type()));
            check_expression(c, expr->get_expr()); // We will need to verify type of this later on
            check_expression(expr->get_body());
            
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("typcase_class")) {
            curr_scope_vars->enterscope();
            Cases c = expr->get_cases();
            for (int i = c->first(); c->more(i); i = c->next(i)) {
                Case curr_case = c->nth(i);
                curr_scope_vars->enterscope();
                curr_scope_vars -> addid(curr_case -> get_name(), new Symbol(curr_case -> get_type()));
                curr_scope_vars->exitscope();
            }
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("dispatch_class")) {
            // dispatch
            curr_scope_vars->enterscope();

            // Step 1: check whether each argument is a legal expression
            Expressions arguments = expr->get_arguments();
            for (int i = arguments -> first(); arguments -> more(i); i = arguments -> next(i)) {
              Expression argument = arguments->nth(i);
              check_expression(c, argument);
            }

            // dispatch on self object
            if (expr->get_expr()->get_name()->equal_string("self", 4)) {
              // Step 2: check method in current class
              if (method_table[c->get_name()].count(expr->get_name())) {
                // TODO: sth else in type checking?
              } else {
                semant_error(c) << "Dispatch to undefined method " << expr->get_name() << ".\n";
                ++semant_errors;
                correctness = false;
              }

            }
            // dispatch not on self object
            else {
              // Step 2: check objectID
              check_expression(c, expr->get_expr());

              // Step 3: check method in type part, we don't know the type of expr->get_expr() now
              
            }
            curr_scope_vars->exitscope();
        }
        else if (expr->instanceof("static_dispatch_class")) {
            // static dispatch

            // Step 1: check whether each argument is a legal expression
            Expressions arguments = expr->get_arguments();
            for (int i = arguments -> first(); arguments -> more(i); i = arguments -> next(i)) {
              Expression argument = arguments->nth(i);
              check_expression(c, argument);
            }

            // Step 2: check objectID
            check_expression(c, expr->get_expr());

            // Step 3: TODO: check type to be dispatched on

            // Step 4: check method in type part
            if (method_table[expr->get_type()].count(expr->get_name())) {
              // TODO: sth else in type checking?
            } else {
              semant_error(c) << "Static dispatch to undefined method " << expr->get_name() << ".\n";
              ++semant_errors;
              correctness = false;
            }
        }
        
        return correctness;
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

