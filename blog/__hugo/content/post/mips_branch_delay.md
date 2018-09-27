---
title: "MIPS and Branch Delays"
date: 2018-08-17T00:19:01+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

**If the label fun is at 0x00401000, what value is contained in register $ra immediately after the execution of the jal instruction on the spim virtual machine?**
```
0x00400200   li  $a0, 42
0x00400204   jal fun
0x00400208   nop
0x0040020C   sw  $v0, x
```


So, MIPS has this thing called a `branch delay slot`, which allows an extra instruction to be performed before the jump - as to keep the clock in sync.

Do I get it, yes.  
Do I understand, maybe? 

---
So it looks like the branch and jump instructions first get loaded into the memory.  
But before they execute, the next instruction directly afterwards gets loaded while the jump is performed.

---

That means, that to answer this question, then `$ra = 0x0040020C`.  
That makes sense, as the return address becomes `$ra + 8`

But wait!
The answer could also be `0x00400208`.

If you're use SPIM this is the result you would get.  
That's because SPIM has branch delay execution **disabled by default**.  
And what that means, is that a function call will return to the immediate next address `$ra + 4`.

