gtlexer='/usr/class/cs143/bin/lexer'
gtparser='/usr/class/cs143/bin/parser'

cd ..
make clean;
make parser;
cd ./test/

# Unit tests I created
echo ==================Unit tests===================

for file in ./unit/* ; do
    echo Testing "$file"
    diff <($gtlexer $file | ../parser) <($gtlexer $file | $gtparser)
done

# Copied from /usr/class/cs143/examples/
echo =================Program tests=================

for file in ./examples/* ; do
    echo Testing "$file"
    diff <($gtlexer $file | ../parser) <($gtlexer $file | $gtparser)
done
