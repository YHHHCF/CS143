(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

Class A {
    x : Bool <- false;
};

Class B inherits A {
    x : Int;
};