#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

#Works
@test "Single Command with arguments: echo test message" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo test message
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" == *"testmessage"* ]]
    [ "$status" -eq 0 ]
}

#One command which is built in, WORKS
@test "Built-in single command: cd with argument" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
cd /tmp
pwd
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>/tmpdsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [[ "$stripped_output" == *"/tmp"* ]]
    [ "$status" -eq 0 ]
}

#Works
@test "Built-in command: rc after failed command" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
nonexistentcommand
rc
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" != *"dsh3>0dsh3>"* ]]
    [ "$status" -eq 0 ]
}

#Works
@test "Simple pipe: ls | grep dshlib.c" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
ls | grep dshlib.c
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

#Works
@test "Multiple pipes: ls | grep d | wc -l" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
ls | grep d | wc -l
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" =~ [0-9]+ ]]
    [ "$status" -eq 0 ]
}

#error in command, WORKS
@test "Too many piped commands" {
    current=$(pwd)
    many_pipes="ls"
    for i in {1..15}; do
        many_pipes="$many_pipes | cat"
    done
    
    run "${current}/dsh" <<EOF
$many_pipes
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" == *"pipelimit"* || "$stripped_output" == *"toomany"* ]]
    [ "$status" -eq 0 ]
}

#works
@test "Built-in command in pipeline" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo test | cd /tmp | wc -c
pwd
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" == *"/tmp"* ]]
    [ "$status" -eq 0 ]
}

#Works finally
@test "Whitespace in pipe syntax" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo hello    |    tr 'a-z' 'A-Z'   |   wc -c
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    
    [[ "$stripped_output" =~ [0-9]+ ]]
    [ "$status" -eq 0 ]
}

#works
@test "Commands with quotes processing" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "hello world"
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

#Finally works
@test "Handling spaces in pipe commands" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "test message" | tr 'a-z' 'A-Z'
exit
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="TESTMESSAGEdsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

#works
@test "Output redirection: simple echo to file" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "hello world" > test_output.txt
cat test_output.txt
rm test_output.txt
exit
EOF
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    [[ "$output" == *"hello world"* ]]
    [ "$status" -eq 0 ]
}

#works
@test "Input redirection: cat from file" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "input test data" > test_input.txt
cat < test_input.txt
rm test_input.txt
exit
EOF
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    [[ "$output" == *"input test data"* ]]
    [ "$status" -eq 0 ]
}

#working
@test "Pipes with redirection: cat from file, pipe to tr, output to file" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "text for pipe test" > pipe_input.txt
cat < pipe_input.txt | tr 'a-z' 'A-Z' > pipe_output.txt
cat pipe_output.txt
rm pipe_input.txt pipe_output.txt
exit
EOF
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    [[ "$output" == *"TEXT FOR PIPE TEST"* ]]
    [ "$status" -eq 0 ]
}

@test "Multiple redirections in pipeline" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo "first line" > multi_test.txt
echo "second line" >> multi_test.txt
cat < multi_test.txt | grep "line" | tr 'a-z' 'A-Z' > multi_out.txt
cat multi_out.txt
rm multi_test.txt multi_out.txt
exit
EOF
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    [[ "$output" == *"FIRST LINE"* && "$output" == *"SECOND LINE"* ]]
    [ "$status" -eq 0 ]
}
