#!/bin/bash
####################### BASH HEADER ######################
# PROGRAMMERS:
#    (
#       (Syam Evani) (Axiom Space) (File Creation) (08-30-2023)
#    )
##########################################################

# Get all the env stuff
source $GUNNS_HOME/bin/bashrc

# Run all tests and return appropriate exit status & diagnostic
tests_output=$(./test/make_all_ut.sh 2>&1)
echo "$tests_output"

# Check if the output contains any of the failure indicators
if [[ $tests_output == *"NO TEST OUTPUT"* || $tests_output == *"FAILURES"* || $tests_output == *"Failures"* ]]; then
    echo "Status: TESTS HAVE FAILED!"
    echo "$tests_output" | grep -E "failed|FAILURES|Failures"  # Print the failure-related output
    exit 1
else
    echo "Status: ALL TESTS HAVE PASSED!"
    exit 0
fi
