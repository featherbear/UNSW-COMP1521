---
title: "Process Management"
date: 2018-09-16T19:41:17+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

A process is an instance of a program that is being executed.

Each process has associated metadata, such as:

* Execution point (PC)
* Register values
* Virtual Address mapping
* File and Socket descriptors

The operating system also keeps extra metadata, such as how long the process has been running for, and its priority

# Multitasking, more like task switching.
A computer cannot do two things at once, rather a lot of things in a negligible amount of time. (Ignore multi-core CPUs).
Each process is given an allocated amount of time to run, before it is 'pre-empted' (paused) and another process is resumed.
During this pre-emption process, the process' state is saved, and the process is marked as suspended, and put into a priority queue to await for its next execution period.

Each process is identified by a Process ID, or PID for short.
