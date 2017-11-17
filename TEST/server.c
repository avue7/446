 /**
 * Athit Vue
 * Chris White
 * CSCI 446
 * Spring 2016
 * Program 3
 *
 * @brief
 *      This program accepts the request from the client and
 *      sends a response to the client. If the server cannot read the
 *      file for any reason, it will send an indication error to the client
 *      and quit. If the server can read the file then it will send the 
 *      file to the client and quit as well. This program takes one 
 *      argument, the port number, in the command line. 
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // Needed for open()

#define MAX_LINE 256
#define MAX_PENDING 5

int
main(int argc, char *argv[])
{
  /* Server end variable declarations */
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  char buf[MAX_LINE];
  int s, new_s;
  char* server_port;
  
  /* Declarations of variables for file */
  int in_file;
  int len;
  int read_status;
  int send_bytes;

  if (argc == 2)
  {
    server_port = argv[1];
  }
  
  /* Build address data structure */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  /* Get local address info */
  if ((s = getaddrinfo(NULL, server_port, &hints, &result)) != 0 )
  {
     /*Note: comment it out since you dont want outputs from the server side */
    //fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s)); 
    exit(1);
  }

  /* Iterate through the address list and try to perform passive open */
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
    {
      continue;
    }
    if (!bind(s, rp->ai_addr, rp->ai_addrlen))
    {
      break;
    }
    close(s);
  }

  if (rp == NULL)
  {
    /* If client is not connected we cannot send these error to client */
    perror("stream-talk-server: bind");
    exit(1);
  }
  if (listen(s, MAX_PENDING) == -1)
  {
    /* If client is not connected we cannot send these error to client */
    perror("Server error: listen");
    close(s);
    exit(1);
  }

  /* Wait for connection, then receive and send file to client */
  while(1)
  {
    if ((new_s = accept(s, rp->ai_addr, &(rp->ai_addrlen))) < 0)
    {
      /* We cannot send this error to client if disconnects. However, client has error checking 
       * for this already */
      perror("Server error: accept"); 
      close(s);
      exit(1);
    }
    bzero(buf, sizeof(buf)); //Clear the buffer of any data
    
    /* Recv the filename of the target file sent from client */
    len = recv(new_s, buf, sizeof(buf), 0);
    if (len == -1)
    {
      /* If we have trouble recv, well attempt to send a message to client */
      send(new_s, "Server error!***", 16, 0);
      exit(1);
    }
    
    /* OPEN THE FILE */
    in_file =  open(buf, O_RDONLY);
    
    if (in_file == -1)
    {
      /* Send error to client that could not open file */
      send(new_s, "Server error!***", 16, 0);
      exit(1);
    }
    bzero(buf, sizeof(buf));

    /* READ THE FILE */
    while((read_status = read(in_file, buf, 1) > 0))
    {
      /* Send the data from the file one char at a time */
      send_bytes = send(new_s, buf, read_status, 0);
      if (send_bytes == -1)
      {
        printf("Error. CANNOT SEND FILE TO CLIENT\n");
        exit(1);
      }
      bzero(buf, sizeof(buf)); //Reset the buffer
    }

    if (read_status == -1)
    {
      send(new_s, "Server error!***", 16, 0);
      exit(1);
    }
    
    /* If read_status is 0, we are at EOF, so we need to close the file */
    if (read_status == 0)
    {
      int close_status;
      close_status = close(in_file);
      if (close_status == -1)
      {
        fprintf(stderr, "Server error: cannot close file.\n");
      }
      break;
    }
  }
    

  freeaddrinfo(result);
  close(in_file);
  close(new_s);
  close(s);

  return 0;
}
