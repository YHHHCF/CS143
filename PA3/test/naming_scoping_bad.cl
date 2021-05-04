(* Naming and Scoping Errors *)

(* Added so semant won't complain *)
Class Main {
    main():C {
        1
    };
};

(* Part 1: Attribute Definitions *)
(* Error 1: Duplicate Definitions *)
Class A {
    x : Int;
    x : Int;
};

(* Error 2a: Overriding Attributes *)
Class B {
    x : Int;
};
Class C inherits B {
    x : Bool;
};

(* Error 2b: Overriding Attributes with Same Type *)
Class D {
    x : Int;
};
Class E inherits D {
    x : Int <- 3;
};

(* Part 2: Formal Parameters of Methods *)
(* Error 3: Same variables twice in method definitions *)
Class F {
    x : Int;
    init(x : Int, y : Bool, x : Bool) : Int {
        {
            x <- x;
        }
    };
};

(*
 *Class G {
 *    x : Int;
 *    init(num1 : Int, num2 : Int) : Int {
 *        {
 *            let x : Int <- 3 in {
 *                let x : Int <- 4 in {
 *                    {
 *                        x <- x + 4;
 *                    }
 *                }
 *            }
 *        }
 *    };
 *};
 *)