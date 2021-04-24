(*
 *  Test good examples of feature
 *)

(* has attribute *)
CLASS A {
    x1 : Int;
    f1() : Int {
        1
    };
};

(* has attribute with assign *)
CLASS A {
    x1 : Int <- 2;
    f1() : Int {
        3
    };
};

(* has method *)
CLASS A {
    f1() : Int {
        4
    };
};

(* has attribute with assign and method *)
CLASS A {
    x1 : Int <- 5;
    f1() : Int {
        6
    };
};
