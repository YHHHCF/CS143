Class Main inherits IO {
    i1 : Int;
    i2 : Int <- 1;

    main() : Int {
        {
            out_int(5+2);
            out_int(10-13);
            out_int(4*2);
            out_int(12/6);
            0;
        }
    };
};