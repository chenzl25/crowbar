rm dfa.result grep.result
cat test/test.txt | ./dfa \[0-9\]\{8,9\} >> dfa.result
cat test/test.txt | grep  -E  "[0-9]{8,9}" >> grep.result
cat test/test.txt | ./dfa Dig >> dfa.result
cat test/test.txt | grep  -E  "Dig" >> grep.result
