1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

It determines when a command's output is fully received from the server through the EOF character appended by the server. When this is detected in the end, client knows that the transmission is over. For handling partial reads, the client can implement a receive loop that continues until the marker is detected.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define the message boundaries using delimeters like EOF or NULL characters. If this is not handled correctly, there is a chance that different parts of different messages can be mixed or the receiver waiting indefinietly for commands to come.


3. Describe the general differences between stateful and stateless protocols.

Stateful protocosl maintain the client session information between requests like TCP, which thus requries remembering prior interactions and requiring connection setup/teardown. Stateless protocols on the other hand treat each request as independent containing all information without server-side memory from prior requests.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?



5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_