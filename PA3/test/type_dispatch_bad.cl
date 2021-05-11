(* Good examples for type checking *)

(* All 3 types of dispatch *)
Class A {
    a : String <- "a";

    method_A() : String {
        a
    };
};

Class A1 inherits A {
    a1 : String <- "a1";

    method_A() : String {
        {
            a <- "a1";
        }
    };
    method_A1() : String {
        a1
    };
};

Class A2 inherits A1 {
    a2 : String <- "a2";

    method_A1() : String {
        {
            a <- "a2";
            a1 <- "a2";
        }
    };
    method_A2() : String {
        a2
    };
};

Class A3 inherits A2 {
    a3 : String <- "a3";

    method_A2() : String {
        {
            a <- "a3";
            a1 <- "a3";
            a2 <- "a3";
        }
    };
    method_A3() : String {
        a3
    };
    (* dispatch on self, which is SELF_TYPE *)
    check_A2() : String {
        method_A2()
    };
    (* dispatch on self, which is SELF_TYPE *)
    check_A3() : String {
        method_A3()
    };
};

Class Main {
    main() : Int {
      1
    };
};
