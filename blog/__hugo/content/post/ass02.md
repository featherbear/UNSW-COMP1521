---
title: "Assignment 2 - My Very Own Shell"
date: 2018-09-17T10:17:36+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Assignment 2 - Make your own shell!  [[View Files Here]](https://github.com/featherbear/UNSW-COMP1521/tree/master/Assignments/ass02)


Considering that I started it the day the assignment was released, the specs changed tirelessly.  
An endless tirade of constant bug squelching and tweaking, but it's good to see an original (?) assignment.

# Command Execution
_Marks: 1_  

The shell was to be able to execute programs entered into the prompt, locating any programs that were in one of the `PATH` directories.  
Nothing too fancy.

# Command History
_Marks: 2_  

The shell was to be able to store (and recall) the most recent 20 valid commands, using the `h` or `history` command.  
Command recalling was accessible with  `!(cmdNo)`, or `!!` which recalled the most recent command.

I did have a few disagreements with the assignment spec's requested implementation of the command history.

## cmdNo
From my programming experience, I would much prefer longer code that is modular and easily separable than short code intertwined in a mess of different functionalities. _[ironic](https://github.com/featherbear/UNSW-COMP1511/blob/master/Lab/Week%2003/order3_challenge1_golf.c)_  

The history 'module' exposed the current command number (`cmdNo`) to the main program. This left the history functionality as an important dependency to the success execution of the program (even its compilation!). A few of my friends were concerned that their initial program code was not working because they had not yet completed the history component.  

Of course, `history.c` isn't a library, and has a use case very specific to this assignment - but I decided to use `cmdNo` internally within the history component.

## The `history` command
Something that I wanted to change (but didn't because it's an assignment and I gotta follow the specs) was the entries that the `h` and `history` commands displayed.  

Consider a case where twenty arbitrary valid commands were executed.  
Upon typing `history` we would see
```
   1  aaa
   2  bbb
   3  ccc
   4  ddd
  ..  ...
  18  rrr
  19  sss
  20  ttt
```

Cool, so we can use `!1` to `!20` right!?  

**Wrong.**

`!2` would work,  
`!3` would work,  
`!20` would work,  
But `!1` won't work.  

`history` also counts as a valid command, and unfortunately, the command history is displayed prior to this command being added into the history itself.

Our command history, after typing in `history` now actually becomes something like this
```
   2  bbb
   3  ccc
   4  ddd
  ..  ...
  18  rrr
  19  sss
  20  ttt
  21  history
```

This means that after we perform `history` (cmdNo 21), we have the recall range from `!2` to `!21`.

It's counter-intuitive to show the cmdNo 1 if we can't use it, and it's easily fixable by adding the `h` and `history` command to the history prior to its execution, however, we got to follow the assignment spec!

# Builtins
_Marks: 1_  

The builtin commands to be implemented were `h`, `history`, `pwd`, `cd` and `exit`.  
Simple!  

(really, it was)

# Wildcard Expansion
_Marks: 2_  

`glob` is a pretty cool feature, huh!  
It allows you to expand wildcards and aliases like `~` and `*.c`.  

As this functionality would result in more tokens, to implement in C, I basically had to replicate the token list, exploding the wildcards and aliases. In terms of memory management, reminder to free the old tokens!

# I/O Redirection
_Marks: 3_  

Input and output redirection was pretty straightforward, just utilising the `dup2` function to change the sinks for `stdin` and `stdout`.  
_I originally also implemented the stdout append redirection, but took that feature out incase it broke the autotests_

# Bonus: Piping


# Remarks
Overall a quite interesting assignment for students to understand what exactly a shell does.  
I'll stick to `bash` though :)  


> (Flex) Time taken to complete: 4 hours 37 minutes. Woo!

![Visualisation of the assignment](/img/ass02_visualisation.gif)