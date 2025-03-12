1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Well, the fork() command creates a new child process. This allows the parent to continue running while the child executes the command. execvp() will replace the process itself without fork().

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork fails, it will return -1 that means the there was no child process that got created. My implementation handles this scenario by checking for it and returning ERR_EXEC_CMD without executing execvp().

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command to execute by searching for the command in directories listed in the PATH variable. PATH is the system enviornment variable that plays a role here.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Purpose of calling wait() in the parent process is that it makes sure that the parent waits for the child to finish.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() extracts the status code of the child process form waitpid(), allowing the shell to check if the command executed successfully.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It uses a has_quotes boolean when encountering double quotes, basically treating quoted arguments as a single argument. This is necessary to ensure that arguments with spaces remain grouped.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: I improved the handling of spaces and quotes and structured command storage using cmd_buff_t instead of a list because this assignment only dealt with one command at a time. I did not face much issues in refactoring.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Processes can communicate asynchronously due to signals, unlike other IPC methods that require a direct data exchange.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL (9)- Immediately terminates a process. SIGTERM(15)- Asks a process to terminate. SIGINT(2)- Interrupts a process. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  The process pauses execution which it receives SIGSTOP. Unlike SIGINT, it can be caught, it remains stopped and can be resumed with SIGCONT.
