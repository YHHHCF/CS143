(*
 *  Test good examples of formal
 *)

(* one formal *)
CLASS A {
    f1(x : Int) : Int {
        1
    };
};

(* two formals *)
CLASS A {
    f1(x : Int, y : Bool) : Int {
        1
    };
};

(* three formals *)
CLASS B {
    f1(x : A, y : Bool, z : String) : Int {
        1
    };
};
