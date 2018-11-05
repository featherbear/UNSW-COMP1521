---
title: "Bit-wise Operations"
date: 2018-07-30T18:31:11+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

AND, OR, XOR...?

# Introduction
At the end of the day, computers run off pulses of electrical signals.  
HIGHs and LOWs...  
ONs and OFFs...  
1s and 0s...  
Binary.  

The computer sees all of its data as binary, the character `A` is seen as an upper-case A to us, but to a computer it sees it as `0b1000001` _(ASCII value of A)_  

Our maths operations are all done as binary too!  
While we add 15 + 6 = 21, the computer adds the binary representations of these values
```
15 = 0b00001111
 6 = 0b00000110
   +   --------
     0b00010101 = 21
```

That's pretty cool, though how do we access these bits? With bit-wise operations!  
_Note: A `bit` is a single `1` or `0` inside a binary number_

# Bit-wise Operators
AND | `a & b`  
OR  | `a | b`  
XOR | `a ^ b`  
NEG | `~a`  
LSHIFT | `a << n`  
RSHIFT | `a >> n`

## AND
The `bit-wise AND` operator `&` compares the bits of each column, and returns a 1 if both bits are 1  

Example:  
`0b111000 & 0b011001`
```
  0b111000
& 0b011001
    ------
  0b011000
```

## OR
The `bit-wise OR` operator `|` compares the bits of each column, and returns a 1 if either (or both) of the bits are 1  

Example:  
`0b111000 | 0b011001`
```
  0b111000
| 0b011001
    ------
  0b111001
```

## XOR
The `bit-wise eXclusive OR` operator `^` compares the bits of each column, and returns a 1 only if exactly one of the bits is a 1  

Example:  
`0b111000 ^ 0b011001`
```
  0b111000
^ 0b011001
    ------
  0b100001
```

## NEG
The `bit-wise NEGation` operator `~` flips all the bits of a number, effectively changing all 0s to 1s, and all 1s to 0s.  

Example: `~0b1100101 == 0b0011010`

## LSHIFT
The `bit-wise Left SHIFT` operator `<<` moves all of the bits in a binary number left by `n`.  
If a bit spills (when shifted to the left, does not have any column to be put in), it it discarded.  
If a number is signed, the sign bit is not changed.  
Example:  
`0b1100101 << 1` == `0b1001010`  
_Note: Mathematically, `LSHIFT` multiplies the number by 2^n_

## RSHIFT
The `bit-wise Right SHIFT` operator `>>` moves all of the bits in a binary number right by `n`.
If a bit spills (when shifted to the right, does not have any column to be put in), it it discarded.
The sign bit is not changed.  
Example:  
`0b1100101 >> 1` == `0b1010010`  
_Note: Mathematically, `RSHIFT` divides the number by `2^n`_

# So what?
**All you need to know is how to add**


Consider the expression `12 - 7`, which is equal to `5`.  
If we did that in binary...
`0b1100 - 0b0111`
```
  0b1100
- 0b0111
    ----
  0b0111
```
When doing binary subtraction, it becomes abit tedious as whenever we have a `0 - 1`, we need to reduce the left bit, and that one might need to reduce its left bit, and so forth...  
This process requires extraneous computer operations, and in efficient.  

**SO INSTEAD**, what if we were to _add_ them?  
`12 - 7` is the same thing as `12 + -7`, both giving us a value of `5`.  

`12` in binary is `0b1100`  
`7` in binary is `0b0111`  
`-7` in two's complement form is `0b1001` (`~0b0111 + 0b1`)  

`12 + -7`  
`0b1100 + 0b1001`
```
  0b1100
+ 0b1001
    ----
  0b0101
```
_Note: The 1 spills over, and is discarded._  
And behold, `0b0101` is equal to `5`!  

Pretty cool, huh!  
By combining the different bit-wise operations, we can do cool stuff  

* Save space - 8 boolean variables could be stored in a single byte!
* Bit masks - We can use the bit-wise AND operator to select a portion of bits in a binary string
* Unix access flags
* They're cool!
* More stuff too!



## Graphic
![Bitwise Operations](/img/bitwise_operations.png)
