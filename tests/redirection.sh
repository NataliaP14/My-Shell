
echo input hi ive just inputted some text > tests/textfiles/input.txt
echo " "


cat < tests/textfiles/input.txt
echo " "


cat < tests/textfiles/input.txt > tests/textfiles/output.txt
echo " "


cat tests/textfiles/output.txt
echo " "


echo new data > tests/textfiles/another.txt
echo " "


cat tests/textfiles/another.txt
echo " "
