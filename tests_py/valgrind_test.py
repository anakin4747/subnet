#!/usr/bin/python3

import csv
import subprocess

def valgrind_mem_check(test_args, err_msg, case, _class):
    command = "valgrind --leak-check=full ./subnet " + test_args
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    expected_output = "All heap blocks were freed -- no leaks are possible"
    if(expected_output in result.stderr):
        return 0
    else:
        print(f"\nCase {case}, Class {_class}, Valgrind {err_msg} Test Failed")
        print(f"Command: {command}")
        print(f"Stdout: {result.stdout}")
        print(f"Stderr: {result.stderr}")
        return 1


tests_failed = 0
tests_ran = 0

with open("tests_py/test_cases.csv", "r") as test_cases:
    reader = csv.DictReader(test_cases, delimiter=',')

    for line in reader:
        if line["Case"] == "#":
            break
        tests_ran += 1
        print(f"Valgrind test: {tests_ran}  ", end='\r')
        tests_failed += valgrind_mem_check(case=line["Case"],
                                           _class=line["Class"],
                                           test_args=line["Test"],
                                           err_msg=line["Error"])


print(f"\nTests ran: {tests_ran}")
print(f"\nTests failed: {tests_failed}\n")