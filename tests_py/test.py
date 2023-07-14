#!/usr/bin/python3

import csv
import subprocess


def invalid_error_test(test_args, err_msg, case, _class):
    command = "./subnet " + test_args
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    if(err_msg in result.stderr):
        return 0
    else:
        print(f"\nCase {case}, Class {_class}, {err_msg} Test Failed")
        print(f"Stdout: {result.stdout}")
        print(f"Stderr: {result.stderr}")
        return 1

# def correct_output_test(test_args, case, _class):
#     command = "./subnet " + test_args
#     result = subprocess.run(command, shell=True, capture_output=True, text=True)


tests_failed = 0
tests_ran = 0

with open("tests_py/test_cases.csv", "r") as test_cases:
    reader = csv.DictReader(test_cases, delimiter=',')

    for line in reader:
        if line["Case"] == "#":
            break
        tests_ran += 1
        tests_failed += invalid_error_test(case=line["Case"],
                                           _class=line["Class"],
                                           test_args=line["Test"],
                                           err_msg=line["Error"])


print(f"\nTests ran: {tests_ran}")
print(f"\nTests failed: {tests_failed}\n")