Class Main inherits IO {
    a : Int;
    set_a(x : Int) : Int {
        let y : Int <- x in y
    };

    main() : Int {
        {
            out_int(set_a(99));
            out_string("\n");
            0;
        }
    };
};
