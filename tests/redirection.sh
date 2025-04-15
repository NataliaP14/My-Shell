
echo Test 1: redirecting some text into a text file
echo input hi ive just inputted some text > tests/textfiles/input.txt
echo success
echo " "

echo Test 2: redirecting the out from the previous direction into cat to output the contents of the text file
cat < tests/textfiles/input.txt
echo " "

echo Test 3: Demonstrating multiple redirections: Redirecting the content of one file to another file and also outputting to `cat`
cat < tests/textfiles/input.txt > tests/textfiles/output.txt
echo " "

echo Test 4: Displaying the contents of `output.txt` using `cat`
cat tests/textfiles/output.txt
echo " "

echo Test 5: Writing new data to `another.txt` using output redirection
echo new data > tests/textfiles/another.txt
echo " "

echo Test 6: Displaying the contents of `another.txt` to verify the newly written data
cat tests/textfiles/another.txt
echo " "
