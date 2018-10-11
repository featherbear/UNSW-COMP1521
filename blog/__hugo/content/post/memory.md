---
title: "Memory"
date: 2018-09-14T20:22:06+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

**Endian**  
Little-Endian  
a b c d  
e f g h  

Big-Endian  
d c b a  
h g f e  

# Virtual Memory
## Intro
Consider a program that stores the number 5 into the address `0x00000000`, and prints it out a short time later...
```
// psuedocode
*(0x00000000) = 5
delay()
print(*(0x00000000))
```
_// Program written in in [NoCode](https://github.com/kelseyhightower/nocode). kappa_

What we should see when we run the program is the number `5`.
```
> ./program1
5          
```

Let's have another program that stores the name "Andrew" into the address `0x00000000`, and prints it out a short time later...
```
// psuedocode
*(0x00000000) = "Andrew"
delay()
print("Hello, " + *(0x00000000))
```

Our output would look like this:
```
> ./program2
Hello, Andrew
```

**But**, what if we ran `program2` slightly after `program1`?
```
> ./program1 | > ./program2
Andrew       | Hello, Andrew
```

Uh-oh! We just lost the value 5, where did it go?  
It got overwritten, because both programs were writing into the same memory address.

Let's desk check it...
```
TIME | > ./program1 | > ./program2     | Value of MEM
0    | [PROG START] |                  |      -      
1    | SET MEM = 5  | [PROG START]     |      5      
2    | DELAY        | SET MEM = Andrew |   Andrew    
3    | PRINT MEM    | DELAY            |   Andrew    
4    | [PROG END]   | PRINT MEM        |   Andrew    
5    |              | [PROG END]       |   Andrew    
```

This is an issue! If two (or even more) programs read and write from the same memory addresses, things will go horribly wrong!  
Luckily, operating systems have a solution to this issue - a mechanism called Virtual Memory!

## Features
Each program is effectively sandboxed, isolating it from the rest of the other programs running on a system.  
This also means that the program is unable to corrupt the memory spaces of other programs if something went wrong (memory protection)

As a result, the program is free to access any memory address - virtual memory address that is..

Physical memory addresses point to the actual locations in the computer's memory chips.  
Virtual memory addresses seamlessly resolve into physical addresses through what is known as a Page Table.
This means that a program accessing the virtual memory address 0x12345678 might access the physical address 0x05206677, without ever actually knowing that physical address.

## Page Tables
Page Tables act as a proxy, mapping virtual memory addresses to physical memory addresses.  
Think of a network switch, when a packet comes in, that packet is routed towards and from its source and destination.

```
-----------------------------
Virtual Addr | Physical Addr
   0 -  999  |    0 -  999
1000 - 1999  | 4000 - 4999
2000 - 2999  | 5000 - 5999
```

So a program accessing address 1001 would actually be accessing address 4001!



Size of a page = size of a frame