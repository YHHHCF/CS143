(*
 *  Test bad examples of expressions part 1
 *)

(* error: assign operator -> incorrect *)
class A {
    x : Int;

    f1() : Int {
        y -> x
    };
};

(* error: dispatch symbols incorrect *)
class A {
    x : Int;
    y : Int;

    f1() : Int {
        a*A.y(1, 2, 3)
    };

    f2() : Int {
        a*y(1, 2, 3)
    };

    f3() : Int {
        y1, 2, 3)
    };
    f4() : Int {
        a.b.c(1, 2, 3)
    };
    f5() : Int {
        a@B@C(1, 2, 3)
    };
};

(* error: misspell fi to fii *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        if x then y else z fii
    };
};

(* error: misspell while to whhile *)
class A {
    x : Int;
    y : Int;

    f1() : Int {
        whhile x loop y pool
    };
};

(* error: block missing semicolon and closing brace *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        {
            x
            y;
            z;
    };
};

(* error: let missing colon, comma, incorrect init, missing IN keyword *)
class A {
    f1() : Int {
        let x Int <- 1 in a < b
    };
    f2() : Int {
        let x : Int <- 1 y : Int <- 2 in a < b
    };
    f3() : Int {
        let x : Int <- A, y : Int <- 2, z : Int <- B in a < b
    };
    f4() : Int {
        let x : Int <- 1, y : Int <- 2, z : Int <- 3 a < b
    };
};
