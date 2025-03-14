1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

in this implementation we used 0x04 to tell if we recieved the entire output. we used a recv loop to make sure it sends the full output instead of a partial one and it stops when 0x04 is detected.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

we made sure to send an extra byte / streln(cmd)+1 to make sure its null terminated. if we didn't do this it may not even process the string/command we send.

3. Describe the general differences between stateful and stateless protocols.

stateful means it remembers and maintains information/session information while stateless retains no memory of previous sessions

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

the main advanatge it has its speed but that also because of its low overhead due to he fact thats its a stateless protocol

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

the interface is sockets.