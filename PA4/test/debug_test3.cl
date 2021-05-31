Class Main inherits IO {
    objA1 : A <- new A;

    main() : Int {
        {
            0;
        }
    };
};

Class A {
    tf : Bool <- true;
    foo() : Int {
        if tf then 1 else 0 fi
    };
};