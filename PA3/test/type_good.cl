(* Good examples for type checking *)

Class Main {
    person1 : People <- new People;
    person2 : People <- person1.copy();

    man1 : Male <- new Male;
    man2 : Male <- man1.copy();

    main() : Int {
        1
    };
};

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
