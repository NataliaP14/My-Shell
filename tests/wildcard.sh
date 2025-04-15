echo *.c

echo "test1" > tests/textfiles/test1.txt
echo "test2" > tests/textfiles/test2.txt
cat tests/textfiles/test*.txt

echo "hidden" > tests/.hidden.txt
echo tests/*.sh

mkdir -p tests/wilddir
echo "wild1InWildDir" > tests/wilddir/wild1.txt
echo "wild2InWildDir" > tests/wilddir/wild2.txt
cat tests/wilddir/wild*.txt

