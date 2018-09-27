---
title: "Week Eight - Devices"
date: 2018-09-23T18:36:02+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Devices
In Unix, devices can be accessed via a filepath!  
Have a try and look at `ls /dev`!

Though we cannot directly access the contents by just reading the contents of the (_or can you_), this convention allows programs to be function more synonymously as a 'file'.  

## Unix Devices
`/dev/diskN` - Hard disk partition  
`/dev/ttyN` - Terminal device  
`/dev/ptyN` - Pseudo-terminal device  

`/dev/mem` - Physical memory (RO)  
`/dev/null` - Data sink  
`/dev/random` - Pesudo-random number stream  


## Special Devices
### Character Devices
* Unbuffered direct access to hardware device
* Read and write by bytes

### Block Devices
* Buffered access to hardware device
* Read and write by chunks of bytes at a time

# [Local IPC (Click to view)](../local_ipc)
# [Network IPC (Click to view)](../network_ipc)


> [Also, assignment 2 was released...](../ass02)
