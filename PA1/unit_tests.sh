make clean; make lexer;

echo Start testing: test_comments.cl
diff <(./lexer ./test/test_comments.cl) <(/usr/class/cs143/bin/lexer ./test/test_comments.cl)

echo Start testing: test_keywords.cl
diff <(./lexer ./test/test_keywords.cl) <(/usr/class/cs143/bin/lexer ./test/test_keywords.cl)

echo Start testing: test_strings.cl
diff <(./lexer ./test/test_strings.cl) <(/usr/class/cs143/bin/lexer ./test/test_strings.cl)

