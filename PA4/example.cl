
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main {
  main():Int { 0 };
};

class A {
	x : Int <- 3;
};

class B inherits A {};

class C {
	a : A <- new A;
	c : String <- "test";
	mC() : Int {
		0
	};
};

class D inherits C {
	b : B <- new B;
	mC() : Int {
		1
	};
};
