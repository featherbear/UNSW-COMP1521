---
title: "Week Six - stat"
date: 2018-08-30T00:21:11+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

MIPS stuff done, back to C. Yay!

This week the lectures covered the associated metadata of files on a Unix system

Originally the code didn't work for directories outside of the current directory.  
That was because the file path returned from the `readdir` function was a path relative to the directory entry it was given.

Consequently, `lstat` wasn't opening the right file, and gave us wrong results.  
To fix that I just had to pass `lstat` an absolute path.  
  

---

It sort of reminds me of the start of 2015, where a popular application had a major flaw
```
APPROOT="$(cd '${0%/*}' && echo $PWD)"
rm -rf "$APPROOT/"*
```
Issue with this, is that if `$APPROOT` is empty, then `rm -rf "/*"` - and there goes all of our files.  
Better hope you're not running it as root.  
But even if you're not... unlucky.