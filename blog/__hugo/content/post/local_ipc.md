---
title: "Local Inter-Process Communication"
date: 2018-09-24T08:45:34+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

In one ear, out the other...wait.  

> [Network IPC? Click here](../network_IPC)


# Pipes
Pipes allow forked processes to communicate with each other, and is often used to redirect the output of a process into the input of another process.

A common usage pattern for pipes is:

* Create a pipe
* Fork the process (both processes contain the pipe file descriptors)
* Write into the pipe as the parent/child
* Read from the pipe as the child/parent


`unistd.h` provides `STDIN_FILENO` and `STDOUT_FILENO` (and `STDERR_FILENO`)

`int dup2(int oldfd, int newfd)`  
Close `newfd` if necessary, and copy `oldfd` into `newfd`.  

Effectively, the `dup2` function sets `newfd` to `oldfd`.  
For example `dup2(fd, STDOUT_FILENO)` will set the output to go into `fd` instead of through the standard output

`int pipe(int fd[2])`  
ret: 0  
err: -1

The `pipe` function allocates a pair of file descriptors in which the first is read from, and the second is written into.  
This allows a child process to communicate with the parent process.

`FILE *popen(char *Cmd, char *Mode)`  
ret: FILE*  
err: NULL

# Message Queues
Message Queues allow for unrelated processes (ie a process that was not forked) to communicate with other processes. As opposed to the buffer stream that pipes provide, message queues separate each message from another.

_Note: Message queue names start with a forward slash `/`_


`mqueue.h` provides the functionality for message queues.

`mqd_t mq_open(char *Name, int Flags)` - Create/open a message queue  
`int mq_close(mqd_t *MQ)` - Close access to a message queue  
`int mq_send(mqd_t MQ, char *Msg, int Size, uint Prio)` - Block and wait until MQ has space  
`int mq_receive(mqd_t MQ, char *Msg, int Size, uint *Prio)` - Receive highest priority message in MQ

> [Network IPC? Click here](../network_IPC)
