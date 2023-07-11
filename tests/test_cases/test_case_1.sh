#!/bin/bash

tests_passed=0
tests_failed=0


# Case 1
## Class A
### Working               ---TODO---
### Invalid IP Address
./subnet 10.0.0./10 2>&1 | grep -i "Invalid IP address with CIDR mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class A, Invalid IP Address Test Failed
fi

### Invalid Mask
./subnet 10.0.0.0/7 2>&1 | grep -i "Invalid subnet mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class A, Invalid CIDR Mask Test Failed
fi

### Invalid Network Address
./subnet 10.0.0.1/10 2>&1 | grep -i "Invalid network address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class A, Invalid Network Address Test Failed
fi

## Class B
### Working               ---TODO---
### Invalid IP Address
./subnet 129.0.0./20 2>&1 | grep -i "Invalid IP address with CIDR mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class B, Invalid IP Address Test Failed
fi
### Invalid Mask
./subnet 129.0.0.0/15 2>&1 | grep -i "Invalid subnet mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class B, Invalid CIDR Mask Test Failed
fi
### Invalid Network Address
./subnet 129.0.0.1/17 2>&1 | grep -i "Invalid network address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class B, Invalid Network Address Test Failed
fi

## Class C
### Working               ---TODO---
### Invalid IP Address
./subnet 198.0.0./30 2>&1 | grep -i "Invalid IP address with CIDR mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class C, Invalid IP Address Test Failed
fi
### Invalid Mask
./subnet 198.0.0.0/23 2>&1 | grep -i "Invalid subnet mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class C, Invalid CIDR Mask Test Failed
fi
### Invalid Network Address
./subnet 198.0.0.1/25 2>&1 | grep -i "Invalid network address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class C, Invalid Network Address Test Failed
fi

if [ $tests_failed != 0 ]; then
    printf "\n$tests_failed Case 1 Tests Failed\n\n"
fi

exit $tests_passed