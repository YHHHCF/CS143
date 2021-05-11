(* Inheritance and self tyep *)
class People {
    name : String;
    age : Int;

    getName() : String {
        name
    };

    getAge() : Int {
        age
    };

    copy() : SELF_TYPE {
        self
    };
};

class Male inherits People {
    isMale : Bool <- true;

    getAge() : Int {
        age + 1
    };
};

class Female inherits People {
    isMale : Bool <- false;
    
    getAge() : Int {
        age - 1
    };
};

(* Error TODO: Leave this to type *)
Class TODO {
    (* var in scope class_type() *)
    method(var : Int) : Int {
        case var of
            (* no error *)
            a : Int => var + 1;
            (* non-Int arguments: Bool + Int *)
            a : Bool => a + 2;
            (* no error *)
            a : String => var + 3;
            (* non-Int arguments: Object + Int *)
            a : Object => a + 4;
        esac
    };
};

Class A {
    method_A() : Int {
        0
    };
};

Class A1 inherits A {
    method_A(x : Int) : Int {
        1
    };
    method_A1() : Int {
        1
    };
};

Class B {
    x : Int;
    y : Bool;

    (* Inferred return type Int of method sum does not conform to declared return type Bool. *)
    sum() : Bool {
        {
            (* non-Int arguments: Int + Bool *)
            x + y;
        }
    };
};

Class C {
    x : Int;
    y : A <- new X;
    z : Bool <- not true;
};

Class D {
    x : Int <- 1;
    method() : Bool {
        {
            x <- "Ha";
        }
    };
};
