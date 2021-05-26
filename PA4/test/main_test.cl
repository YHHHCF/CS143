Class Main inherits IO {
    i1 : Int;
    i2 : Int <- 1;
    str1 : String;
    str2 : String <- "hello";

    main() : Int {
        {
            out_string("i1: ").out_int(i1).out_string("\n");
            out_string("i2: ").out_int(i2).out_string("\n");
            out_string("str1: ").out_string(str1).out_string("\n");
            out_string("str2: ").out_string(str2).out_string("\n");
            0;
        }
    };
};
