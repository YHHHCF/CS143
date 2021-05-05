(*
 * Bad examples of inheritance
 *)

(* Add a Main class so the other part of the semant won't complain *)
Class Main {
    main():C {
      1
    };
};

(* Error 1: Cannot inherit from Bool, Int, String *)
class BoolChild inherits Bool {
    a : Int;
};

class IntChild inherits Int {
    a : Int;
};

class StringChild inherits String {
    a : Int;
};

(*
 * Error 2: Cannot add already defined class
 * GrandParent <- Parent1, Parent1
 *)

Class GrandParent {
    a : Int;
};

Class Parent1 inherits GrandParent {
    a : Int;
};

Class Parent1 inherits GrandParent {
    a : Int;
};

Class Parent2 inherits GrandParent {
    a : Int;
};

(*
 * Error 3: Inherit from undefined class
 * GrandParent <- Parent1, Parent2, Parent3
 * Parent4 <- Child41
 *)

Class Child41 inherits Parent4 {
    a : Int;
};

Class Child42 inherits Parent4 {
    a : Int;
};
