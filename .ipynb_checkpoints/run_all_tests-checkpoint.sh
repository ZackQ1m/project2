#!/bin/bash

for i in {01..05}; do
    ./main < test_cases/test$i.in > myoutput$i.txt 2>/dev/null
    echo "Comparing test$i..."
    diff myoutput$i.txt test_cases/test$i.out
done
