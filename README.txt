Natalia Peguero np1087
Anna Koganov aee71

Testing plan
Our testing plan consisted of two parts: One test for the batch mode and another test for the interactive mode. The batch mode test is a file containing all of the shell commands with either good or bad tokens following the commands, as well as invalid commands. We had several smaller scripts to implement these tests. We implemented several sh files for the test cases:

Basic_commands.sh:
 - Contained the built in commands we implemented: cd, pwd, die and which. With some external commands: date, echo, and whoami. 
- The echo command is always called followed by the text to be outputted, and ls is always called by itself. 
- The test program also calls cd where we tested several functionalities such as: going into subdirectories and moving back directories using ‘cd ..’ and cd ../..’
- Every time this command is called, it is followed by the pwd command to demonstrate that the working directory is appropriately switched and the pwd command is functional. 
- The which command is also tested several times, and we only expect an output when a valid, non built-in command is given as an argument. 
- The die command we tested while including an argument with it, to make sure the program exits as exit failure, while printing the argument
- We also tested some other external commands to make sure they were correctly outputting the data, such as whoami, date,

conditionals.sh
- This script will show that the echo command only runs when the boolean logic applies (using true, false for the previous statement, with conditions: and , and or) such as:
- Testing false with or echo: which correctly prints out the statement
- Testing true with and echo: which correctly prints out the statement
- Testing false with and echo: which correctly does not print out the statement
- Testing true with or echo: which correctly does not print out the statement
- Testing false with or false and another or echo: which correctly prints out the statement
- Testing echo with and echo: which correctly prints out the statement

invalid.sh 
- For the first part of this test, the file calls the built-in commands with invalid arguments. cd is called with no arguments and a nonexistent directory, and we expect the appropriate error message to print. We also call pwd with an argument when it expects none, and which with an unknown command. 
- The next part of this test was invalid redirection and pipe commands. These tests demonstrate that the redirect command only works when there is only one valid filename for the indicated input or output. They also demonstrate that the pipe commands only work when there is one pipe and two valid commands on either side. 
- The next part of the test was the wildcard test, which shows that the wildcard commands will only work if there are files that match the pattern and have the correct syntax. 
- Lastly, the script attempts to execute an unknown command and prints the appropriate error message.

pipeline.sh
Demonstrates how commands can be piped together. For example, ls | sort would output a sorted list of files and directories in the current directory, as ls outputs the files and directories and sort sorts them. We implemented several tests such as: 
The first test pipes echo output to grep to search for a word.
The second test pipes echo output to wc -w to count words.
The third test pipes ls output to sort to sort the listed files.
The fourth test pipes echo output to tr to convert lowercase to uppercase.
The fifth test pipes file content to grep to search for a pattern.
The sixth test pipes wildcard file match to wc -l to count matching lines.
The seventh test pipes false to echo to demonstrate no output from false

redirection.sh
demonstrates how text and program output can be redirected to given files, and displays the contents of the files to prove success. 
test 1: redirects text to a file
test 2: takes the output from a previous command and redirects it into cat in order to output the contents of the text file
test 3: multiple redirections- redirects file content to 	another file and also cat
test 4: displays the content of output.txt using cat
test 5: writes text input to a file using output redirection
test 6: displays the contents of the output file from the previous command to demonstrate success.

wildcard.sh
first test uses the wildcard to retrieve all files in the current directory with the .c extension
second test redirects input into 2 test text files, which then uses the wildcard command to get all of the test text files that were just created.
third test uses the wildcard character to match sh files in a given directory
fourth test tests the redirection command with multiple sub directories using the wildcard character.

Once all of the tests are complete, the program outputs a statement saying so.

Run_all_test.sh -> COMMAND -> ./mysh run_all_test.sh
This sh file runs all of the previous sh file test cases all at once

We also tested the interactive mode by running the program in the terminal and manually inputting the commands and making sure we get the appropriate output. We decided it was important to check all of the commands with incomplete, incorrect, and missing arguments to make sure that user input cannot cause unexpected behavior in any circumstance. It was also important to test that the program creates correct output when valid commands are given.
