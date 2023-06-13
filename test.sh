#!/bin/bash
g++ A1.cpp
for (( j=1; j<10; j++))
do
echo "test $j diff:"
./a.out < tests/$j/$j.in > test.out
diff test.out tests/$j/$j.out
printf "\n"
done
rm test.out
rm a.out
