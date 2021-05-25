
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main {
  main():Int { 0 };
};

class A {
	x : Int <- 1;
	y : Int <- 2;
	a() : Int {
		0
	};
};

class B inherits A {
	z : Int <- 3;
};

class C inherits B {
	w : Int <- 4;
};

class D {
	a : A <- new A;
	c : String <- "test";
	mC() : Int {
		0
	};
};

class E inherits D {
	b : B <- new B;
	mC() : Int {
		1
	};
};
