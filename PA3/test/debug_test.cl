(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

(* Checks if the ground truth cascades typing errors *)
Class A {
    x : Bool <- false;
};

Class B inherits A {
    t : C;
    a : Int <- t.foo();
};

Class C {
    foo() : Int {
        false
    };
};

(* Just to check sample output *)
Class D {
    x : Int <- 1 <= false;
};
Class E {
    x : Int <- 1 < false;
};