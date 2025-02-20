1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  we call fork/exevp because if we call execvp by itself it can replaces our current process with a new one. this means we cannot continue our initial program since its been replaced/overwritten. we use fork with it to avoid this since fork allows us to make a child process to run the new program (using execvp) and also allows the parent/inital process to continue without replacing it.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  if fork fails it just means its unable to make a child process (this is caused by a variety of reasons). for me personally i print a error message explaining why it failed and set the rc variable as -1 (or whatever the error code is).

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp uses the first argument and look through the directories listed inside the PATH variable to find the command and if its found then it replaces our current process with the command/file. the PATH variable basically tells execvp where to look through for the command. the second argument is used as arguments for the command we want to execute.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: its exactly how it sounds, it waits for the child process to finish before continuing the current/process process. if we didnt call it, it can issues with the timing of the outputs and ruin some formatting. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS() gives the exit status of the child process and its important because it tells us why the child process failed and can be used to check if the child process ended properly so its pretty handy for error handling.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  for this assignment i just iterates through the string and if it detects " then it will keep add the characters into a character array until it reaches another " char. this is necessary because some commands can take quoted arguments like echo and it is useful if we want to preserve the spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  i made quite a bit of changes because now we are dealing with argv which meant i have to break every argument down one by one. my original parsing code only broke the string down by first word/cmd and arguments are just one string. I also had to makes sure the code deals withh quoted arguments which required iterating through and add all characters to the char array until we see a second quote character (I explained it in more detail in the previous question). for non quoted argument is similiar idea but we start by adding the first non ' ' char and if we detect another ' ' next then we stop adding the chars into the array. the only real challenge was trying to refactor the code into breaking down every word and maintaining a similiar logic i was using before. I know its different from the approuch explained in lecture but I didnt want to completely discard my previous code as it seems like refactoring was encouraged.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  a signal is used to notify or interrupt a program/process to when a specific event/issue occurs (a example of this is when we come across segmentation faults and it terminates the process). there are many kinds of signals. it differs from IPC because they do not deal with the tranfer/movement of data like IPCs do (an exmaple of this is using piping in linux).

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGSEGV is one that we are very familiar with and it is triggered when there is a segmentation fault which is when we access or reference memory we do not own. the next one is SIGINT(this just means signal interrupt) which is triggered when we do ctrl C which teriminates the process we are running. the final one that we use fairly commonly is SIGTERM (termination of process) which usually occurs before exit() and it usually requests for the process/program to teminate (it does give the program a chance to clean up since it does not temrinate immediately); 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: SIGSTOP stops or pauses the process and it cannot be ignored or caught because it to ensure lower level control.
