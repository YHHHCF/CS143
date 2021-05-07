(* Naming and Scoping Errors *)

(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

(* Part 1: ObjectID Duplicate definitions in same scope *)
(* Error 1: Duplicate Attributes in same class *)
Class A {
    x : Int;
    x : Int;
};

(* Error 2: Overriding Attributes from parent *)
Class B {
    x : Int;
};
Class C inherits B {};
Class C1 inherits C {
    x : Int;
};

Class D {
    x : Int;
};
Class E inherits D {
    x : Int <- 3;
};

(* Error 3: Formal Parameters multiply defined *)
Class F {
    x : Int;
    init1(x : Int, y : Bool, x : Bool) : Int {
        {
            x <- x;
        }
    };

    init2(x : Int, y : Bool, x : Int) : Int {
        {
            x <- x;
        }
    };
};

(* Case branches and let will not have multi-defined issues *)

(* Part 2: ObjectID undeclared *)
(* Error 4: Undeclared ObjectID *)
Class G {
    (* Good example *)
    init1(num1 : Int, num2 : Int) : Int {
        let x : Int <- 1, y : Int <- 2 + x in {
            x + 1;
        }
    };

    init2(num1 : Int, num2 : Int) : Int {
        (* Undeclared identifier y (use Object) *)
        (* non-Int arguments: Int + Object (Implement in type) *)
        let x : Int <- 1 + y, y : Int <- 2 in {
            x + 1;
        }
    };
};

Class H {
    class_type(var : A) : Int {
        {
            (* Undeclared identifier a *)
            a;
            case var of
                a : A => {
                    a;
                    1;
                };
                b : B => {
                    (* Undeclared identifier a *)
                    a;
                    var;
                    2;
                };
                c : C => 3;
                d : D => 4;
                e : E => 5;
            esac;
            var;
            1;
        }
    };
};

Class I {
    init1(num1 : Int, num2 : Int) : Int {
        (* Undeclared identifier num3. *)
        (* non-Int arguments: Object + Int *)
        num3 + 1
    };
};
