---
title: "Week Two - BigNum.c"
date: 2018-08-05T00:01:39+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

This week's lab dealt with handling (simple) arithmetic of large numbers larger than 2,147,483,647 (2^32).

If we added one to that number...  
`11111111 11111111 11111111 11111111 +`   
`00000000 00000000 00000000 00000001`  
`-----------------------------------`  
`00000000 00000000 00000000 00000000`  
_(That's a zero)_

So instead, what we can do is to create an array of integers - each index being a different order of magnitude.  
For example, the number 915676 can be represented like `{ 9, 1, 5, 6, 7, 6 }`.  
So if we added the number 5...  
`{ ..., 7, 6 + 5 = 11 }`  
`{ ..., 8, 1 }`  
`{ 9, 1, 5, 6, 8, 1 }`  
Which gives us 915681  

The challenge activity for this week was to implement subtraction and multiplication of these `BigNum`s.  
I also decided to put in division because I got bored