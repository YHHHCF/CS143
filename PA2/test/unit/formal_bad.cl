(*
 *  Test bad examples of formal
 *)

(* error: X is not an ObjectID *)
CLASS A {
    f1(X : Int) : Int {
        1
    };
};

(* error: int is not an typeID *)
CLASS A {
    f1(x : int) : Int {
        1
    };
};

(* error: missing colon *)
CLASS A {
    f1(x Int) : Int {
        1
    };
};
