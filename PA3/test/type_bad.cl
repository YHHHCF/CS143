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