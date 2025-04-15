# invalid.sh

# ------------------------ Invalid Built-in Command Tests ------------------------

echo Test 1: `cd` command with no argument (missing directory)
cd
echo " "

echo Test 2: `cd` command with non-existent directory
cd /nonexistent_directory
echo " "

echo Test 3: `pwd` with an unexpected argument (should not take any)
pwd /home/user
echo " "

echo Test 4: . `which` with an unknown command
which unknown_command
echo " "


# ---------------------- Invalid Redirection and Pipes -------------------------

echo Test 5: Missing filename after output redirection
echo "Hello" > 
echo " "

echo Test 6: Missing filename after input redirection
cat < 
echo " "

echo Test 7: Multiple input redirections
cat < input.txt < output.txt
echo " "

echo Test 8: Multiple output redirections
echo "Hello" > output.txt > another_output.txt
echo " "

echo Test 9: Trying to redirect to a non-existent directory/file
echo "Hello" > /nonexistent_path/output.txt
echo " "

echo Test 10: Missing command before pipe
| echo "This shouldn't work"
echo " "

echo Test 11: Missing command after pipe
echo "Hello" |
echo " "

echo Test 12: Multiple pipes in a single command
echo "Hello" | echo "World" | echo "!"
echo " "


# ------------------------ Invalid Wildcard Tests -----------------------------

echo Test 13: Wildcard not matching any files, which just prints the echo statement
echo *.nothing
echo " "



