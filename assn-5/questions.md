1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

I did this by using a waitpid() loop after creating all the processes in the pipeline. If I forgot to call waitpid() on all the child processes, then these processes would have become zombies. The shell hence would not be able to determine when the commands finish executing.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because if we don't, then EOF would not get detected, i.e. reading processes will continue waiting for more input. Memory leaks can occue if we leave pipes open because fds are not released.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in because we need to change the directory of the shell process itself. If it was an external process, then the change would only change its own working directory and when the process actually terminates, parent shell would not be affected by it.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I think the best solution for this would be to dynamically add memory for the pipe_fds instead of using a fixed size array. The biggest trade-off would probably be to implement better memory management and much more error handling for the same.
