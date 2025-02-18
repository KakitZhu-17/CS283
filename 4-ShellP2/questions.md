1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  we do not use exevp by itself because it can replaces our current process wiith a new one. this means we cannot continue our intital program since its overwritten. we use fork with it to avoid this since fork allows us to make a child process to run the new program and also allows the parent/inital process to continue.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  if fork fails it just measn its unable to make a child process(this is caused by a variety of reasons). for me personally i print a error and continue taking inputs.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp uses the first argument and uses the direcotry inside the PATH variable to find the command and if its found then it replaces our current process with the command. the PATH variable basically tells exec where to look for the command.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: its exactly how it sounds, it waits for the child process to finish before continuing the current process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS() gives the exit code of the child process back and its important because it tells us why the child process failed and can help our current process/parent know what to do next. its also handy for error handling.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  for this assignment i just iterates through the string and if it detects " then it will keep add the characters into a character array until it reaches anothe " char. this is necessary because some commands can take quroted arguments like echo and it is useful if we want to preserve the spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  i made quite a bit off changes because now we are dealing with argv which meanst i have to break every argyment down one by one. my original parsing code only broke the string down by first word/cmd and arguments are just one string. I also had to makes sure the code deals withh quoted arguments which required iterating through and add all chacrters to the char array until we see a second quote character. for non qutoed argument is similiar idea but we start by adding the first non ' ' char and if we detect a ' ' next then we stop adding the chars.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  a signal is used to notify or interrupt a program/process to when a specific event occurs (a exameple of this is when we come across segmentation faults). there are many kinds. it differs from IPC because they do not deal with the tranfer of data like IPCs do and instead it just deals more with error handling.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGSEGV is one that we are very familitar with and its is trigger when there is a segmentation fault (accessing memory we do not own). the next one is SIGINT(this just means signal interrupt) which is triggered when we do ctrl C which temrinates the process we are running. the ifnal one that we use fairly commonly is SIGTERM (termination of process) which usually occurs before exit()(before clean up); 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: SIGSTOP stops or pauses the process and it cannot be ignored or caught because it to ensure lower level control or kernel control
