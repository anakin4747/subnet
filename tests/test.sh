#!/bin/bash

tests_passed=0

for case in {1..5}
do
    ./test_cases/test_case_$case.sh
    tests_passed=$((tests_passed + $?))
done

printf "\n$tests_passed Tests Passed\n\n"