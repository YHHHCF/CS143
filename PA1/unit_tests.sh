make clean;
make lexer;

echo Testing integers
diff <(./lexer ./test/test_integers.cl) <(/usr/class/cs143/bin/lexer ./test/test_integers.cl)

echo Testing identifiers
diff <(./lexer ./test/test_identifiers.cl) <(/usr/class/cs143/bin/lexer ./test/test_identifiers.cl)

echo Testing special notations
diff <(./lexer ./test/test_special_notations.cl) <(/usr/class/cs143/bin/lexer ./test/test_special_notations.cl)

echo Testing keywords
diff <(./lexer ./test/test_keywords.cl) <(/usr/class/cs143/bin/lexer ./test/test_keywords.cl)

echo Testing white spaces
diff <(./lexer ./test/test_white_space.cl) <(/usr/class/cs143/bin/lexer ./test/test_white_space.cl)

echo Testing comments
diff <(./lexer ./test/test_comments.cl) <(/usr/class/cs143/bin/lexer ./test/test_comments.cl)

echo Testing strings
diff <(./lexer ./test/test_strings.cl) <(/usr/class/cs143/bin/lexer ./test/test_strings.cl)

