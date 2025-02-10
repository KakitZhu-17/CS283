1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  it is a good choice for this application because we can set the limit for buffer size and prevents any overflow that can happen. overall its just very safe

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  its better to use malloc because fixed arrays can cause the buffer to overflow and malloc allows for resizing later on.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: it needs to be trimmed because the commands that we may run may be sensitive to white space and can cause the command to fail or have unintended affects. this is for safety reasons since there is no gurantee the commands will trim white space before hand.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  first redirecting outputs of our command into a separate txt file (cmd > file.txt). another example would be redirecting in the other direction, more specifically getting input from a text file into the command (cmd < file.txt). these two share more or less the same problem with one another which is trying to make sure the inputs and outputs to go in the right direction/destination and another issue i can see would be trying to make sure the txt file has the correct inputs for the command to work properly (though this is likely more command based). another example would be appending our command outputs into a file thats not empty (cmd >> file.txt). thie issue iwth this would be checking if the file exists in our directory or not to make sure we dont make a new file or overwrite a old one. one final thing I could see being a challenge might be trying to incorate it to work with piping since that seems quite complex if we are chaining them.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  piping (uses |) connects/chains the output of a command into another command as stdin/input while redirection (uses > or <) directs the output of a command into a file or get a input from the file into a command. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  its absolutely important to keep them separate because if a error message is in STDOUT then it will most likely be used as a input for another command if we are piping and this can have unintended or event dangerous affects if we are chaining/piping mutliple commands at the same time. Also keeping them separate helps a lot in debugging because we can set the STDERR to be outputted into a file for testing and evaluation later on.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: I think that if we get a error(STDERR) we should terminate or exit the process/pipline to prevent any unintended affects. We shouldn't merge them together since that would be counter intuitive to the idea of STDERR's purpose but this really depends on what we want to do with the shell.