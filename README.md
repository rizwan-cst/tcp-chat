# Tcp Chat Client & Server

tldr: A simple chat program that uses the tcp protocol to facilitate communication between a client and server. 

## Overview

Have you ever sent or received an email and wondered how it got delivered to you in one piece without any contents missing? Well you're in luck because whether you are moving files, sending emails, or web browsing, it's all reliant on the Transmission Control Protocol (TCP).

TCP operates on 3 way handshake between the client and server which is essentially just 3 messages. Instead of going through the intricacies of tcp, I just want to demonstrate how the protocol works on a fundamental level. To understand how tcp is reliable and able to securely transfer data without loss, 2 terms need to be understood. A SYN packet is a piece of data that is used to initiate a connection. An ACK is also a piece of information but it is used to send acknowledgements to the recipient similar to replies.To simplify things in a nutshell, let's examine the figure below:

Client ---> Server - Client sends a SYN packet to initiate a connection  
Client <--- Server - Server responds with their own SYN packet and an ACK which is just an acknowledgement  
Client ---> Server - Client acknowledges and a connection is established.

Here's where this program comes into play. This is a simple tcp chat program that allows for a server and client to communication with each other through a terminal. The source code provided is well documented with comments to allow the user to understand the innerworkings of the program. 

## Prerequisites

The program itself is divided into 2 scripts, 1 for the called 'server.c' and another for the client called 'client.c'. This program will ONLY run on Linux, apologies for any Windows users out there. The reasoning being is that the entire infrastructure code relies on Linux, such as the POSIX Socket API which allows the client and server to communicate using sockets. More details on the actual code itself will in the Technical Breakdown section. Please ensure that your machine has a minimum of 10.6 total KiB of space for both files. Once downloaded, follow the instructions:

1: Copy both downloaded files into a new directory (This directory will be used to compile and run both scripts later on)  
2: Using a terminal, switch into the new directory to confirm both files are inside using ls -l  
3: For the purpose of this demonstration, the GCC will be used (GNU Compiler Collection) to compile both programs.  
4: run the following command to compile the server script: gcc server.c -o server -lpthread  
5: run the following command to compile the client script: gcc client.c -o client -lpthread  
6: Execute the compiled server script first: ./server (Note that the order in which you execute the program is important, executing the client script before the server will result in an error.)  
7: Execute the compiled client script next: ./client 

At this point 2 way communication between the client and server should be enabled. This program limits sending messages one at a time, meaning the server and client cannot send messages simultaneously with each other. To exit the program, have the client send a message with the word 'quit', this should automatically terminate the session on both ends. If you terminate the session and immediately try to initate another one, you may get this error: "Address in use". This is a normal ocucrance, wait up to 2 minutes for it to dissapear.   

## Technical Breakdown

Alrighty so over here is where breakdown of the code is going to be discussed. The majority of the code base itself is built upon multiple functions found in various headers. 

`#include <pthread.h>` - Used to implement threading in our program pthread_create(), pthread_join()  
`#include <stdbool.h>` - A bool designed as a flag to check whether the client has quit the program  
`#include <stdio.h>` - Standard input/output printf(), perrror()  
`#include <stdlib.h>` - releasing memory from functions malloc()  
`#include <string.h>` - Specific string function strcmp() used to compare the client message to check if they typed quit  
`#include <sys/types.h>` - Incorperates specific datatypes Network  
`#include <sys/socket.h>` - Essential socket API function socket(), bind() listen()  
`#include <netdb.h>` - Configuring host getaddrinfo()  
`#include <unistd.h>` - Socket operations close()  
`#include <stdint.h>` - Ensuring specific data sizes, sock_t

As seen from the header files, this program implements multi-threading to allow 2 way communication between both nodes. Outside of the main function, a separate worker thread is created to handling the sending and receiving portion of script. Both portions are encased in a while loop to allow communication so long as the client does not type in quit. Since the program implements threading, an additional parameter needs to added when compiling the program: gcc server.c -o server '-lpthread'. 

## Resources

This project couldn't have been done without the help of the following resources:  
GeeksforGeeks - Understanding TCP 
Beej's Guide to Network Programming  
Socket Programming - Dan Gookins  
Socket Programming - Jacob Sorber
