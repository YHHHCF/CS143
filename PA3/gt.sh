#!/bin/csh -f
./lexer $* | ./parser | /usr/class/cs143/bin/semant
