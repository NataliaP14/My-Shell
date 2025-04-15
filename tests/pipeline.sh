# ---------------------- Pipe and Command Tests ---------------------------

echo Test 1: Pipe echo hello world to grep hello
echo hello world | grep hello
echo " "

echo Test 2: Pipe echo one two three to wc -w
echo one two three | wc -w
echo " "

echo Test 3: Pipe ls to sort
ls | sort
echo " "

echo Test 4: Pipe echo "this is lowercase" to tr a-z A-Z
echo "this is lowercase" | tr a-z A-Z
echo " "

echo Test 5: Create a file and pipe its content to grep
echo -e "file1.txt\nfile2.txt" > tests/textfiles/filepipeline.txt
cat tests/textfiles/filepipeline.txt | grep "file"
echo " "

echo Test 6: Pipe a wildcard match to wc -l
cat tests/textfiles/file*.txt | wc -l
echo " "

echo Test 7: Pipe false to echo "this should print"
false | echo "this should print"
echo " "
