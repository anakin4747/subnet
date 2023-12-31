# Subnetting Program

To better understand subnetting concepts I decided to write my own command line tool for calculating subnets. 

This tool can:

    - Calculate number of subnets possible given an IP address and a subnet mask
    - Calculate number of hosts per subnet
    - Show subnet address, broadcast address, and address ranges for each subnet
    - Interpret subnet masks in several forms (Classful, CIDR, Dotted Decimal)
    - Calculate subnet masks for given host or subnet requirements

I used C because I wanted a chance to use some more advanced C programming practices I leaned recently like how to implement object oriented programming in C. For more details on the OOP concepts in this project see [OOP](docs/OOP.md). 

I am very happy with how this program turned out in terms of abstraction. The input processing feels very detached from calculating the output which is thanks to object oriented thinking.


# Installation and Usage

To install on a Linux system, ensure you have a C tool-chain and make installed, then clone this repository.

The default make target builds in the current directory and runs a python test script to detect any errors.

    $ make

After running make you can call the program.

    $ ./subnet 10.0.0.0/10

Or you can build and install it with the install target.

    $ sudo make install

    $ subnet 10.0.0.0/10

The default installation path is /usr/local/bin. You can uninstall with the uninstall make target.

    $ sudo make uninstall

Running the command with no arguments will print the possible inputs.

    $ subnet
    ...


# Testing

The python script tests_py/test.py processes a list of tests to be performed. The list of tests are in the tests_py/test_cases.csv file. I add every failure mode I run into onto the list of tests to ensure no errors are ever reintroduced. I added this python script to the default make target so errors are detected automatically. More details [tests_py](docs/tests_py.md).

# Regular Expressions

I used regular expression to filter inputs, for more details see [regex validation](docs/regex_validation.md).


## Input possibilities:
### Case 1 (Classful & CIDR):

    $ subnet [Network address]/[New CIDR mask]

Notes:

The old subnet mask is a classful mask determined from the first octet of the [Network address].
The new subnet mask is in CIDR format.

Examples:

    $ subnet 192.168.0.0/25       # Old subnet mask is 24, new is 25
    $ subnet 10.0.0.0/10          # Old subnet mask is 8, new is 10

### Case 2 (Classful & Dotted Decimal):

    $ subnet [Network address] [Dotted decimal subnet mask]

Notes:

The old subnet mask is a classful mask determined from the first octet of the [Network address].
The new subnet mask is in dotted decimal notation.

Examples:

    $ subnet 192.168.0.0 255.255.255.128      # Old subnet mask is 24, new is 25
    $ subnet 10.0.0.0 255.128.0.0             # Old subnet mask is 8, new is 9


### Case 3 (CIDR & Dotted Decimal):

    $ subnet [Network address]/[Old CIDR mask] [New dotted decimal subnet mask]

Notes:

The old subnet mask is in CIDR format. The new subnet mask is in dotted decimal notation.

Examples:

    $ subnet 192.168.0.0/25 255.255.255.192      # Old subnet mask is 25, new is 26
    $ subnet 10.0.0.0/9 255.192.0.0              # Old subnet mask is 9, new is 10


### Case 4 (Classful Maximum Subnets|Hosts):

    $ subnet [Network address] [0-9]{0,10}(s|h)

Notes:

Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
appropriate new subnet mask.  The old subnet mask is a classful mask determined from the first octet 
of the [Network address].
    
Examples:

    $ subnet 192.168.0.0 60s      # Calculate a new subnet mask to accommodate at least 60 subnets with the maximum 
                                  # hosts per subnet possible. The old subnet is calculated from the class.

    $ subnet 10.0.0.0 100h        # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum 
                                  # number of subnets possible. The old subnet is calculated from the class.

### Case 5 (CIDR Maximum Subnets|Hosts):
    
    $ subnet [Network address]/[Old CIDR mask] [0-9]{0,10}(s|h)

Notes:

Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
appropriate new subnet mask.  The old subnet mask is in CIDR notation.

Examples:

    $ subnet 192.168.0.0/25 60s   # Calculate a new subnet mask to accommodate at least 60 subnets with the maximum 
                                  # hosts per subnet possible. The old subnet is obtained from the CIDR notation.

    $ subnet 10.0.0.0/9 100h      # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum 
                                  # number of subnets possible. The old subnet is obtained from the CIDR notation.

### Case 6: - Coming soon

    $ subnet [Network address] ([0-9]*s:[0-9]*(i|h))+


### Case 7: - Coming soon

    $ subnet [Network address]/[Old CIDR mask] ([0-9]*s:[0-9]*(i|h))+


# Some Thoughts

Some things I want to implement in the future:

    - Cases 6 & 7 which will require multiple subnet calculations (possibly utilizing recursion)
    - More advanced forms of testing
    - Add test cases for logically correct addresses (not just checking that all invalid inputs error out correctly)
    - Retrieving subnet info given a specific node within the subnet
    - Refactoring src/input.c:process_input_ip_and_masks() so that there is less code repetition
    - Switches to choose output information
    - Adding a switch to print binary representation


# Examples

    $ subnet 192.168.0.0/25

    Number of subnets possible: 2
    Number of hosts per subnet: 126
    New subnet mask in CIDR notation: 25
    Old subnet mask in CIDR notation: 24
    Class of subnet: C
    Network address: 192.168.0.0
    Broadcast address: 192.168.0.255
    New subnet mask: 255.255.255.128
    Old subnet mask: 255.255.255.0
    Interesting octet: 4
    Block size: 128

    Subnet: 192.168.0.0/25
    Directed Broadcast Address: 192.168.0.127
        Usable Address Range: 192.168.0.1 - 192.168.0.126

    Subnet: 192.168.0.128/25
    Directed Broadcast Address: 192.168.0.255
        Usable Address Range: 192.168.0.129 - 192.168.0.254

---

    $ subnet 10.0.0.0 255.128.0.0

    Number of subnets possible: 2
    Number of hosts per subnet: 8388606
    New subnet mask in CIDR notation: 9
    Old subnet mask in CIDR notation: 8
    Class of subnet: A
    Network address: 10.0.0.0
    Broadcast address: 10.255.255.255
    New subnet mask: 255.128.0.0
    Old subnet mask: 255.0.0.0
    Interesting octet: 2
    Block size: 128

    Subnet: 10.0.0.0/9
    Directed Broadcast Address: 10.127.255.255
        Usable Address Range: 10.0.0.1 - 10.127.255.254

    Subnet: 10.128.0.0/9
    Directed Broadcast Address: 10.255.255.255
        Usable Address Range: 10.128.0.1 - 10.255.255.254

---

    $ subnet 172.16.0.0/17 255.255.192.0

    Number of subnets possible: 2
    Number of hosts per subnet: 16382
    New subnet mask in CIDR notation: 18
    Old subnet mask in CIDR notation: 17
    Class of subnet: B
    Network address: 172.16.0.0
    Broadcast address: 172.16.127.255
    New subnet mask: 255.255.192.0
    Old subnet mask: 255.255.128.0
    Interesting octet: 3
    Block size: 64

    Subnet: 172.16.0.0/18
    Directed Broadcast Address: 172.16.63.255
        Usable Address Range: 172.16.0.1 - 172.16.63.254

    Subnet: 172.16.64.0/18
    Directed Broadcast Address: 172.16.127.255
        Usable Address Range: 172.16.64.1 - 172.16.127.254