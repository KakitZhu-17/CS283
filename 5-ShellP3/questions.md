1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

we ensure this by using waitpid to ensure all processess finish before we continue on with the rest of the code. if we forget to call waitpid then we will hav a very clear desync or inconsistent timing with the child and the parent (one can finish before the other which can mess up the output of the program). we also wouldnt be able to get he exit/ error codes if we dont wait for the process to finish. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

from my personal experience from this assignment it seems to freeze my program if I dont close it and it seems to be caused by the program waiting and trying to get outputs from pipes that cannot be written or read because we didnt close it in a previous iteration / child process in which it was done writing or reading. so the waiting causes freezing.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

this is because a external command need a separate process to run (which we learned from previous assignments) which also means that anything we do with the external commands only changes the process but not the parent(ie. our cwd) which means if cd was a external command it would not change anything in the current directory (in the parent process/our real directory). so cd needs to be a built in because it updates the current directory directly and allows it to keep the changes made (think of it as the parent process).

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

this might not be too difficult, my current implementation counts the number of pipes chars in the beginning from the buffer anayways (buffer can be reallocated at anytime). As for the piping itself we can remove the CMD_MAX all together and it should still work fine as long as i can still keep track of the ammount of pipes/commands there are in the buffer. all of it goes back to the buffer, so realloc might be the key to this.
