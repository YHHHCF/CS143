gtlexer='/usr/class/cs143/bin/lexer'

cd ..
make clean;
make lexer;
cd ./test/

# Unit tests I created
echo ==================Unit tests===================

for file in ./unit/* ; do
	echo Testing "$file"
	diff <(../lexer $file) <($gtlexer $file)
done

# Copied from /usr/class/cs143/examples/
echo =================Program tests=================

for file in ./examples/* ; do
	echo Testing "$file"
	diff <(../lexer $file) <($gtlexer $file)
done
