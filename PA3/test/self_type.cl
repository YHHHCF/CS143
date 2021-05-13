(* Added so semant won't complain *)
Class Main {
    main() : Int {
        1
    };
};

Class A {
	a : SELF_TYPE;
	m() : SELF_TYPE {
		a
	};
};

Class B inherits A {
	b : B;
	c : C;
	m() : SELF_TYPE {
		c <- a
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
