(*
 * Error 4: contains cycle
 * Object <- GoodGrandParent
 * GoodGrandParent <- GoodParent1
 * GoodGrandParent <- GoodParent2
 * GoodParent1 <- GoodChild11
 * GoodParent1 <- GoodChild12
 * GoodParent2 <- GoodChild21
 * GoodParent2 <- GoodChild22
 * A <- B
 * B <- C
 * C <- A
 * C <- D
 * E <- F
 * F <- G
 * G <- E
 *)

(* Want part of the graph to be good to make sure the test is strong enough *)
Class GoodGrandParent {
    a : Int;
};

Class GoodParent1 inherits GoodGrandParent {
    a : Int;
};

Class GoodParent2 inherits GoodGrandParent {
    a : Int;
};

Class GoodChild11 inherits GoodParent1 {
    a : Int;
};

Class GoodChild12 inherits GoodParent1 {
    a : Int;
};

Class GoodChild21 inherits GoodParent2 {
    a : Int;
};

Class GoodChild22 inherits GoodParent2 {
    a : Int;
};

(* Bad cases below *)
Class B inherits A {
    a : Int;
};

Class C inherits B {
    a : Int;
};

Class A inherits C {
    a : Int;
};

Class D inherits C {
    a : Int;
};

Class F inherits E {
    a : Int;
};

Class G inherits F {
    a : Int;
};

Class E inherits G {
    a : Int;
};
