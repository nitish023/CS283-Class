1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  Well fgets() is a good choice because reading user input because it allows us to read an entire line until an EOF is encounterd and also allows us to specify what is the maximum input length we want.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  I think we needed to use malloc() to allocate memory because it allocates memory on a heap instead of a stack as in the case of a fixed-size array. Allocating on heap allows for more flexibility and will be able to handle large input sizes hence we can even increase our size of SH_CMD_MAX in the future without any worries.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces from each command is necessary because extra spaces can cause unexpected behavior or lead to errors when we try to execute that command. Also, normal shells also do this so kind of makes sense if our shell does it too.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  ls > output.txt ; echo "New log" >> log.txt ; sort < unsort.txt > sort.txt ; Challenges: Need to open files, using dup2() to redirect.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection basically controls where the input is coming from or where the output is going to be saved. Piping on the other hand sends the output of command to another command so it connects multiple commands unlike redirection which just affects how a command is interacting with files.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping these separate in a shell will ensure that the error does not get mixed with the output. For example, if we do run into an error in our program, debugging would be much easier if the error was written on its own instead of being mixed with whatever program generated.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  We should check when a command fails, if it does fail we should return an appropriate code indicating the error and should print an appropriate message. I don't think we should merge them but if we have to, we can do something like this using redirection: ls not_exist > output.txt 2>&1