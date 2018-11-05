---
title: "Networking"
date: 2018-09-27T09:38:34+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

whirrrrrrr clunk

# Types of networks
* Local Area Network (WAN) - A physically contained network
* Wide Area Network (WAN) - A geographically dispersed LAN (ie multi-campus networks)
* Internet - A collection of WANs

# Communication over the Network
* Data is split into packets (chunks of data with headers)
* Packets are sent across physical links (wire, radio, fibre)
* Packets pass through multiple nodes (routing)
* Packets reach the destination
* Packets are reordered, verified and buffered
* Data is reassembled

# The Network Stack
* Physical layer - transfer of bits
* Link layer - Ethernet, MAC, CSMA, etc
* Network layer - Routing, IP
* Transport layer - TCP/UDP
* Application layer - Application-defined protocols

# Protocols
A protocol is a set of defined rules that dictate how systems communicate with each other, in terms of both communication, and representation of data

# IP Addresses
IPv4 address are given as 32-bit addresses (dotted quad)  
IPv6 address are given as 128-bit addresses  
`127.0.0.1` (aka `localhost`) is a special loopback address that points back to the device  
IP addresses can be assigned statically, or dynamically via a DHCP server  

# DNS Server
Devices connect to other devices by their IP address.  
Of course, it isn't feasible to memorise every IP address for every website we go to.

As a result, website names were created, so one could type www.google.com instead of 216.58.196.142.

The Domain Name System is responsible for the translation of this hostname into an IP address.  
A DNS server (aka name server) is a database that maps hostnames to IP addresses - They operate with other DNS servers to provide a distributed global lookup table for all the devices in the world.

* Iterated Query - Client contacts each DNS Server it knows to resolve an address
* Recursive Query - DNS Servers contacts another server, propagating the query

![DNS Servers (Summary)](/img/dns_1.png)
![DNS Servers (Subdomains can point to different IPs)](/img/dns_2.png)

## DNS Levels
* Top-level domain (TLD)
* Authoritative 
* Local server

# TCP and UDP
The two main transmission protocols are TCP and UDP

## Transmission Control Protocol
TCP can be considered as a _'call and response'_ protocol. For every packet the server receives, and acknowledgement of that packet (ACK) should be sent back to the client. If something goes wrong (such as a timeout, or a duplicate ACK), the packet would be resent.

TCP also contain checksum mechanisms to ensure the integrity of the data inside the packet.

![Laughs in TCP](/img/tcp_ack_ack_ack.png)


## User Datagram Protocol
UDP can be considered as _'fire and forget'_, as it makes no care if the destination has successfully received the packet or not.

UDP is considerably faster than TCP as there is less overhead from the information in the packet header.

It is preferred over TCP when large amounts of mission-criticial data is sent (ie when playing games, you don't necessarily need every single packet to be received)

## Summary
![TCP vs UDP](/img/tcp_vs_udp.png)

# Ethernet
CSMA - On collision, wait for a random time and retransmit  
Collision - Two packets received at the same time  
Interference - Two packets sent at the same time
