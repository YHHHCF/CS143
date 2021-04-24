(*
 *  Test good examples of class
 *)

(* no inheritance, no feature *)
cLaSs A {};

(* has inheritance, no feature *)
Class B inherits A {};

(* no inheritance, has feature *)
CLASS A1 {
    xa1 : Int <- 0;
    fa1() : Int {
        1
    };
};

(* has inheritance, has feature *)
class B1 inherits A1 {
    xb1 : Int <- 2;
    fb1() : Int {
        3
    };
};
