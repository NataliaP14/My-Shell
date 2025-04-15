echo hello world | grep hello

echo one two three | wc -w

ls | sort

echo "this is lowercase" | tr a-z A-Z

echo -e "file1.txt\nfile2.txt" > tests/textfiles/filepipeline.txt
cat tests/textfiles/filepipeline.txt | grep "file"

cat tests/textfiles/file*.txt | wc -l

false | echo "this should print"


