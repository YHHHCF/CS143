(*
 *  Test good examples of expressions
 *)

(* assign *)
class A {
    x : Int;

    f1() : Int {
        y <- x
    };
};

(* dispatch *)
class A {
    x : Int;
    y : Int;

    f1() : Int {
        a@A.y(1, 2, 3)
    };

    f2() : Int {
        a.y(1, 2, 3)
    };

    f3() : Int {
        y(1, 2, 3)
    };
};

(* if *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        if x then y else z fi
    };
};

(* while *)
class A {
    x : Int;
    y : Int;

    f1() : Int {
        while x loop y pool
    };
};

(* block *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        {
            x;
            y;
            z;
        }
    };
};

(* let *)
class A {
    f1() : Int {
        let x : Int <- 1 in a < b
    };
    f2() : Int {
        let x : Int <- 1, y : Int <- 2 in a < b
    };
    f3() : Int {
        let x : Int <- 1, y : Int <- 2, z : Int <- 3 in a < b
    };
    f4() : Int {
        (let x : Int <- 1 in a) < b
    };
    f5() : Int {
        (let x : Int <- 1, y : Int <- 2 in a) < b
    };
    f6() : Int {
        (let x : Int <- 1, y : Int <- 2, z : Int <- 3 in a) < b
    };
};

(* case *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        case x of y : Int => z; esac
    };
    f2() : Int {
        case x of y : Int => z; y1 : Int => z1; esac
    };
};

(* new *)
class A {
    f1() : Int {
        new A
    };
};

(* isvoid *)
class A {
    f1() : Int {
        isvoid 1
    };
};

(* operations *)
class A {
    (* + - * / *)
    f1() : Int {
        {
            1 + 2 - 3 * 4 / 5;
            1 + 2 / 3 - 4 * 5;
            1 * 2 * 3 * 4;
            1 - 2 - 3 - 4;
            1 + 2 + 3 + 4;
            1 / 2 / 3 / 4;
            1 / (2 / 3) / 4;
        }
    };

    (* ~ < <= = not *)
    f2() : Int {
        {
            ~x;
            ~~x;
            not x;
            not not x;
            x <= y;
            (x <= y) <= z;
            x <= (y <= z);
            x < y;
            (x < y) < z;
            x < (y < z);
            x = y;
            (x = y) = z;
            x = (y = z);
            ~w = not ((x < y) <= z);
        }
    };
};

(* () *)
class A {
    f1() : Int {
        (x)
    };
    f2() : Int {
        ((x))
    };
};

(* ID, integer, string, true, false *)
class A {
    f1() : Int {
        {
            x;
            1;
            "Hello";
            true;
            false;
        }
    };
};
