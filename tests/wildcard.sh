echo Test 1: testing the wildcard for getting all of the c files in the current 
echo *.c
echo " "

echo Test 2: testing the wildcard functionality while redirecting
echo "test1" > tests/textfiles/test1.txt
echo "test2" > tests/textfiles/test2.txt
cat tests/textfiles/test*.txt
echo " " 

echo Test 3: using wildcard to match sh files inside of the test directory
echo tests/*.sh
echo " "

echo Test 4: testing redirction with multiple sub directories and using the wildcard
mkdir -p tests/wilddir
echo "wild1InWildDir" > tests/wilddir/wild1.txt
echo "wild2InWildDir" > tests/wilddir/wild2.txt
cat tests/wilddir/wild*.txt
echo " "

