echo Test 1: Testing false with or which should not print
false
or echo this should print
echo " "

echo Test 2: Testing true with and which should print
true
and echo this should print too
echo " "

echo Test 3: Testing false and and which should not print
false
and echo this should not print
echo " "

echo Test 4: Testing true with or which should not print
true
or echo this should not print
echo " "

echo Test 5: Testing false with or false and another or, which will print
false
or false
or echo previous was false, this will print
echo " "

echo Test 6: Testing previous command of echo with and, which should print
echo success
and echo chained success
echo " "