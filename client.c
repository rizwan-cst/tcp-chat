#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>




/*
CLient Script 
3rd iteration of tcp-chat client & server!
Based on previous iteration, the client  could only send  1 message before terminating the session.
This new iteration expands on the previous build to incorperate threading.
This was a new concept but it allowed me to enable two way communication with the feature to termiate the session when needed to.
I'm sure there is an easier way to enable two way communicaiton without threading...
     possibly use  gai error instead of perror!
*/


/*
A worker thread function responsible for handling the communication for the server
In contrast to the servver script, it just uses it's own network_socket which was created during socket()
Our other socket (network_socket) is listening for incoming connections
*/


void *communication_thread(void *network_socket) {

char message[1024];
char receive[1024];
bool flag = true;


/* 
the flag's job to check whether the client wants to quit at any given moment.
while the flag is true, the program to continue to allow 2 way communication.
 */

while(flag) {

//sending portion of the script

printf("sending message...\n");

printf("Enter in a message:\n ");

fgets(message, sizeof(message), stdin);

message[strcspn(message, "\n")] = 0;


/*
program is designed so that client initates a response and and server catches it first before sending anything back
worker thread function can only have void * passed into, since the recv() and send()
requires an int as the socket file descriptor for the first parameter, we need to
defereference and cast the value as an int
 */


int change_void = *(int *)network_socket;

int sending = send(change_void, message, strlen(message), 0);


if(sending == -1) {

       perror("failed to send message...\n");
       exit(1);

} else if (sending > 0) {


message[sending] = '\0';


//check to see if the client typed 'quit' to exit program 
//strcmp compare two strings and 0 indicated success


if(strcmp(message, "quit") == 0) {

      printf("you have exited the program...\n");
      flag = false;
      close(change_void);
      break;

      }

 } else {
 
 break;
 
 }


//receiving portion of the script


printf("receiving message...\n");


int change_void2 = *(int *)network_socket;

int receiving = recv(change_void2, receive, sizeof(receive), 0 );

if (receiving == -1) {

    perror("failed to receive message...\n");
    exit(1);

  } else if (receiving == 0) {

    perror("connection terminated...\n");
 
  } else if (receiving > 0) {
  
//received string needs to be '\0'
//This is because send() recv() do not automatically null terminate it        
    receive[receiving] = '\0';
   
    printf("message received: %s\n", receive);

  } else {

    break;

   }
 
 }
      return NULL;
}

//main thread

int main() {

//*reference is a pointer to struct addrinfo hints


struct addrinfo hints;
struct addrinfo *reference;

char receive[1024];


//setting the client configuration, tcp, ipv4/ipv6, listening on 8080...
//both sockets must be listening on the same ports


printf("configuring host...\n");

//memeset() is used to clear up the function and ensure no garbage memory is present
memset(&hints, 0, sizeof(hints));

hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;

int configuration = getaddrinfo("127.0.0.1", "8001", &hints, &reference);

//error handling to check whether the function failed (this will appear mulitple times throughout the code)

if(configuration != 0) {

   
      perror("failed to configure host...\n");
      exit(1);

  }


//reference pointer to the struct hints members
//creating our network socket

printf("creating socket...\n");  
int network_socket = socket(
        reference->ai_family,
        reference->ai_socktype,
 	reference->ai_protocol
	);

if(network_socket == -1) {
     
     perror("failed to create socket..,\n");
     exit(1);
  }

//A client script does not the bind() but instead connect() yet it's still the same parameters
printf("establishing connection...\n");
int connecting = connect(
		network_socket,
		reference->ai_addr,
		reference->ai_addrlen
	        );

if(connecting == -1) {
    perror("failed to establish connection...\n");
    exit(1);
  }



//Initializing our worker thread
pthread_t texting_thread;


//creating our worker thread using teh communcation_thread function
//4th argument is parameter that our communication_thread uses/gets passed in
pthread_create(&texting_thread, NULL, communication_thread, (void *)&network_socket);


//forces main to wait untill this thread has finished running
pthread_join(texting_thread, NULL);





//release the memory used by the getaddrinfo function stored in *reference
freeaddrinfo(reference);

//close our main socket
 close(network_socket);

  
return 0;

}










