 /**
 * Athit Vue
 * Chris White
 * CSCI 446
 * Spring 2016
 * Program 3
 *
 *
 * @brief
 *      This program requests a file from a server as specified from 
 *      the command line and sends the filename to the server specified
 *      in the command line. The server will respond and this client program
 *      will process the server's response. If a server indicates an error this 
 *      program will display the error to the user. Upon success, the program
 *      will save the file to its local directory and quit. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> // Needed for open()
#define MAX_LINE 256

/* Prototyping our readchunck() */
ssize_t readchunck( int sockfd, void *buf, size_t len );

int main(int argc, char *argv[])
{
  /* Declaration of global variables */
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  char *host;
  char buf[MAX_LINE];
  int s;
  int len;
  int send_bytes;
  int read_bytes;
  char* server_port;
  char* request;
  int out_file;
  int write_status;
  int close_status;
  int recv_bytes;
  
  /* Check to see if argument count == 4. */
  if (argc==4)
  {
    host = argv[1];
    server_port = argv[2];
    request = argv[3];
  }
  else if (argc < 4)
  {
    fprintf(stderr, "Client error: Must specify three arguments. \n");
    exit(1);
  }
  else
  {
    fprintf(stderr, "Client error: Too many arguments. Must specify only three. \n");
    exit(1);
  }

  /* Translate host name into peer's IP address */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ((s = getaddrinfo(host, server_port, &hints, &result)) != 0 )
  {
    fprintf(stderr, "Client error: Unable to find host '%s'\n", argv[1]);
    exit(1);
  }

  /* Iterate through the address list and try to connect */
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
    {
      continue;
    }
    if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1)
    {
      break;
    }
    close(s);
  }
  
  if (rp == NULL)
  {
    perror("Client error");
    exit(1);
  }
  freeaddrinfo(result);

/*##############SEND OUR FILE REQUEST#################*/
  request[MAX_LINE-1] = '\0';
  len = strlen(request);
  send_bytes = send(s, request, len, 0);
  if (send_bytes == -1)
  {
    fprintf(stderr, "Client error: Unable to send request\n");
    exit(1);
  }
/*###############################################*/  
  
  bzero(buf, sizeof(buf));
  
  /* Attempting check to see if server recv the filename */
  recv_bytes = recv(s, buf, 16, 0);
  if (recv_bytes == -1)
  {
    fprintf(stderr, "Server error: could not recieve from server.\n");
    exit(1);
  }
  
   /*If server sends message 'Server error!***', there is a possible error that either
   * the server could not open or read the file requested */
  char* haystack = buf;
  char* needle = "Server error!***";
  if (strstr(haystack, needle) != NULL)
  {
    fprintf(stderr, "Server error: Unable to access file '%s'\n", request);
    exit(1);
  }
  else
  {
    /* Need to open a local file with requested filename if it exists, if it doesn't then 
   * we need to create a new file. Give the user permission to read and write*/
    out_file = open(argv[3], O_RDWR| O_CREAT, S_IRUSR | S_IWUSR);
    if (out_file == -1)
    {
      fprintf(stderr, "Client error: Cannot open write-to file.\n");
      exit(1);
    }
    write_status = write(out_file, buf, recv_bytes);
    if(write_status == -1)
    {
      fprintf(stderr, "Client error: Cannot write '%s' to local directory.", request);
      exit(1);
    }
  }
    
/*############RECIEVE THE FILE REQUESTED IF ANY##########*/
  /* While connection is established and pointer rp to address socket exists */
  while(rp != NULL) 
  {
    bzero(buf, sizeof(buf));
    /* Call the readchunck function */
    read_bytes = readchunck(s, buf, MAX_LINE);

    /* Write the contents of the file from the server to the opened file in the client */
    write_status = write(out_file, buf, read_bytes);
    if(write_status == -1)
    {
      fprintf(stderr, "Client error: Cannot write '%s' to local directory.", request);
      exit(1);
    }

    /* When we have no more bytes to read we have reached the eof */
    if (read_bytes <= 0)
    {
      close_status = close(out_file); // Close the file we just wrote to
      if (close_status == -1)
      {
        fprintf(stderr, "Client error: Cannot close the write-to file");
        exit(1);
      }
      break;
    }
    if (read_bytes == -1)
    {
      fprintf(stderr, "Server error: Unable to send requested data from '%s'\n", request);
      exit(1);
    }
  }
/*##############################################*/
  
  /* Close the socket when we are done transfering the file data */
  close(s);
  return 0;
}

/**
 * readchunck
 * 
 * Receive len bytes of data into array buffer from socket sockfd. Always receive len bytes
 * of data unless the socket closes. The return value will fall into one of three cases:
 *   (1) on success the return value equals len
 *   (2) on error the return value equals -1
 *   (3) if the socket closes without receiving len bytes, then return the number
 *       actually received, which may be zero
 *
 * @param sockfd        The socket number we are currently using.
 * @param buffer         A pointer to the first element of the buffer.
 * @param len             The length that the user specified.
 *
 * @return recv_bytes  The number of bytes that is returned by recv().
 * @return -1                An error has occured.
 */
ssize_t readchunck( int sockfd, void *buffer, size_t len ) 
{
  /* Define readchunck to return exactly len bytes unless an error occurs or the socket closes. */
  int recv_bytes;
  int close_status;
  int length = (int)len; // Casting len as int.
  bzero(buffer, len); // Clear the buffer. 
  recv_bytes = recv(sockfd, buffer, length, 0);
  
  if (recv_bytes == -1) //Error if recv returns -1
  {
    fprintf(stderr, "Server error: server was unable to send data.\n");
    return -1;
  }
  /*If recv return 0, close socket and return recv_bytes. 
    A recv_bytes of zero may mean socket was closed normally */
  else if (recv_bytes == 0) 
  {
    close_status = close(sockfd);
    if (close_status == -1)
    {
      printf("Client error while closing: %s.\n", strerror(errno));
      return -1;
    }
    return recv_bytes;
  }
  else if (recv_bytes == length) // If recv returns users length then return the recv_bytes.
  {
    return recv_bytes;
  }
  else
  {
    /* Declaring some temp variables */
    int new_len;
    int add_bytes;
    int temp_len;
    int temp_recv_bytes; 
    
    /* If recv_bytes != len, then we must keep calling it to get the rest of the bytes.
     * **Note: general purpose void* buffer points to the first element of the buffer 
     * only, so when we store the data into the buffer we must remember it's last
     * index where we stored the data. Make sure we don't over-lap the data we 
     * have aready stored on the next call. Need to add an offset to the buffer pointer
     * when we call recv again. */
    while(recv_bytes != length)
    {
      temp_len = length;
      temp_recv_bytes = recv_bytes;
      new_len = temp_len - temp_recv_bytes; // New len = len - len already parse.
      add_bytes = recv(sockfd, (char*)buffer + recv_bytes, new_len, 0);
      
      if (add_bytes == -1)
      {
        fprintf(stderr, "Server error:  server could not finish sending data.\n");
        return -1;
      }
      /* If we have 0 bytes to add, that means we are at EOF. Break from loop
       * This can also be added to the while loop condition, but it's easier to read
       * from here */
      if (add_bytes == 0)
      {
        break;
      }
      
      recv_bytes = recv_bytes + add_bytes;  // Update the recv_bytes to include new bytes recv. 
    }
    return recv_bytes;
  }
}


