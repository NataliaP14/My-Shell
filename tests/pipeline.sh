# ---------------------- Pipe and Command Tests ---------------------------

echo Test 1: Pipe echo hello world to grep hello
echo hello world | grep hello
# Expected: hello world
# Explanation: 'echo hello world' prints "hello world", and 'grep hello' filters for lines containing 'hello', which in this case, matches the entire output.
echo "-------------------------"

echo Test 2: Pipe echo one two three to wc -w
echo one two three | wc -w
# Expected: 3
# Explanation: 'echo one two three' prints "one two three", and 'wc -w' counts the words, which are 3 in this case.
echo "-------------------------"

echo Test 3: Pipe ls to sort
ls | sort
# Expected: Sorted list of files and directories in the current directory
# Explanation: 'ls' lists files and directories, and 'sort' sorts them alphabetically.
echo "-------------------------"

echo Test 4: Pipe echo "this is lowercase" to tr a-z A-Z
echo "this is lowercase" | tr a-z A-Z
# Expected: THIS IS LOWERCASE
# Explanation: 'echo "this is lowercase"' prints the string, and 'tr a-z A-Z' translates lowercase letters to uppercase.
echo "-------------------------"

echo Test 5: Create a file and pipe its content to grep
echo -e "file1.txt\nfile2.txt" > tests/textfiles/filepipeline.txt
cat tests/textfiles/filepipeline.txt | grep "file"
# Expected: file1.txt, file2.txt
# Explanation: 'echo' creates a file with two lines "file1.txt" and "file2.txt". 'cat' outputs the content, and 'grep' filters lines containing "file".
echo "-------------------------"

echo Test 6: Pipe a wildcard match to wc -l
cat tests/textfiles/file*.txt | wc -l
# Expected: Number of lines in the file(s) matching file*.txt
# Explanation: 'cat tests/textfiles/file*.txt' matches all text files in the directory and 'wc -l' counts the number of lines.
echo "-------------------------"

echo Test 7: Pipe false to echo "this should print"
false | echo "this should print"
# Expected: "this should print"
# Explanation: 'false' returns a non-zero exit status, but since 'echo' does not depend on it, "this should print" will be printed.
echo "-------------------------"
