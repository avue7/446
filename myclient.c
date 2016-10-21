// client.c file
// Athit Vue and Chris White
// CSCI 446
// LAB 4
/*****************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{

  struct addrinfo hints;
  struct addrinfo *rp, *result;


  int sockfd, n,s;
  const char *portno;

  /* Changed the size of the char array from 255 to 1024 to include
     more characters in the input.*/
  char buffer[1024];
  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = argv[2];

  memset( &hints, 0, sizeof( hints ) );
  hints.ai_family = AF_INET; //AF_INET(IPv4) and AF_INET6 (IPv6) and AF_UNSPEC for any
  hints.ai_socktype = SOCK_STREAM; //TCP
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ( ( s = getaddrinfo( argv[1], portno, &hints, &result ) ) != 0 ) {
    fprintf( stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror( s ) );
    exit( 1 );
  }

  /* Iterate through the address list and try to connect */
  for ( rp = result; rp != NULL; rp = rp->ai_next ) {
    if ( ( sockfd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
      continue;
    }

    if ( connect( sockfd, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
      break;
    }

       close( s );
  }
  if ( rp == NULL ) {
    perror( "p-counter: connect" );
    exit( 1 );
  }

  freeaddrinfo( result );

  printf("Please enter the message: ");

  bzero(buffer,1024); // Changed the buffer size from 255 to 1024 to include more chars in buffer.
  fgets(buffer,1024,stdin);

  n = send(sockfd,buffer,strlen(buffer),0);//Mod from write to send()
  if (n < 0)
    error("ERROR writing to socket");

  bzero(buffer,1024);
  n = recv(sockfd,buffer,1024, 0); //Mod from read to recv()
  if (n < 0)
    error("Client ERROR reading from socket");
  printf("%s\n",buffer);


  close(sockfd);
  return 0;
}

