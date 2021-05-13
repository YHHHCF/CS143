(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

Class A inherits SELF_TYPE {

};

Class SELF_TYPE inherits A {

};

Class B inherits Int {

};

Class C inherits BA {

};

Class String {

};
Class IO {

};
Class Object {

};
Class Int {

};
Class Bool {

};
Class SELF_TYPE {

};
