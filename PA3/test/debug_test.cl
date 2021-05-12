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
    x : Bool;
    foo() : Bool {
        x <- 2
    };
};

(* Just to check sample output *)
Class D {
    x : Int;
};
Class E {
    x : Int <- 1 < false;
};

(* Checks if statement *)
Class F {
    x : Bool;
    foo() : Bool {
        if 1 then 1 else 0 fi
    };
};

(* Checks while loop *)
Class G {
    x : Bool;
    foo() : Bool {
        while 1 loop 1 pool
    };
};

(* Checks assign operator *)
Class H {
    x : Bool;
    foo() : Bool {
        x <- 2
    };
};

(* Checks case statements *)
class I {
    x : C;
    foo() : C {
        case var of
            a : A => 1;
            b : B => 2;
        esac
    };
};

(* What happens if an attribute is initialized as undeclared class? *)
class J {
    foo(i : Object) : Int {
        1
    };
};

class J_caller {
    y : J;
    x : People;
    foo1() : Int {
        y.foo(x)
    };
};

(* Checking error messages for type mismatches *)
class K {
    bool1 : Bool <- 1;
    bool2 : Bool <- "1";
    int1 : Int <- true;
    int2 : Int <- "1";
    string1 : String <- true;
    string2 : String <- 1;

    b1 : B;
    a1 : A <- a1;

    bool_foo() : Bool {
        1
    };
    int_foo() : Int {
        "string"
    };
    string_foo() : String {
        false
    };

    a_ob : A;
    b_ob : B;
    fooA() : A {
        b_ob
    };
    fooB() : B {
        a_ob
    };
};