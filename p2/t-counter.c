/**
 * Athit Vue
 * Chris White
 * CSCI 446
 * Spring 2016
 *
 *
 * @brief
 *      This template provides the framework you need for program 1. Complete the functions
 *      defined and create any you feel are neccessary. stream-talk-client.c on Learn will
 *      prove useful.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#define SERVER_NAME "www.ecst.csuchico.edu"
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"


/***************************************************************
 * Arguments: same as recv(2)
 * Return:-1 on error or bytes received
 *
 * Receive len bytes of data into array buf from socket sockfd. Always receive len bytes
 * of data unless the socket closes. The return value will fall into one of three cases:
 *   (1) on success the return value equals len
 *   (2) on error the return value equals -1
 *   (3) if the socket closes without receiving len bytes, then return the number
 *       actually received, which may be zero
 ***************************************************************/
ssize_t readchunck( int sockfd, void *buf, size_t len );

int main(int argc, char *argv[])
{
  /* Declaration */
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int socket_number;
  int len;
  int user_length = 0;
  int sent_bytes;
  int read_bytes;
  int count;

  /* Initialization */
  char *host = (char*)SERVER_NAME;
  user_length = atoi(argv[1]);  
  char buffer[1024];
  count = 0;
 /* if (argc==2) 
  {
    host = argv[1];
  }
  else 
  {
    fprintf(stderr, "usage: %s host\n", argv[0]);
    exit(1);
  }
*/

  /* Translate host name into peer's IP address */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ((socket_number = getaddrinfo(host, SERVER_PORT, &hints, &result)) != 0 ) 
  {
    fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(socket_number));
    exit(1);
  }

  /* Iterate through the address list and try to connect */
  for (rp = result; rp != NULL; rp = rp->ai_next) 
  {
    if ((socket_number = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 ) 
    {
      continue;
    }
    if (connect(socket_number, rp->ai_addr, rp->ai_addrlen) != -1)
==    {
      printf("Successfully connected to socket: %d\n", socket_number);
      break;
    }
    close(socket_number);
  }
 
  if (rp == NULL)
  {
    perror("stream-talk-client: connect");
    exit(1);
  }

  freeaddrinfo(result);

  sent_bytes = send(socket_number, REQUEST, sizeof REQUEST, 0);

  //readchunck(socket_number, buffer, user_length);
  
  while((read_bytes = readchunck(socket_number, buffer, user_length)) > 0)
  {
    printf("Readchunck bytes returned is: %d\n", read_bytes);
    /*if (read_bytes != user_length)
    {
      printf("Readchunck bytes returned is: %d ------------------------>>> NOT EQUAL\n", read_bytes);
      printf("%s\n", buffer);
    }*/
    printf("%s\n", buffer);
    while(char* found = strstr(buffer, argv[2]) != NULL)
    {
    
     }
    if (found != NULL)
    {
      count = count + 1;
    }
  
    printf("COUNT IS : %d\n", count);
    //bzero(buffer, user_length);
  }

  //printf("Readchunck bytes returned is: %d\n", read_bytes);
  
  close(socket_number);
  return 0;
}



ssize_t readchunck( int sockfd, void *buffer, size_t len ) 
{
  /* Define readchunck to return exactly len bytes unless an error occurs or the socket closes. */
  int recv_bytes;
  recv_bytes = recv(sockfd, buffer, len, 0);
  char* new_buffer = (char*)buffer;
  /*while(recv_bytes = recv(sockfd, buffer, len, 0) > 0)
  {
    char* new_buffer = (char*)buffer;
    printf("%c", *new_buffer);
  }*/

  
  if(recv_bytes != len)
  {
    int add_to_bytes;
    int needed_len;
    needed_len = len - recv_bytes;
    char add_to_buffer[needed_len];
    add_to_bytes = recv(sockfd, add_to_buffer, needed_len, 0);
    recv_bytes = recv_bytes + add_to_bytes;
    strcat(new_buffer, add_to_buffer);
    buffer = new_buffer;
    //new_buffer[len+needed_len] = 
    //*buffer = add_to_buffer;
  }
  printf("Add to buffer : %c\n", *new_buffer);
  printf("Bytes returned inside readchunck() after adding: %d\n",recv_bytes);

  return recv_bytes;
}
