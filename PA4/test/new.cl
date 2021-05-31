Class A inherits IO {
    i1 : Int;
    i2 : Int <- 2;

    print(x : Int, y : Int) : Int {
        {
            out_int(i1);
            out_string("\n");
            out_int(i2);
            out_string("\n");
            out_int(x);
            out_string("\n");
            out_int(y);
            out_string("\n");
            x;
        }
    };
};

Class Main inherits IO {
    a : A <- new A;

    main() : Int {
        {
            a.print(11, 22);
            0;
        }
    };
};
