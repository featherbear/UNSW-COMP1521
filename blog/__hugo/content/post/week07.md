---
title: "Week Seven - Virtual Memory"
date: 2018-09-05T00:15:52+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Computers are pretty cool, eh?  

So in managing the memory of a system, there is this concept of pages and frames.
Essentially, the pages are memory mappings to arbitrarily orderd frames. When a program requests a specific memory, the page table redirects the program to a physical memory address dictated by the frame that that page corresponds to.

It reminds me of the VirtualStore system on Windows-based systems.  
It's like a sandbox - the program has access to request any memory range. However the actual memory address it will access will be different.
