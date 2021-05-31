Class Main inherits IO {
    i1 : Int;
    i2 : Int <- 1;
    str1 : String;
    str2 : String <- "hello";
    objA1 : A <- new A;
    objB2 : B;

    main() : Int {
        {
            out_int(if i1 < i2 then 1 else 0 fi);
            out_int(if i2 <= 5 then 1 else 0 fi);
            while i2 <= 0 loop
                {
                    out_int(i2);
                    i2 <- i2 + 1;
                }
            pool;
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