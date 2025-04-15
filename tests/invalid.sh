# invalid.sh

# ------------------------ Invalid Built-in Command Tests ------------------------

# 1. `cd` command with no argument (missing directory)
cd
# Expected: Error: missing argument for 'cd' or Error: No such file or directory

# 2. `cd` command with non-existent directory
cd /nonexistent_directory
# Expected: Error: No such file or directory

# 3. `pwd` with an unexpected argument (should not take any)
pwd /home/user
# Expected: Error: `pwd` does not take arguments

# 4. `exit` with an invalid argument (not a valid exit code)
exit foo
# Expected: Error: invalid argument to exit (should ideally exit with status 1)

# 5. `which` with an unknown command
which unknown_command
# Expected: unknown_command not found


# ---------------------- Invalid Redirection and Pipes -------------------------

# 6. Missing filename after output redirection
echo "Hello" > 
# Expected: Error: missing or invalid filename after '>'

# 7. Missing filename after input redirection
cat < 
# Expected: Error: missing or invalid filename after '<'

# 8. Multiple input redirections
cat < input.txt < output.txt
# Expected: Error: multiple input redirections

# 9. Multiple output redirections
echo "Hello" > output.txt > another_output.txt
# Expected: Error: multiple output redirections

# 10. Trying to redirect to a non-existent directory/file
echo "Hello" > /nonexistent_path/output.txt
# Expected: Error: couldn't open output file

# 11. Missing command before pipe
| echo "This shouldn't work"
# Expected: Error: missing command before pipe

# 12. Missing command after pipe
echo "Hello" |
# Expected: Error: missing command after pipe

# 13. Multiple pipes in a single command
echo "Hello" | echo "World" | echo "!"
# Expected: Error: only one pipe allowed


# ------------------------ Invalid Wildcard Tests -----------------------------

# 14. Wildcard not matching any files
echo *.nonexistent
# Expected: *.nonexistent (no files match the pattern)

# 15. Wildcard that should match files, but it's not matching due to syntax
echo *.*
# Expected: List of files in the current directory if any match, otherwise no output


# ---------------------------- Other Invalid Command Tests ----------------------

# 16. Executing a non-existent command
unknown_command
# Expected: Error: command not found
