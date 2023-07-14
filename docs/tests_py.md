# Testing with Python

The tests_py/test.py python script processes a list of test (in CSV format) into arguments and expected responses of calls to this project.

If a test fails the script provides information for you to quickly replicate for debugging.

The script reads the CSV file and runs each possible test as a subprocess.

The CSV file has the following format.

The first column is the input Case,
The second is the class of the IP address,
The third is test arguments passed to the function,
The fourth is the expected error response of the test

    Case,Class,Test,Error,Comments

    1,A,"10.0.0./9","Invalid IP address"
    ...
    ...
    2,A,"10.0.0.0 254.0.0.0","Invalid CIDR mask, smaller than old mask"
    ...
    ...
    5,C,"192.0.0.0/25 100","Invalid second argument"
    ...
    ...

There are currently 80 and counting tests in tests_py/test.py and I want to add some test cases for correct outputs to stdout not just errors on stderr.

These test are ran automatically after every build since the script is executed as part of the default target.

    all: $(BINARY)
        @./tests_py/test.py

This allows me to immediately detect errors after every change.

    $ make

    Tests ran: 83

    Tests failed: 0
    
    $