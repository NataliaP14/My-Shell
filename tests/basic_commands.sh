echo "Test 1: Testing the 'echo' command"
echo hello world
# Expected output: "hello world"
echo "-------------------------"

echo "Test 2: Testing the 'ls' command"
ls
# Expected output: List of files and directories in the current working directory
echo "-------------------------"

echo "Test 3: Testing the 'cd' command (valid directory) and printing the directory"
cd tests
pwd
# Expected output: Path to the 'tests' directory (should change current directory to 'tests')
echo "-------------------------"

echo "Test 4: Testing the 'cd ../..' command and printing the directory"
cd ../..
pwd
# Expected output: Path to the parent directory (should go up two levels)
echo "-------------------------"

echo "Test 5: Testing the 'cd P3' command (valid directory change), and printing the directory"
cd P3
pwd
# Expected output: Path to the 'P3' directory
echo "-------------------------"

echo "Test 6: Testing the 'date' command"
date
# Expected output: Current date and time
echo "-------------------------"

echo "Test 7: Testing the 'which' command for 'echo'"
which echo
# Expected output: Path to 'echo' command, something like: "/usr/bin/echo"
echo "-------------------------"

echo "Test 8: Testing the 'which' command for 'ls'"
which ls
# Expected output: Path to 'ls' command, something like: "/usr/bin/ls"
echo "-------------------------"

echo "Test 9: Testing the 'which' command for 'cd', which prints nothing"
which cd
# Expected output: No output (as 'cd' is a built-in command, it shouldn't be found as an executable)
echo "-------------------------"

echo "Test 10: Testing the 'which' command for a non-existing command 'no_command'"
which no_command
# Expected output: No output (should return with no result or an error indicating the command is not found)
echo "-------------------------"

echo "Test 11: Testing the 'whoami' command"
whoami
# Expected output: The username of the current user
echo "-------------------------"

echo "Test 12: Testing the 'die hello' command where the program terminates with failure and arguments with die are printed"
die hello 
# Expected output: "hello" will be printed, then mysh should terminate with an error code (exit status 1)
echo "-------------------------"

