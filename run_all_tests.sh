#!/bin/bash

all_passed=true

for i in {01..06}; do
    ./main < test_cases/test$i.in > myoutput$i.txt 2>/dev/null
    echo "Comparing test$i..."
    if diff myoutput$i.txt test_cases/test$i.out; then
        echo "Test$i passed!"
    else
        echo "Test$i failed!"
        all_passed=false
    fi
    echo "-------------------------------------"
done

if $all_passed; then
    echo "All tests passed!"
else
    echo "Some tests failed."
fi
