---
title: "Program Concurrency"
date: 2018-09-16T19:50:57+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Do it, again.

There are (more than but I'll talk about) two ways for a program to create concurrent tasks, forks and threads.

# Process Forking
A fork duplicates/clones the current process into its own separate space.  
(Memory, variables, state is independently cloned).

## Functions
Library: unistd.h  

### `pid_t fork(void)`  
Returns -1 on errorm, 0 if child, else the PID of the child process

### `pid_t waitpid(pid_t pid, int *status, int options)`
Waits for a process, storing its return value into `status`. Returns the PID of the exited process.  

`pid < -1` - wait for any child process in the process group |`pid`|  
`pid == -1` - wait for any child process  
`pid == 0` - wait for any child process in the same PID group  
`pid > 0` - wait for `pid`  

### `pid_t wait(int *status)`
Waits for any process to exit, and stores its return value into `status`. Returns the PID of the exited process.  
Equivalent to `waitpid(-1, &status, 0)`

# Threading
The biggest difference between child processes and threads is that threads **share resources**.

## Functions
Library: pthread.h

### `int pthread_create(pthread_t *Thread, pthread_attr_t *Attr, void *(*Func)(void *), void *Arg)`
Create a new thread  
Arg 1 - Thread information to store into  
Arg 2 - Attributes  
Arg 3 - Function for thread to run  
Arg 4 - Arguments for thread's function to run  

### `pthread_t pthread_self(void)`
Gets current thread ID

### `pthread_t pthread_equal(pthread_t t1, pthread_t t2)`
Returns a non-zero value if `t1` and `t2` are the same.  
Returns **0 if NOT THE SAME**

### `int pthread_join(pthread_t T, void **value_ptr)`
Wait for thread `T` to finish, storing the thread's return value into `value_ptr`

### `void pthread_exit(void *value_ptr)`
Terminates the current thread, storing a return value in `value_ptr`

# Implications of Concurrency
## The Good
* Programs can run 'simultaneously'
* If there are multiple CPU cores, multiple programs can actually be run simultaneously.
* Each program operates without affecting other processes
## The Not-So-Good
### Problem
If a program intended for concurrent use is poorly written, the program may experience the following issues:  
* Non-deterministic output - the same input produces different outputs
* Deadlock - processes indefinitely wait for each other to signal
* Starvation - a process keeps missing access to a resource
* Corruption - (bank example)

### Solution? Semaphores!
Semaphores can be considered as an overpowered locking flag that processes can use.  

It basically performs the below operation, except without requiring execution cycles to wait.
```
while (!flag);
doThis();
```
Instead of repeatedly checking the value of `flag`, semaphores communicate with the operating system to instruct the program when to continue.

#### semaphore.h definitions
```
#include <semaphore.h>

int sem_init(sem_t *Sem, int Shared, uint Value) // Initialise Sem
int sem_wait(sem_t *Sem) // Wait for Sem > 0, then decrement and continue
int sem_post(sem_t *Sem) // Increment Sem and continue
int sem_destroy(sem_t *Sem) // Free Sem
```

A locking flag allows concurrent functions to fully complete before other operations are executed.