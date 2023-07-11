#!/bin/bash

tests_passed=0
tests_failed=0

# Case 4
#   Class A
#     Working
#     Invalid IP Address
#     Invalid Mask
#     Invalid Network Address
#     Invalid Max Expression

#   Class B
#     Working
#     Invalid IP Address
#     Invalid Mask
#     Invalid Network Address
#     Invalid Max Expression

#   Class C
#     Working
#     Invalid IP Address
#     Invalid Mask
#     Invalid Network Address
#     Invalid Max Expression

if [ $tests_failed != 0 ]; then
    printf "\n$tests_failed Case 4 Tests Failed\n\n"
fi

exit $tests_passed