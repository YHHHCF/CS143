(*
 *  Test bad examples of class
 *)

(* error:  a is not a type identifier *)
Class a inherits B {
};

(* error:  b is not a type identifier *)
Class A inherits b {
};

(* error:  keyword inherits is misspelled *)
Class A inherts B {
};

(* error:  opening brace is missing *)
Class A inherits B
};

(* error:  semicolon is missing *)
Class A inherits B {
}

(* error:  closing brace is missing *)
Class E inherits A {
;

