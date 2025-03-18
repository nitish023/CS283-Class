#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#working
@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF
    [ "$status" -eq 0 ]
}

#works
@test "Local Pipes" {
    run "./dsh" <<EOF                
ls | grep dshlib.c
exit
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.clocalmodedsh4>dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

#working
@test "Remote Command Execution" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1
    run "./dsh" -c -p $PORT <<EOF
echo hello from remote
exit
EOF

    kill $SERVER_PID
    wait
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [[ "$stripped_output" == *"hellofromremote"* ]]
    [ "$status" -eq 0 ]
}

#woroking
@test "Remote Pipes" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1 

    run "./dsh" -c -p $PORT <<EOF
ls | grep dshlib.c
exit
EOF

    kill $SERVER_PID
    wait

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [[ "$stripped_output" == *"dshlib.c"* ]]
    [ "$status" -eq 0 ]
}

#works
@test "Remote CD Command" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1 
    run "./dsh" -c -p $PORT <<EOF
cd /tmp
pwd
exit
EOF

    kill $SERVER_PID
    wait

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [[ "$stripped_output" == *"/tmp"* ]]
    [ "$status" -eq 0 ]
}

#correct results
@test "Too Many Pipes" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1
    MANY_PIPES="ls"
    for i in {1..10}; do
        MANY_PIPES="$MANY_PIPES | cat"
    done

    run "./dsh" -c -p $PORT <<EOF
$MANY_PIPES
exit
EOF

    kill $SERVER_PID
    wait

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [[ "$stripped_output" == *"pipinglimited"* ]]
    [ "$status" -eq 0 ]
}

#works
@test "Stop Server Command" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1

    run "./dsh" -c -p $PORT <<EOF
stop-server
EOF
    sleep 1
    if ps -p $SERVER_PID > /dev/null; then
        echo "Server still running!"
        kill $SERVER_PID
        wait
        false  # Make the test fail
    fi

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [ "$status" -eq 0 ]
}

#working fine
@test "Error Handling" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1
    run "./dsh" -c -p $PORT <<EOF
nonexistentcommand
exit
EOF

    kill $SERVER_PID
    wait

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output}"

    [[ "$stripped_output" == *"notfound"* ]] || [[ "$stripped_output" == *"Nosuchfile"* ]]
    [ "$status" -eq 0 ]
}

#Workss
@test "Multiple Client Connections" {
    PORT=12345
    ./dsh -s -p $PORT &
    SERVER_PID=$!
    sleep 1

    CLIENT1_OUTPUT=$(echo -e "echo client1\nexit" | ./dsh -c -p $PORT)
    CLIENT2_OUTPUT=$(echo -e "echo client2\nexit" | ./dsh -c -p $PORT)
    
    kill $SERVER_PID
    wait

    stripped_output1=$(echo "$CLIENT1_OUTPUT" | tr -d '[:space:]')
    stripped_output2=$(echo "$CLIENT2_OUTPUT" | tr -d '[:space:]')

    echo "Client 1 output: $CLIENT1_OUTPUT"
    echo "Client 2 output: $CLIENT2_OUTPUT"
    echo "Stripped 1: ${stripped_output1}"
    echo "Stripped 2: ${stripped_output2}"

    [[ "$stripped_output1" == *"client1"* ]]
    [[ "$stripped_output2" == *"client2"* ]]
}

#working
@test "Multi-threaded Server: Concurrent Processing" {
    PORT=12346
    ./dsh -s -p $PORT -x &
    SERVER_PID=$!
    sleep 1
    
    OUTPUT1=$(mktemp)
    OUTPUT2=$(mktemp)
    
    (echo -e "sleep 3\necho SLOW_DONE\nexit" | ./dsh -c -p $PORT > $OUTPUT1) &
    PID1=$!
    
    sleep 0.5
    
    (echo -e "sleep 1\necho FAST_DONE\nexit" | ./dsh -c -p $PORT > $OUTPUT2) &
    PID2=$!
    wait $PID1
    wait $PID2

    TIME1=$(date -r $OUTPUT1 +%s)
    TIME2=$(date -r $OUTPUT2 +%s)
    DIFF=$(($TIME1 - $TIME2))
    
    kill $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
    rm $OUTPUT1 $OUTPUT2
    echo "Time difference between clients: $DIFF seconds"
    [ $DIFF -ge 1 ]
}


