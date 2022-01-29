#!/bin/bash
#for filename in `find . | egrep '\.cpp'`;
#do
#  gcov -n -o . $filename > /dev/null;
#done

gcovr -r .
gcovr -r . --html -o coverage.html
