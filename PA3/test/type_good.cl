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
    check_test() : Bool {
        false
    };
};

Class A4 {
    x : Int;
};

Class B {
    a : A <- new A;
    a1 : A1 <- new A1;
    a2 : A2 <- new A2;
    a3 : A3 <- new A3;

    check_A() : Int {
        {
            (* dispatch on a, which is type A *)
            a.method_A();
            (* dispatch on a1, which is type A1 *)
            a1.method_A();
            (* dispatch on a2, which is type A2 *)
            a2.method_A();
            (* dispatch on a3, which is type A3 *)
            a3.method_A();

            (* static dispatch on object a3 and type A *)
            a3@A.method_A();
            (* static dispatch on object a3 and type A1 *)
            a3@A1.method_A();
            (* static dispatch on object a3 and type A2 *)
            a3@A2.method_A();
            (* static dispatch on object a3 and type A3 *)
            a3@A3.method_A();

            1;
        }
    };
};

Class B1 inherits B {};
Class B11 inherits B1 {};
Class B12 inherits B1 {};
Class B2 inherits B {};
Class B21 inherits B2 {};
Class B22 inherits B2 {};


Class C {
    a : A;
    a1 : A1;
    a2 : A2;
    a3: A3;
    b : B;
    b1 : B1;
    b11 : B11;
    b12 : B12;
    b2 : B2;
    b21 : B21;
    b22 : B22;

    cond() : Int {
        {
            if true then a1 else a2 fi;
            if true then a2 else a3 fi;
            if true then a3 else a2 fi;
            if true then b22 else a3 fi;
            if true then b21 else b22 fi;
            if true then b22 else b11 fi;
            0;
        }
    };
};

Class Main inherits IO {
    (* Class People of attribute person1 is undefined. *)
    (* 'new' used with undefined class People. *)
    person1 : People <- new People;
    person2 : Int <- person1@Int.copy();

    (* Class Male of attribute man1 is undefined. *)
    (* 'new' used with undefined class Male. *)
    man1 : Male <- new Male;
    (* Class Male of attribute man2 is undefined. *)
    (* Dispatch on undefined class Male. *)
    man2 : Male <- man1.copy();

    a : A <- new A;
    a1 : A1 <- new A1;
    a2 : A2 <- new A2;
    a3 : A3 <- new A3;

    main() : Int {
        {
            a.method_A();
            a1.method_A();
            a2.method_A();
            a3.method_A();

            a1.method_A1();
            a2.method_A1();
            a3.method_A1();

            a2.method_A2();
            a3.method_A2();

            a3.method_A3();

            a@A.method_A();
            a1@A.method_A();
            a2@A1.method_A1();

            (* (* First round dispatch *)
            * out_string("dispatch method_A on a: ").out_string(a.method_A()).out_string("\n");
            * out_string("dispatch method_A on a1: ").out_string(a1.method_A()).out_string("\n");
            * out_string("dispatch method_A on a2: ").out_string(a2.method_A()).out_string("\n");
            * out_string("dispatch method_A on a3: ").out_string(a3.method_A()).out_string("\n");

            * out_string("dispatch method_A1 on a1: ").out_string(a1.method_A1()).out_string("\n");
            * out_string("dispatch method_A1 on a2: ").out_string(a2.method_A1()).out_string("\n");
            * out_string("dispatch method_A1 on a3: ").out_string(a3.method_A1()).out_string("\n");

            * out_string("dispatch method_A2 on a2: ").out_string(a2.method_A2()).out_string("\n");
            * out_string("dispatch method_A2 on a3: ").out_string(a3.method_A2()).out_string("\n");

            * out_string("dispatch method_A2 on a3: ").out_string(a3.method_A3()).out_string("\n");

            * (* Second round dispatch *)
            * out_string("dispatch method_A on a: ").out_string(a.method_A()).out_string("\n");
            * out_string("dispatch method_A on a1: ").out_string(a1.method_A()).out_string("\n");
            * out_string("dispatch method_A on a2: ").out_string(a2.method_A()).out_string("\n");
            * out_string("dispatch method_A on a3: ").out_string(a3.method_A()).out_string("\n");

            * out_string("dispatch method_A1 on a1: ").out_string(a1.method_A1()).out_string("\n");
            * out_string("dispatch method_A1 on a2: ").out_string(a2.method_A1()).out_string("\n");
            * out_string("dispatch method_A1 on a3: ").out_string(a3.method_A1()).out_string("\n");

            * out_string("dispatch method_A2 on a2: ").out_string(a2.method_A2()).out_string("\n");
            * out_string("dispatch method_A2 on a3: ").out_string(a3.method_A2()).out_string("\n");

            * out_string("dispatch method_A2 on a3: ").out_string(a3.method_A2()).out_string("\n");

            * (* Static dispatch *)
            * out_string("static dispatch method_A on object a type A: ").out_string(a@A.method_A()).out_string("\n");
            * out_string("static dispatch method_A on object a1 type A: ").out_string(a1@A.method_A()).out_string("\n");
            * out_string("static dispatch method_A1 on object a2 type A1: ").out_string(a2@A1.method_A1()).out_string("\n");
            *)
            
            0;
        }   
    };
};
