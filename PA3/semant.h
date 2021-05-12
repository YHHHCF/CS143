#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include <map>
#include <set>
#include <list>
#include <stack>
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

    // parent_map is used to get the Child's Parent
    // key is a typeID for child, value is a typeID for parent
    std::map<Symbol, Symbol> parent_map;

    // Used to manage current scope for naming_and_scoping_DFS; maps an attribute's objectID to typeID
    SymbolTable<Symbol, Symbol> *curr_scope_vars = new SymbolTable<Symbol, Symbol>();
    
    // key is a class's typeID, value is aan attribute map for that class
    // an attribute map's key is objectID, value is the Feature class
    std::map<Symbol, std::map<Symbol, Feature> > attribute_table;

    // key is a class's typeID, value is a method map for that class
    // a method map's key is methodID, value is the Feature class
    std::map<Symbol, std::map<Symbol, Feature> > method_table;

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

        // Add the child and parent to parent_map
        this->parent_map[typeID] = parent_typeID;

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
        naming_DFS(class_map[idtable.lookup_string("Object")]);
        scoping_and_typing_DFS(class_map[idtable.lookup_string("Object")]);
    }

    // DFS used to traverse through the inheritance tree for naming section
    void naming_DFS(Class_ c) {
        // Enter a new scope for each new class
        curr_scope_vars->enterscope();

        // Checks for conflicts in the expressions/body of methods and attributes
        check_naming(c);

        // Enter the DFS to check child classes
        auto children_typeIDs = this->inheritance_map[c->get_typeID()];
        for (auto iter = children_typeIDs.begin(); iter != children_typeIDs.end(); ++iter) {
            naming_DFS(this->class_map[*iter]);
        }

        // Done processing the current scope; exit the scope
        curr_scope_vars->exitscope();
    }

    // DFS used to traverse through the inheritance tree for scoping and typing section
    void scoping_and_typing_DFS(Class_ c) {
        // Enter a new scope for each new class
        curr_scope_vars->enterscope();
        
        // Checks for conflicts in the expressions/body of methods and attributes
        check_scoping_and_typing(c);

        // Enter the DFS to check child classes
        auto children_typeIDs = this->inheritance_map[c->get_typeID()];
        for (auto iter = children_typeIDs.begin(); iter != children_typeIDs.end(); ++iter) {
            Class_ next = this->class_map[*iter];
            if ((strcmp(next->get_typeID()->get_string(), "String") != 0)) {
                if ((strcmp(next->get_typeID()->get_string(), "Bool") != 0)) {
                    if ((strcmp(next->get_typeID()->get_string(), "Int") != 0)) {
                        scoping_and_typing_DFS(this->class_map[*iter]);
                    }
                }
            }
        }

        // Done processing the current scope; exit the scope
        curr_scope_vars->exitscope();
    }

    // Checks naming conflicts and adds them to the current scope
    void check_naming(Class_ c) {
        if (semant_debug) {
            printf("check_naming for class: %s\n", c->get_typeID()->get_string());
        }
        std::map<Symbol, Feature> curr_method_map;
        std::map<Symbol, Feature> curr_attribute_map;
        
        // Enter the scope
        Features features = c->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i)) {
            Feature curr_feature = features->nth(i);
            if (curr_feature->instanceof("attr_class")) {
                if (semant_debug) {
                    printf("checking attr: %s, %s\n", curr_feature->get_objectID()->get_string(), curr_feature->get_typeID()->get_string());
                }
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
                    Symbol attr_typeID = curr_feature->get_typeID();
                    // Handle prim_slot for Int, Bool, String
                    if (attr_typeID->equal_string("_prim_slot", 10)) {
                        curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(c->get_typeID()));
                        continue;
                    }
                    curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(curr_feature->get_typeID()));
                    curr_attribute_map[curr_feature->get_objectID()] = curr_feature;
                }
            }
            else if (curr_feature->instanceof("method_class")) {
                if (semant_debug) {
                    printf("checking method: %s\n", curr_feature->get_methodID()->get_string());
                }
                /* ERROR 4: Duplicate Definitions of Methods in a Class */
                // if the method table of this class contains the methodID
                if (curr_method_map.count(curr_feature->get_methodID())) {
                    semant_error(c) << "Method " << curr_feature->get_methodID() << " is multiply defined.\n";
                    ++semant_errors;
                } else {
                    curr_method_map[curr_feature->get_methodID()] = curr_feature;
                }
            }
        }
        // the attributes and methods for current class
        this->attribute_table[c->get_typeID()] = curr_attribute_map;
        this->method_table[c->get_typeID()] = curr_method_map;
    }

    // Checks scoping conflicts and adds them to the current scope; also processes expressions
    void check_scoping_and_typing(Class_ c) {
        Features features = c->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i)) {
            Feature curr_feature = features->nth(i);
            // Handles Multiply defined Formals (parameters) in Method Declarations
            if (curr_feature->instanceof("method_class")) {
                Formals formals = curr_feature->get_formals();
                curr_scope_vars->enterscope();

                if (semant_debug) {
                    printf("class %s : begin check expression for method %s\n", c->get_typeID()->get_string(), curr_feature->get_methodID()->get_string());
                }
                
                // Check Formals
                for (int j = formals->first(); formals->more(j); j = formals->next(j)) {
                    Formal curr_formal = formals->nth(j);
                    // ERROR 5: Duplicate Definitions of Formals in a Method 
                    if (curr_scope_vars->probe(curr_formal->get_objectID()) != NULL) {
                        semant_error(c) << "Formal Parameter " << curr_formal->get_objectID() << " is multiply defined.\n";
                        ++semant_errors;
                    }
                    curr_scope_vars->addid(curr_formal->get_objectID(), new Symbol(curr_formal->get_typeID()));
                }
                // Handles Correctly Defined Methods
                Symbol expected_typeID = curr_feature->get_typeID();
                Symbol evaluated_typeID = check_expression(c, curr_feature->get_expression());

                // basic classes may have null method body
                // if (!idtable.lookup_string("no_expression")) {   <-- this is previous code
                if (strcmp(evaluated_typeID->get_string(), "no_expression") != 0) {  //  <-- I changed to this; resolved an error
                    if (!conform(expected_typeID, evaluated_typeID)) {
                        semant_error(c) << "Inferred return type " << evaluated_typeID << " of method " << curr_feature->get_methodID() 
                                    << " does not conform to declared return type " << expected_typeID << ".\n";
                        ++semant_errors;
                    }
                }

                if (semant_debug) {
                    printf("class %s : finish check expression for method %s\n", c->get_typeID()->get_string(), curr_feature->get_methodID()->get_string());
                }

                curr_scope_vars->exitscope();
            }
            // Handling Attributes
            else {
                if (semant_debug) {
                    printf("class %s : begin check expression for attribute %s\n", c->get_typeID()->get_string(), curr_feature->get_objectID()->get_string());
                }

                Symbol expected_typeID = curr_feature->get_typeID();
                Symbol evaluated_typeID = check_expression(c, curr_feature->get_expression());

                if (semant_debug) {
                    printf("expected typeID: %s; evaluated typeID: %s\n", expected_typeID->get_string(), evaluated_typeID->get_string());
                }
                // If expected_typeID is not defined, report error
                if (!this->class_map.count(expected_typeID)) {
                    semant_error(c) << "Class " << expected_typeID << " of boobo attribute " << curr_feature->get_objectID() << " is undefined.\n";
                    ++semant_errors;
                    curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(idtable.lookup_string("Object")));
                } else {
                    if ((!conform(expected_typeID, evaluated_typeID)) && (strcmp(evaluated_typeID->get_string(), "no_expression") != 0)) {
                        semant_error(c) << "Inferred type " << evaluated_typeID << " of attribute " << curr_feature->get_objectID() 
                                        << " does not conform to declared type " << expected_typeID << ".\n";
                        ++semant_errors;
                        curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(idtable.lookup_string("Object")));
                    }
                    else {
                        curr_scope_vars->addid(curr_feature->get_objectID(), new Symbol(curr_feature->get_typeID()));
                    }

                    if (semant_debug) {
                        print_symbol_table();
                        printf("class %s : finish check expression for attribute\n", c->get_typeID()->get_string());
                    }
                }                
            }
        }
    }

    Symbol check_expression(Class_ c, Expression expr) {
        if (expr->instanceof("let_class")) {
            if (semant_debug) {
                printf("check_expression for let_class\n");
            }
            curr_scope_vars->enterscope();
            // Any Let Expression checking done here
            curr_scope_vars->addid(expr->get_objectID(), new Symbol(expr->get_typeID()));
            check_expression(c, expr->get_init_expression()); // TODO: verify type of this
            Symbol type = check_expression(c, expr->get_body_expression());
            curr_scope_vars->exitscope();

            if (semant_debug) {
                printf("let_class : %s\n", type->get_string());
            }
            return type;
        }
        else if (expr->instanceof("typcase_class")) {                                                    // I am a little confused about the implementation; only basic skeleton code ahead
            if (semant_debug) {
                printf("check_expression for typcase_class\n");
            }
            curr_scope_vars->enterscope();
            Expression e = expr->get_expression();                                                       // Is this an attribute? What if it doesn't exist?
            Symbol type_e = *(curr_scope_vars->lookup(e->get_objectID()));                               // This should be dynamic type -- not sure if I am correct in doing this
            Cases cases_ = expr->get_cases();
            Case selected_case = cases_->nth(1);

            // Selecting the correct branch
            for (int i = cases_->first(); cases_->more(i); i = cases_->next(i)) {
                Case curr_case = cases_->nth(i);
                curr_scope_vars->enterscope();

                // Inside case variable scope
                curr_scope_vars->addid(curr_case->get_objectID(), new Symbol(curr_case->get_typeID()));
                Symbol type_k = curr_case->get_typeID();
                if (conform(type_k, type_e)) {  // if the typek conforms to type of expr0
                    if (equal(type_k, least_common_ancestor(type_k, selected_case->get_typeID()))) {    // If the case we are evaluating has a type closer bound to expr0 than selected_case
                        selected_case = curr_case;                                                       // If there are multiple of the same type_k, we choose the last one -- is this correct?
                    }
                }
                check_expression(c, curr_case->get_expression());                                        // Do all the branches need to be correct expressions, or just the one we evaluate to?
                curr_scope_vars->exitscope();
            }
            curr_scope_vars->exitscope();
            // TODO: Implement assignment of expr0 to selected_case ( I don't really understand case, sorry :( )
            Symbol type = check_expression(c, selected_case->get_expression());  // technically computing twice
            if (semant_debug) {
                printf("typcase_class : %s\n", type->get_string());
            }
            return type;
        }
        else if (expr->instanceof("dispatch_class")) {
            if (semant_debug) {
                printf("check_expression for dispatch_class\n");
            }
            // dispatch: e0.f(e1, e2, ...., en) or self.f(e1, e2, ...., en)
            Symbol T0, Ti, Ti_declare;
            // Step 1: evaluate e0 and T0
            if (is_self(expr->get_expression())) {
                // T0 is current class typeID
                T0 = c->get_typeID();
                if (semant_debug) {
                    printf("Dispatch on self : %s\n", T0->get_string());
                }
            } else {
                // evaluate e0 and find its typeID T0
                T0 = check_expression(c, expr->get_expression());
                if (semant_debug) {
                    printf("Dispatch on an object : %s\n", T0->get_string());
                }
            }

            // check T0 defined
            if (!this->class_map.count(T0)) {
                semant_error(c) << "Dispatch on undefined class " << T0 << ".\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }

            // Step 2: find method and formals
            // check method defined
            Class_ method_implement_class = check_method(T0, expr->get_methodID());
            if (!method_implement_class) {
                semant_error(c) << "Dispatch to undefined method " << expr->get_methodID() << ".\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }
            Feature feature = this->method_table[method_implement_class->get_typeID()][expr->get_methodID()];
            Formals formals = feature->get_formals();

            // Step 3: check whether each argument is a legal expression and the types conform to formal types
            Expressions arguments = expr->get_arg_expressions();
            if (arguments->len() != formals->len()) {
                semant_error(c) << "Method " << expr->get_methodID() << " invoked with wrong number of arguments.\n";
                ++semant_errors;
                // return the declared return type in this case
                return feature->get_typeID();
            }
            for (int i = arguments->first(); arguments->more(i); i = arguments->next(i)) {
                Expression curr_argument = arguments->nth(i);
                Ti = check_expression(c, curr_argument);

                Formal curr_formal = formals->nth(i);
                Ti_declare = curr_formal->get_typeID();

                if (!conform(Ti, Ti_declare)) {
                    // do not return in the for loop, want to show all this kind of parameter errors in a dispatch
                    semant_error(c) << "In call of method " << expr->get_methodID() << ", type " << Ti << " of parameter " \
                    << curr_formal->get_objectID() << " does not conform to declared type " << Ti_declare << ".\n";
                    ++semant_errors;
                }
            }

            // Step 4: return the declared return type
            Symbol type = feature->get_typeID();
            if (semant_debug) {
                printf("dispatch_class : %s\n", type->get_string());
            }
            return type;
        }
        else if (expr->instanceof("static_dispatch_class")) {
            if (semant_debug) {
                printf("check_expression for static_dispatch_class\n");
            }
            // static dispatch: e0@T.f(e1, e2, ...., en)
            Symbol T0, Ti, Ti_declare;
            // Step 1: evaluate e0 and T0
            if (is_self(expr->get_expression())) {
                // it is legal to use self as e0
                T0 = c->get_typeID();
            } else {
                // evaluate e0 and find its typeID T0
                T0 = check_expression(c, expr->get_expression());
            }

            // check T defined
            if (!this->class_map.count(expr->get_typeID())) {
                semant_error(c) << "Static dispatch on undefined class " << expr->get_typeID() << ".\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }

            // check conform for static dispatch
            if (!conform(T0, expr->get_typeID())) {
                semant_error(c) << "Expression type " << T0 << " does not conform to declared static dispatch type " \
                << expr->get_typeID() << ".\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }

            // Step 2: find method and formals
            // check method defined
            Class_ method_implement_class = check_method(expr->get_typeID(), expr->get_methodID());
            if (!method_implement_class) {
                semant_error(c) << "Static dispatch to undefined method " << expr->get_methodID() << ".\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }
            Feature feature = this->method_table[method_implement_class->get_typeID()][expr->get_methodID()];
            Formals formals = feature->get_formals();

            // Step 3: check whether each argument is a legal expression and the types conform to formal types
            Expressions arguments = expr->get_arg_expressions();
            if (arguments->len() != formals->len()) {
                semant_error(c) << "Method " << expr->get_methodID() << " invoked with wrong number of arguments.\n";
                ++semant_errors;
                // return the declared return type in this case
                return feature->get_typeID();
            }
            for (int i = arguments->first(); arguments->more(i); i = arguments->next(i)) {
                Expression curr_argument = arguments->nth(i);
                Ti = check_expression(c, curr_argument);

                Formal curr_formal = formals->nth(i);
                Ti_declare = curr_formal->get_typeID();

                if (!conform(Ti, Ti_declare)) {
                    // do not return in the for loop, want to show all this kind of parameter errors in a dispatch
                    semant_error(c) << "In call of method " << expr->get_methodID() << ", type " << Ti << " of parameter " \
                    << curr_formal->get_objectID() << " does not conform to declared type " << Ti_declare << ".\n";
                    ++semant_errors;
                }
            }

            // Step 4: return the declared return type
            Symbol type = feature->get_typeID();
            if (semant_debug) {
                printf("static_dispatch_class : %s\n", type->get_string());
            }
            return type;
        }
        else if (expr->instanceof("assign_class")) {
            if (semant_debug) {
                printf("check_expression for assign_class\n");
            }
            // type_expr is the type of init expression
            Symbol type_expr = check_expression(c, expr->get_expression());

            // attr_objectID is the objectID of the attribute to be assigned
            Symbol attr_objectID = expr->get_objectID();
            if (!curr_scope_vars->lookup(attr_objectID)) {
                semant_error(c) << "Assignment to undeclared identifier " << attr_objectID << ".\n";
                ++semant_errors;
                return type_expr;
            }

            // type_expected is the typeID fo the attribute to be assigned
            Symbol type_expected = *(curr_scope_vars->lookup(attr_objectID));
            if (!conform(type_expr, type_expected)) {
                semant_error(c) << "Type " << type_expr <<" of assigned expression does not conform to declared type " \
                << type_expected << " of identifier " << attr_objectID << ".\n";
                ++semant_errors;
                return type_expr;
            }
            if (semant_debug) {
                printf("assign_class : %s\n", type_expected->get_string());
            }
            return type_expr;
        }
        else if (expr->instanceof("cond_class")) {
            if (semant_debug) {
                printf("check_expression for cond_class\n");
            }
            Symbol type_pred = check_expression(c, expr->get_pred_expression());
            if (!isBool(type_pred)) {
                semant_error(c) << "Predicate of 'if' does not have type Bool.\n";
                ++semant_errors;
            }
            Symbol type_then = check_expression(c, expr->get_then_expression());
            Symbol type_else = check_expression(c, expr->get_else_expression());
            Symbol type_ret = least_common_ancestor(type_then, type_else);
            if (semant_debug) {
                printf("cond_class : %s\n", type_ret->get_string());
            }
            return type_ret;
        }
        else if (expr->instanceof("loop_class") ) {
            if (semant_debug) {
                printf("check_expression for loop_class\n");
            }
            Symbol type_pred = check_expression(c, expr->get_pred_expression());
            if (!isBool(type_pred)) {
                semant_error(c) << "Loop condition does not have type Bool.\n";
                ++semant_errors;
            }
            check_expression(c, expr->get_body_expression());
            if (semant_debug) {
                printf("loop_class : Object\n");
            }
            // return a void, which is Object type
            return idtable.lookup_string("Object");
        }
        else if (expr->instanceof("block_class") ) {
            if (semant_debug) {
                printf("check_expression for block_class\n");
            }
            Expressions exprs = expr->get_body_expressions();

            // If the body is empty, return Object
            if (exprs->len() == 0) {
                semant_error(c) << "The block expression is empty.\n";
                ++semant_errors;
                return idtable.lookup_string("Object");
            }

            Symbol type_e;  // Keeps track of the TYPEID of expressions, used for type checking
            for (int i = exprs->first(); exprs->more(i); i = exprs->next(i)) {
                Expression curr_expr = exprs->nth(i);
                type_e = check_expression(c, curr_expr);
            }
            if (semant_debug) {
                printf("block_class : %s\n", type_e->get_string());
            }
            return type_e;  // Contains the TYPEID of the last expression
        }
        else if (expr->instanceof("new__class")) {
            if (semant_debug) {
                printf("check_expression for new__class\n");
            }
            if (!this->class_map.count(expr->get_typeID())) {
                semant_error(c) << "'new' used with undefined class " << expr->get_typeID() << "\n";
                ++semant_errors;
                if (semant_debug) {
                    printf("new__class : Object\n");
                }
                return idtable.lookup_string("Object");
            }
            if (semant_debug) {
                printf("new__class : %s\n", expr->get_typeID()->get_string());
                // printf("Debug new class: %ld\n", this->class_map.count(expr->get_typeID()));
            }
            return expr->get_typeID();
        }
        else if (expr->instanceof("isvoid_class")) {
            if (semant_debug) {
                printf("check_expression for isvoid_class\n");
            }
            check_expression(c, expr->get_expression());
            if (semant_debug) {
                printf("isvoid_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        }
        else if (expr->instanceof("plus_class")) {
            if (semant_debug) {
                printf("check_expression for plus_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " + " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("plus_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("sub_class")) {
            if (semant_debug) {
                printf("check_expression for sub_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " - " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("sub_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("mul_class")) {
            if (semant_debug) {
                printf("check_expression for mul_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " * " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("mul_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("divide_class")) {
            if (semant_debug) {
                printf("check_expression for divide_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " / " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("divide_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("neg_class")) {
            if (semant_debug) {
                printf("check_expression for neg_class\n");
            }
            Symbol type_e = check_expression(c, expr->get_expression());
            if (!isInt(type_e)) {
                semant_error(c) << "Argument of ~ has type " << type_e << " instead of Int.\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("neg_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("lt_class")) {
            if (semant_debug) {
                printf("check_expression for lt_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " < " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("lt_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        }
        else if (expr->instanceof("leq_class")) {
            if (semant_debug) {
                printf("check_expression for leq_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " <= " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("leq_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        }
        else if (expr->instanceof("eq_class")) {
            if (semant_debug) {
                printf("check_expression for eq_class\n");
            }
            Symbol type_e1 = check_expression(c, expr->get_expression1());
            Symbol type_e2 = check_expression(c, expr->get_expression2());
            if (!isInt(type_e1) || !isInt(type_e2)) {
                semant_error(c) << "non_Int arguments: " << type_e1 << " = " << type_e2 << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("eq_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        }
        else if (expr->instanceof("comp_class")) {
            if (semant_debug) {
                printf("check_expression for comp_class\n");
            }
            Symbol type = check_expression(c, expr->get_expression());
            if (!isBool(type)) {
                semant_error(c) << "Argument of 'not' has type " << type << " instead of Bool." << "\n";
                ++semant_errors;
            }
            if (semant_debug) {
                printf("comp_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        }
        else if (expr->instanceof("object_class")) {
            if (semant_debug) {
                printf("check_expression for object_class\n");
            }
            if (!curr_scope_vars->lookup(expr->get_objectID())) {
                semant_error(c) << "Undeclared identifier " << expr->get_objectID() << ".\n";
                ++semant_errors;
                // If cannot find this object, return Object
                return idtable.lookup_string("Object");
            }
            Symbol type = *(curr_scope_vars->lookup(expr->get_objectID()));
            if (semant_debug) {
                printf("object_class : %s\n", type->get_string());
            }
            return type;
        }
        else if (expr->instanceof("int_const_class")) {
            if (semant_debug) {
                printf("int_const_class : Int\n");
            }
            return idtable.lookup_string("Int");
        }
        else if (expr->instanceof("string_const_class")) {
            if (semant_debug) {
                printf("string_const_class : String\n");
            }
            return idtable.lookup_string("String");
        }
        else if (expr->instanceof("bool_const_class")) {
            if (semant_debug) {
                printf("bool_const_class : Bool\n");
            }
            return idtable.lookup_string("Bool");
        } else if (expr->instanceof("no_expr_class")) {
            // TODO1: need to handle more special cases in basic classes
            // TODO2: need to review when implementing SELF_TYPE
            if (semant_debug) {
                printf("class %s : has a no_expr that needs to be handled later\n", c->get_typeID()->get_string());
            }
            return idtable.lookup_string("no_expression");
        } else {
            // The code should never reach this -- default case
            if (semant_debug) {
                printf("class %s : has a expression not captured\n", c->get_typeID()->get_string());
            }
            return idtable.lookup_string("_no_type");
        }
    }

    // Given a typeID and a methodID, return the least ancestor's typeID
    // who has this methodID overidden or implemented
    Class_ check_method(Symbol typeID, Symbol methodID) {
        // if (semant_debug) {
        //     print_method_table();
        // }
        Class_ ret = nullptr;
        while (!typeID->equal_string("_no_class", 9)) {
            if (semant_debug) {
                printf("check_method %s from class %s\n", methodID->get_string(), typeID->get_string());
            }
            if (this->method_table[typeID].count(methodID)) {
                ret = this->class_map[typeID];
                if (semant_debug) {
                    printf("found in %s\n", typeID->get_string());
                }
                break;
            } else {
                typeID = this->parent_map[typeID];
            }
        }
        return ret;
    }

    // print all methods in method table for debug
    void print_method_table() {
        printf("========Print method_table Start=========\n");
        for (auto class_entry : this->method_table) {
            Symbol typeID = class_entry.first;
            printf("Class %s has methods: ", typeID->get_string());
            for (auto method_entry : class_entry.second) {
                Symbol methodID = method_entry.first;
                printf("%s ", methodID->get_string());
            }
            printf("\n");
        }
        printf("=========Print method_table End==========\n");
    }

    // print all symbols in symboltable
    void print_symbol_table() {
        printf("========Print method_table Start=========\n");
        curr_scope_vars->dump();
        printf("=========Print method_table End==========\n");
    }

    // return the least common ancestor for 2 typeIDs
    Symbol least_common_ancestor(Symbol typeID1, Symbol typeID2) {
        // return directly if one typeID is another's ancester
        if (conform(typeID1, typeID2)) {
            if (semant_debug) {
                printf("Lease common ancestor of %s, %s -> %s\n", typeID1->get_string(), typeID2->get_string(), typeID2->get_string());
            }
            return typeID2;
        }
        if (conform(typeID2, typeID1)) {
            if (semant_debug) {
                printf("Lease common ancestor of %s, %s -> %s\n", typeID1->get_string(), typeID2->get_string(), typeID1->get_string());
            }
            return typeID1;
        }
        
        std::stack<Symbol> stack1, stack2;
        // go from typeID1 up to Object and put it to a stack1
        Symbol curr = typeID1;
        while (!curr->equal_string("_no_class", 9)) {
            stack1.push(curr);
            curr = this->parent_map[curr];
        }

        // go from typeID2 up to Object and put it to a stack2
        curr = typeID2;
        while (!curr->equal_string("_no_class", 9)) {
            stack2.push(curr);
            curr = this->parent_map[curr];
        }

        // check first different symbol from top of stack1 and stack2
        curr = stack1.top(); // must be object;
        // stack1 and stack2 won't be empty before breaking out
        // since one type cannot be another type's ancestor
        while (!stack1.empty() && !stack2.empty()) {
            if (equal(stack1.top(), stack2.top())) {
                curr = stack1.top();
                stack1.pop();
                stack2.pop();
            } else {
                break;
            }
        }
        if (semant_debug) {
            printf("Lease common ancestor of %s, %s -> %s\n", typeID1->get_string(), typeID2->get_string(), curr->get_string());
        }
        return curr; // least common ancestor
    }

    // return true if typeID1 conform to (<=) typeID2
    bool conform(Symbol typeID1, Symbol typeID2) {
        Symbol curr = typeID1;
        while (!curr->equal_string("_no_class", 9)) {
            if (equal(curr, typeID2)) {
                return true;
            } else {
                curr = this->parent_map[curr];
            }
        }
        return false;
    }

    // return true if typeID1 equals to typeID2
    bool equal(Symbol typeID1, Symbol typeID2) {

        return strcmp(typeID1->get_string(), typeID2->get_string()) == 0;
    }

    bool isInt(Symbol typeID) {
        return strcmp(typeID->get_string(), "Int") == 0;
    }

    bool isString(Symbol typeID) {
        return strcmp(typeID->get_string(), "String") == 0;
    }

    bool isBool(Symbol typeID) {
        return strcmp(typeID->get_string(), "Bool") == 0;
    }

    // return true current expression is "self"
    bool is_self(Expression expr) {
        return expr->instanceof("object_class") &&
        expr->get_objectID()->equal_string("self", 4);
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

