(*
 * Object <- IO, Int, Bool, String, GrandParent, Main
 * IO <- IOChild1
 * IOChild1 <- IOChild2
 * GrandParent <- Parent1, Parent2, Parent3
 * Parent1 <- Child11, Child12
 * Parent2 <- Child21, Child22
 * Parent3 <- Child31, Child32
 *)

class IOChild1 inherits IO {
    aIOChild1 : Int;
};

class IOChild2 inherits IOChild1 {
    aIOChild2 : Int;
};

Class GrandParent {
    aGrandParent : Int;
};

Class Parent1 inherits GrandParent {
    aParent1 : Int;
};

Class Parent2 inherits GrandParent {
    aParent2 : Int;
};

Class Parent3 inherits GrandParent {
    aParent3 : Int;
};

Class Child11 inherits Parent1 {
    aChild11 : Int;
};

Class Child12 inherits Parent1 {
    aChild12 : Int;
};

Class Child21 inherits Parent2 {
    aChild21 : Int;
};

Class Child22 inherits Parent2 {
    aChild22 : Int;
};

Class Child31 inherits Parent3 {
    aChild31 : Int;
};

Class Child32 inherits Parent3 {
    aChild32 : Int;
};

Class Main {
    main() : Int {
      1
    };
};
