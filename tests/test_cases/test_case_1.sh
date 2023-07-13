#!/bin/bash

tests_passed=0
tests_failed=0

test_for_invalid_errors(){
    local args="$1"
    local err_msg="$2"
    local case=$3
    local class=$4

    ./subnet $args 2>&1 | grep -i "$err_msg" > /dev/null
    if [ $? -eq 0 ]; then
        tests_passed=$((tests_passed + 1))
    else
        tests_failed=$((tests_failed + 1))
        echo "Case $case, Class $class, $err_msg Test Failed"
    fi
}

# Case 1
## Class A
test_for_invalid_errors 10.0.0./10 "Invalid IP address" 1 A

### Working               ---TODO---
### Invalid IP Address
./subnet 10.0.0./10 2>&1 | grep -i "Invalid IP address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class A, Invalid IP Address Test Failed
fi

### Invalid Mask
./subnet 10.0.0.0/7 2>&1 | grep -i "Invalid CIDR mask, smaller than classful mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class A, Invalid CIDR Mask Test Failed
fi

### Invalid Mask
./subnet 10.0.0.0/8 2>&1 | grep -i "Invalid CIDR mask, same as classful mask" > /dev/null
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
./subnet 129.0.0./20 2>&1 | grep -i "Invalid IP address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class B, Invalid IP Address Test Failed
fi

### Invalid Mask
./subnet 129.0.0.0/15 2>&1 | grep -i "Invalid CIDR mask, smaller than classful mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class B, Invalid CIDR Mask Test Failed
fi

### Invalid Mask
./subnet 129.0.0.0/16 2>&1 | grep -i "Invalid CIDR mask, same as classful mask" > /dev/null
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
./subnet 198.0.0./30 2>&1 | grep -i "Invalid IP address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class C, Invalid IP Address Test Failed
fi

### Invalid Mask
./subnet 198.0.0.0/23 2>&1 | grep -i "Invalid CIDR mask, smaller than classful mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 1, Class C, Invalid CIDR Mask Test Failed
fi

### Invalid Mask
./subnet 198.0.0.0/24 2>&1 | grep -i "Invalid CIDR mask, same as classful mask" > /dev/null
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