// Athit Vue and Chris White
// CSCI 446
// LAB 4

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <string.h>

#define BACKLOG 20


void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, s, n;
  const char *portno;
  char buffer[1024]; //Change from 255 to 1024 to include more chars.

  struct addrinfo hints, *res;
  struct sockaddr client_addr;
  socklen_t sin_size;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  portno = (argv[1]) ;

  memset(&hints, 0, sizeof( hints ));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  if ((s = getaddrinfo(NULL, portno, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == \
-1) {
    perror("server: socket");
    exit(1);
  }
 if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("server: bind");
    exit(1);
  }

  freeaddrinfo(res); // all done with this structure

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  while(1) {
    sin_size = sizeof client_addr;

    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);//accept client fd
    if (newsockfd == -1) {
      perror("accept");
    }

    bzero(buffer,1024); // Changed from 255 to 1024 to include more chars.
    n = recv(newsockfd,buffer,1024,0); //Mod from read to recv
    if (n < 0) error("ERROR reading from socket");
 
     printf("Here is the message: %s\n", buffer);
  

    n = send(newsockfd,"I got your message:",18, 0);//Mod from write to send()
    if (n < 0) error("ERROR writing to socket");
    }

  close(newsockfd);
  close(sockfd);
  return 0;
}
