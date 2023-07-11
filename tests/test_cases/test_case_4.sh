#!/bin/bash

tests_passed=0
tests_failed=0

# Case 4
## Class A
### Working               ---TODO---
### Invalid IP Address
./subnet 10.0.0. 100h 2>&1 | grep -i "Invalid IP address" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 4, Class A, Invalid IP Address Test Failed
fi

### Invalid Subnet Mask From too Many Hosts
./subnet 10.0.0.0 16777215h 2>&1 | grep -i "Invalid subnet mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 4, Class A, Invalid Mask From Too Many Hosts Test Failed 
fi

### Invalid Subnet Mask From too Many Subnets
./subnet 10.0.0.0 16777215h 2>&1 | grep -i "Invalid subnet mask" > /dev/null
if [ $? -eq 0 ]; then
    tests_passed=$((tests_passed + 1))
else
    tests_failed=$((tests_failed + 1))
    echo Case 4, Class A, Invalid Mask From Too Many Hosts Test Failed 
fi

# ### Invalid Network Address
# ./subnet 10.0.0.1/9 255.192.0.0 2>&1 | grep -i "Invalid network address"  > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class A, Invalid Network Address Test Failed
# fi

# ### Invalid Max Expression

# ## Class B
# ### Working               ---TODO---
# ### Invalid IP Address
# ./subnet 129.0.0./17 255.255.224.0 2>&1 | grep -i "Invalid IP address" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class B, Invalid IP Address Test Failed
# fi

# ### Invalid Dotted Decimal Subnet Mask
# ./subnet 129.0.0.0/17 255.255.224. 2>&1 | grep -i "Invalid dotted decimal mask" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class B, Invalid Dotted Decimal Mask Test Failed
# fi

# ### Invalid Mask
# ./subnet 129.0.0.0/17 255.255.0.0 2>&1 | grep -i "Invalid subnet mask" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class B, Invalid Mask Test Failed
# fi

# ### Invalid Network Address
# ./subnet 129.0.0.1/17 255.255.224.0 2>&1 | grep -i "Invalid network address" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class B, Invalid Network Address Test Failed
# fi

# ### Invalid Max Expression

# ## Class C
# ### Working               ---TODO---
# ### Invalid IP Address
# ./subnet 198.0.0./25 255.255.255.192 2>&1 | grep -i "Invalid IP address" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class C, Invalid IP Address Test Failed
# fi

# ### Invalid Dotted Decimal Subnet Mask
# ./subnet 198.0.0.0/25 255.255.255. 2>&1 | grep -i "Invalid dotted decimal mask" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class C, Invalid Dotted Decimal Mask Test Failed
# fi

# ### Invalid Mask
# ./subnet 198.0.0.0/25 255.255.254.0 2>&1 | grep -i "Invalid subnet mask" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class C, Invalid Mask Test Failed
# fi

# ### Invalid Network Address
# ./subnet 198.0.0.1/25 255.255.255.192 2>&1 | grep -i "Invalid network address" > /dev/null
# if [ $? -eq 0 ]; then
#     tests_passed=$((tests_passed + 1))
# else
#     tests_failed=$((tests_failed + 1))
#     echo Case 4, Class C, Invalid Network Address Test Failed
# fi

# ### Invalid Max Expression

if [ $tests_failed != 0 ]; then
    printf "\n$tests_failed Case 4 Tests Failed\n\n"
fi

exit $tests_passed