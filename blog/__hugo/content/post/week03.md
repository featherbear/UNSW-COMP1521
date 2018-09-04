---
title: "Week Three - Bit Fields, Unions"
date: 2018-08-07T00:14:18+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

This week's lab revealed a pretty important piece of information, that  
**bit fields are structured in reverse order**

Consider the C code
```
struct _bit_fields {
   unsigned int a : 4,
                b : 8,
                c : 20;
};
```

This struct could be represented in two ways  
A) `aaaabbbb bbbbcccc cccccccc cccccccc`  
OR  
B) `cccccccc cccccccc ccccbbbb bbbbaaaa`  

We can find out by using a `union`!
A `union` allows the same portion of memory to be intepreted as different data types.  

What that means for this question, is that we can represent the entire `struct _bit_fields` as one thing, like an integer!
So if we create a variable with this type, and through a `union`, set the value to `1` - we can check if `a` is 1, or if `c` is one.
  
(On the CSE machines, `a` becomes 1, so therefore representation A was correct)  
Therefore it tells us that the order that we define our bitfields are important, and are mapped in the reverse order! 