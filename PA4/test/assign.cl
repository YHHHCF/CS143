Class Main inherits IO {
    a : Int;
    set_a(x : Int) : Int {
        a <- x
    };

    main() : Int {
        {
            set_a(66);
            out_int(a);
            out_string("\n");
            0;
        }
    };
};
