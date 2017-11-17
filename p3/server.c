/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */
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

  if (argc < 1)
  {
    fprintf(stderr, "Invalid Number of Arguments.\n");
    exit(1);
  }
  if (argc > 1)
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
    fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s));
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
    perror("Server error: bind");
    exit(1);
  }
  if (listen(s, MAX_PENDING) == -1)
  {
    perror("Server error: listen");
    close(s);
    exit(1);
  }

  /* Wait for connection, then receive and print text */
  while(1)
  {
    if ((new_s = accept(s, rp->ai_addr, &(rp->ai_addrlen))) < 0)
    {
      perror("Server error: accept");
      close(s);
      exit(1);
    }
    bzero(buf, sizeof(buf));
    len = recv(new_s, buf, sizeof(buf), 0);
    if (len == -1)
    {
      printf("error reading from client");
    }
    /*fputs(buf, stdout);
      
    if (len > 0)
    {
      printf("LENS_RECV = %i\n", len);  //Debugging for right size recieved
      printf("%s\n", buf);
      send_bytes = send(new_s, "I got your message:", 18, 0);
      if (send_bytes < 0)
      {
        perror("Error writing to socket");
      }
    }*/
    
    /* OPEN THE FILE */
    in_file =  open(buf, O_RDONLY);
    /*if (in_file > 0)
    {
      printf("I WAS ABLE TO OPEN THE FILE ALRITE\n");
    }*/
    if (in_file == -1)
    {
      // Use strerror() to print the error message instead of a number.
      printf("Error. Cannot open input file: %s\n", buf);
      exit(1);
    }
    bzero(buf, sizeof(buf));

    /* READ THE FILE */
    while((read_status = read(in_file, buf, sizeof(buf)) > 0))
    {
      //printf("I WAS ABLE TO READ THE FILE ALRITE\n");
      //printf("%s\n", buf);
      //printf("READ STATUS IS: %d\n", read_status);
      //printf("SIZE OF BUFFER BEFORE SENT: %li\n", sizeof(buf));
      send_bytes = send(new_s, buf, sizeof(buf), 0);
      //printf("Sent bytes: %d\n", send_bytes);
      //printf("SIZE OF BUFFER IS: %li\n", sizeof(buf));
      /*if (send_bytes >= 0)
      {
        printf("I SENT THE FILE TO THE CLIENT ALRITE!\n");
      }*/
      if (send_bytes == -1)
      {
        printf("Error. CANNOT SEND FILE TO CLIENT\n");
        exit(1);
      }
      bzero(buf, sizeof(buf));
    }
      
    //printf("%d\n", read_status);
    if (read_status == -1)
    {
      printf("Error. Cannot read input file: %s\n", buf);
      exit(1);
    }
    if (read_status == 0)
    {
      break;
    }
  }
    

  freeaddrinfo(result);
  close(in_file);
  close(new_s);
  close(s);

  return 0;
}
