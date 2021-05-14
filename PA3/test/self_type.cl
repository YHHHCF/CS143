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
	self : String <- m2(1);
	self(x : SELF_TYPE) : SELF_TYPE {
		x@SELF_TYPE.m()
	};
};

Class D {
	a : SELF_TYPE;

	m(self : Int) : Int {
		isvoid (self)
	};
};
