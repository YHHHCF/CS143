(*
 *  Test bad examples of expressions part 2
 *)

(* error: case missing DARROW, semicolon
 * Note: my output is different from sample parser output
 * But it can catch the error correctly
 *)
class A {
    x : Int;
    y : Int;
    z : Int;

    f1() : Int {
        case x of y : Int z; y1 : Int => z1; esac
    };

    f2() : Int {
        case x of y : Int => z y1 : Int => z1; esac
    };
};

(* error: new misspelling and a is not typeID *)
class A {
    f1() : Int {
        neew A
    };
    f2() : Int {
        new a
    };
};

(* error: A is not an expression *)
class A {
    f1() : Int {
        isvoid A
    };
};

(* error: misuse of nonassoc ops and unary ops *)
class A {
    f1() : Int {
        {
            x <= y <= z;
            x < y < z;
            x = y = z;
            x not y;
            x isvoid y;
            x ~ y;
        }
    };
}

(* error: contains unmatched brace *)
class A {
    f1() : Int {
        (x
    };
    f2() : Int {
        ((x)
    };
    f3() : Int {
        (x)))
    };
};

(* typeID, float, 'string', True *)
class A {
    f1() : Int {
        {
            X;
            1.1;
            'hello';
            True;
        }
    };
};
