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
    x : Int;
    init(num1 : Int, num2 : Int) : Int {
        let x : Int <- 3 in {
            x <- num1 + num2;
        }
    };
};

Class G2 {
    class_type(var : A) : Int {
        case var of
            a : A => 1;
            a : B => 2;
            a : C => 3;
            a : D => 4;
            a : E => 5;
        esac
    };
};