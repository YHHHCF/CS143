(* This is the first line
 This is the second line
 This is the last line *)

=>

(* This is a comment
   (* This is a nested comment *)
*)

=>

(* This is a comment
   (* This a depth 1 nested comment
      (* This is a depth 2 nested comment
         (* This is a depth 3 nested comment *)
       *)
    *)
*)
=>

-- This is a single line comment

=>

-- (* This is a nested comment in single line *)

=>

-- (* This is a valid comment

=>

-- -- This is another valid comment

=>

(* A comment with   null character is ok *)

=>

(* A comment with  EOF in the middle is ok *)

=>

-- A comment with EOF at the end is not ok