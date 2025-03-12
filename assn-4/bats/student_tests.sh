#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#tests follow similar structure to the assignment_tests.sh
@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "External: Check 'pwd' runs without errors" {
    run ./dsh <<EOF
pwd
EOF
    [ "$status" -eq 0 ]
}

@test "External: Check 'echo' with arguments" {
    run ./dsh <<EOF
echo Hello World
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="HelloWorlddsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]
}

@test "Built-in: Exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF

    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "Change directory - no args" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test

    run "${current}/dsh" <<EOF                
cd
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Change directory - with args" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test

    run "${current}/dsh" <<EOF                
cd dsh-test
pwd
EOF
    actual_path=$(realpath /tmp/dsh-test)  
    expected_output="${actual_path}dsh2>dsh2>dsh2>cmdloopreturned0"
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Built-in: 'cd' to a non-existent directory should fail" {
    current=$(pwd)

    run "${current}/dsh" <<EOF                
cd /tmp/nonexistent-dir
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cd:Nosuchfileordirectorydsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "rc: should return 0 after successful command" {
    current=$(pwd)

    run "${current}/dsh" <<EOF
ls
rc
EOF

    filtered_output=$(echo "$output" | tail -n 3 | tr -d '[:space:]')
    expected_output="dsh2>dsh2>0dsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Filtered Output: $filtered_output"
    echo "Exit Status: $status"
    echo "${filtered_output} -> ${expected_output}"

    [ "$filtered_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
