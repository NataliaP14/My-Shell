#!/bin/bash

# Run all test scripts in the tests directory

echo "Running all test scripts..."

# Running basic_commands.sh
echo "Running basic_commands.sh..."
./mysh tests/basic_commands.sh
echo "-------------------------"

# Running conditionals.sh
echo "Running conditionals.sh..."
./mysh tests/conditionals.sh
echo "-------------------------"

# Running invalid.sh
echo "Running invalid.sh..."
./mysh tests/invalid.sh
echo "-------------------------"

# Running pipeline.sh
echo "Running pipeline.sh..."
./mysh tests/pipeline.sh
echo "-------------------------"

# Running redirection.sh
echo "Running redirection.sh..."
./mysh tests/redirection.sh
echo "-------------------------"

# Running wildcard.sh
echo "Running wildcard.sh..."
./mysh tests/wildcard.sh
echo "-------------------------"

echo "All tests completed!"
