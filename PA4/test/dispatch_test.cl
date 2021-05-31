Class A inherits IO {
    i1 : Int;
    i2 : Int <- 2;

    print(x : Int, y : Int) : Int {
        {
            out_int(x);
            out_int(y);
            x;
        }
    };
};

Class B inherits A {
    str1 : String;
    str2 : String <- "hello\n";

    print(x : Int, y : Int) : Int {
        {
            out_int(x);
            out_int(y);
            out_string(str1);
            out_string(str2);
            y;
        }
    };
};

Class Main inherits B {
    main() : Int {
        {
            self@A.print(11, 22);
            self@B.print(33, 44);
            print(55, 66);
            0;
        }
    };
};
