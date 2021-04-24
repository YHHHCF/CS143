(*
 *  Test bad examples of feature
 *)

(* error: X1 is not an ObjectID *)
CLASS A {
    X1 : Int <- 5;
    f1() : Int {
        6
    };
};

(* error: F1 is not an ObjectID *)
CLASS A {
    x1 : Int <- 5;
    F1() : Int {
        6
    };
};

(* error: both 'int's are not TypeID *)
CLASS A {
    x1 : int <- 5;
    f1() : int {
        6
    };
};

(* error: missing colons *)
CLASS A {
    x1 Int <- 5;
    f1() Int {
        6
    };
};

(* error: missing assign *)
CLASS A {
    x1 : Int 5;
    f1() : Int {
        6
    };
};

(* error: missing types *)
CLASS A {
    x1 : 5;
    f1() : {
        6
    };
};

(* error: missing the value to assign *)
CLASS A {
    x1 : Int <- ;
    f1() : Int {
        6
    };
};

(* error: has empty method *)
CLASS A {
	x1 Int <- 5;
    f1() : Int { };
};

(* error: missing semi colons *)
CLASS A {
    x1 : Int <- 5
    f1() : Int {
        6
    }
};
