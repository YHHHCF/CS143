(* Good Tests *)

(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

Class A {};
Class B {};
Class C {};
Class D {};
Class E {};

Class G1 {
    (* x, y have scope G1 *)
    x : Bool <- false;
    y : Int <- 1;

    (* num1, num2 have scope init() *)
    init(num1 : Int, num2 : Int) : Int {
        (* x has scope let_expr *)
        let x : Int <- 3 in {
            (*
             * x in let_expr
             * num1, num2 in init()
             * y in G1
             *)
            x <- num1 + num2 + y;
        }
    };
};

Class G2 {
    (* var in scope class_type() *)
    class_type(var : A) : Int {
        case var of
            (* this a has scope of the following block_expr *)
            a : A => {
                a;
                1;
            };
            (* each a has scope of each branch *)
            a : B => {
                a;
                2;
            };
            a : C => 3;
            a : D => 4;
            a : E => 5;
        esac
    };
};

Class G3 {
    (* x has scope G3 *)
    x : Bool <- false;
    init(num1 : Int, num2 : Int) : Int {
        (* x has scope outer let_expr *)
        let x : String <- "3" in {
            (* x has scope inner let_expr *)
            let x : Int <- 4 in {
                x <- x + 4;
            };
        }
    };
};

Class G4 {
    x : Bool <- false;
    init(num1 : Int, num2 : Int) : Int {
        (* each x has a new scope *)
        let x : String <- "3", x : Int <- 4, x : Object, x : Int <- 6 in {
            (* binded with x : Int <- 6 *)
            x + 1;
        }
    };
};

Class H {
    (* x has scope H *)
    x : Int <- 1;
    get_x() : Int {
        x
    };
};

Class H1 inherits H {
    (* x has scope H1 *)
    get_x() : Int {
        x
    };
};

Class H2 inherits H {
    (* x has scope H2 *)
    get_x() : Int {
        x
    };
};

Class I {
    (* x has scope I, can use before declare *)
    get_x() : Int {
        x
    };
    x : Int <- 1;
};
