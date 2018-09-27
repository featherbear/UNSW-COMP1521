---
title: "Network Inter-Process Communication"
date: 2018-09-27T09:21:54+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

. . . - - - . . .


> [Read more about networking](../networking)

Networked communication between devices play an important role in the running of systems.  
Sure we could just copy everything onto USBs and CDs, then get them posted to our destination -  
but that's slow... _[or is it????](https://rfc1149.net/rfc1149.html)_

# Sockets
Sockets are the method in which programs connect to programs on other devices.  

The server creates a socket, binding itself to an address, and waits for connections.  
The client creates a socket, connecting to the address of the server, and writes and reads from the socket

`sys/socket.h` provides the functionality for sockets  

`int socket(int Domain, int Type, int Protocol)`  
Domain: `AF_LOCAL`, `AF_INET`  
Type: `SOCK_STREAM`, `SOCK_DGRAM`  
Protocol: See `etc/protocols`  

ret: socket descriptor, or -1 on error

`int bind(int Sockfd, SockAddr *Addr, socklen_t AddrLen)`  

`int listen(int Sockfd, int Backlog)`

`typedef struct sockaddr_in SockAddr`  
`.sin_family` - domain  
`.sin_port` - port  
`.sin_addr` - address  
`.sin_zero[8]` - padding  

`int accept(int Sockfd, SockAddr *Addr, socklen_t *AddrLen)`  

* Blocks until connection request
* Returns new socket descriptor, or -1 on error

`int connect(int Sockfd, SockAddr *Addr, socklen_t AddrLen)`

* Connect to the socket  
* Returns 0, or -1 on error

> [Read more about networking](../networking)