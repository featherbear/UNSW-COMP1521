---
title: "Week One - Bitwise Operations"
date: 2018-07-29T00:18:02+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Redefine how we can use numbers!

> Read more [here](../bitwise_operations)

Logical Operators  
AND | `a && b`  
OR  | `a || b`  
EQ  | `a == b`  
GT  | `a > b`  
GTE | `a >= b`  
LT  | `a < b`  
LTE | `a <= b`  
  
Bitwise Operators  
AND | `a & b`  
OR  | `a | b`  
XOR | `a ^ b`  
NEG | `~a`  
LSHIFT | `a << n`, where `n` is an integer  
RSHIFT | `a >> n`, where `n` is an integer  
If signed, sign bit replaced left-end bit  

How the bitwise operators differ to the logical operators, is that the bitwise operators compare the individual bits of the operands and can return a non-binary answer, whilst the logical operators only returns a `1` for true, or a `0` for false.  

For `LSHIFT` and `RSHIFT`, mathematically we are multiplying / dividing by 2^n  
