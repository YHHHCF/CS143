class A {
    x : Int <- 0;
    let_conflict(): Int {
        let x : Int <- 1, y : Int <- 2 in 1 < 2
    };
};