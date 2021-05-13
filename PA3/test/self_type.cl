(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

Class A {
	a : SELF_TYPE;
	m() : Int {
		a
	};
};

Class B inherits A {
	b : B;
	c : C;
	m() : Int {
		c1 <- a
	};
};

Class C inherits B {
	x : SELF_TYPE <- 2;
	z : String <- m2(1);
	m2(x : SELF_TYPE) : SELF_TYPE {
		1
	};
};

Class D inherits C {
};
