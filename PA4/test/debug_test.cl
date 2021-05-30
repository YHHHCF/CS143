Class Main inherits IO {
    i1 : Int;
    i2 : Int <- 1;
    str1 : String;
    str2 : String <- "hello";
    objB : B;

    main() : Int {
        {
            out_int(if i2=0 then 1 else 0 fi);
            out_int(i1);
            out_int(i2);
            out_string(str1);
            out_string(str2);
            out_int(12/4);
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

Class B {
    counter : Int <- 0;
    fooB(b : Bool) : Int {
        {
        while b loop
            {
                counter <- counter + 1;
                b <- false;
            }
        pool;
        counter;
        }
    };
};