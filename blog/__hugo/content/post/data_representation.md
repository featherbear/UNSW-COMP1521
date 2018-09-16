---
title: "The Representation of Data"
date: 2018-09-14T20:30:54+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

One and zero, one and zero.

# Number Systems
## Binary
Binary numbers are base 2, with the numbers: 0 1  
_The number 1337 can be represented in binary as 0b10100111001_

## Decimal
Decimals are base 10, with the numbers 0 1 2 3 4 5 6 7 8 9

## Octal
Octals are base 8, with the numbers 0 1 2 3 4 5 6 7  
_The number 1337 can be represented in binary as 0o2471_

## Hexadecimal
Hex numbers are base 16, with the numbers 0 1 2 3 4 5 6 7 8 9 A B C D E F  
_The number 1337 can be represented in binary as 0x539_

# Storage of data in computer systems
A computer can't directly store the letter A, the hexadecimal 0x539, or even the number 15!  
A computer deals with everything in binary (a consequence of being an electrical device).  

On a high level, we can store pictures, videos, images, text without ever thinking about binary -  
this is because of the abstractions provided by our operating systems and black magic :)

# Signed and Unsigned numbers
A `signed` number means that the number has a positive or negative sign in front of it.  
An `unsigned` number means that the number is only positive.  

Consider a byte (`char` in C), which has 8 bits.  
The total number of different values it can store is 2^8 = 256.  
A `signed` byte has its values in the -127 - 127 range.  
An `unsigned` byte has its values in the 0 - 255 range.  

Signed or unsigned, the byte still holds 256 different values, the only thing that has changed is the offset of the range.

## Representation
When looking at the binary representation of a `signed` number, the first bit denotes the sign of the number.  
A first bit of `0` dictates a positive number.  
A first bit of `1` dictates a negative number.  

`unsigned char b = 0b00001011 = 11`  
`signed char ddd = 0b00001011 = 11`  
`unsigned char a = 0b10001011 = 139`  
`signed char ccc = 0b10001011 = -117`  

Tip: If the first bit of a `signed` number is 1, subtract `2^(n-1)`, where `n` is the number of bits of that number's data type.

## Consequences
As an `unsigned` number cannot be negative, if we set that number to a negative value, it will turn into its largest positive value.  
(ie `unsigned char a = -1;` will set `char = 255`)

# Two&apos;s Complement
The two's complement method is a way to convert the binary representation of a positive number into the binary representation of its negative value.  

To do so, invert all the bits and add one (`~BITS + 0b1`)

Let's try convert the number 15, `0b00001111`.  
First, flip all the bits: `0b11110000`.  
Now, add one: `0b11110001`.

# Floating-point Numbers
Floating point numbers are numbers that we might consider to be _numbers with decimal places_, such as the number 150.75

## Data Types
In most programming languages, floating point numbers are given the data types:  
`float` - single precision - 32 bits (4 bytes)
`double` - double precision - 64 bits (8 bytes)

### Single Precision
First bit - Sign  
Next 8 bits - Exponent  
Last 23 bits - Fraction  

### Double Precision
First bit - Sign  
Next 11 bits - Exponent  
Last 52 bits - Fraction  

### Exponent
In binary, the exponent of the fraction is represented relative to a bias value B = 2^n - 1.  
Why? Dunno.  
What this means to us is that the exponent is stored as exponent + B
### Example
Convert 150.75 to a floating point number (single precision).  
```
150.75 = 10010110.11
// 1     0   0   1    0   1   1   0 . 1     1   
// 128 + 0 + 0 + 16 + 0 + 4 + 2 + 0 + 1/2 + 1/4)
```

Now we normalise the fraction into scientific notation
1.001011011 x 2^7

Our exponent is 2^7.  
7 = `0b00000111`

Now add on our bias value (127 in this case)
```
  0b00000111
+ 0b01111111
  --------
  0b10000110
```

So the binary we store as the exponent is `0b10000110`.  

Piecing everything together...
```
SIGN | EXP(8)   | FRAC(23)
-----|----------|---------
0    | 10000110 | 00101101100000000000000
```

So we get the result `01000011000101101100000000000000`  

