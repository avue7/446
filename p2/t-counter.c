 /**
 * Athit Vue
 * Chris White
 * CSCI 446
 * Spring 2016
 *
 *
 * @brief
 *      This program requests a file from a web server as specified and parses
 *      the file recieved. It will read chunks from the file, specified by the user 
 *      through the command line argument, one chunk at a time and count the
 *      instances of the string until the file has reach EOF. The user must specify
 *      this string (tag) through the command line argument as well. Then, the 
 *      program will display the total number of instances of the string (tag) to 
 *      the user. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* Defining the macros we will be using */
#define SERVER_NAME "www.ecst.csuchico.edu" 
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"

/* Prototyping our readchunck() */
ssize_t readchunck( int sockfd, void *buf, size_t len );

int main(int argc, char *argv[])
{
  /* Declaration of variables */
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int socket_number;
  int len;
  int user_length;
  int sent_bytes;
  int read_bytes;
  int count;

  /* Initialization */
  char *host;
  user_length = atoi(argv[1]); //Need to convert the string to an integer.
  char buffer[1024]; //Setting our buffer to a good size.
  count = 0; //Making sure our count starts at 0.
  
  /* Check the command line arguments for errors */ 
  if (argc==3) 
  {
    /* Must cast our SERVER_NAME variable as a char* to use in our functions */
    host = (char*)SERVER_NAME;
  }
  else 
  {
    if (argc > 3)
    {
      fprintf(stderr, "Invalid number of arguments. Must input three arguments only.\n");
      exit(1);
    }
    if (argc < 3)
    {
      fprintf(stderr, "Invalid number of arguments. Expecting three arguments.\n");
      exit(1);
    }
  }

  /* Translate host name into peer's IP address */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
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
    {
      //printf("Successfully connected to socket: %d\n", socket_number);
      break;
    }
    close(socket_number);
  }
 
  if (rp == NULL)
  {
    printf("Cannot establish a connection. %s.\n", strerror(errno));
    exit(1);
  }

  freeaddrinfo(result); //Free the resources. Release the dynamically allocated structures. 

  /* Send our request to the connected server */
  sent_bytes = send(socket_number, REQUEST, sizeof REQUEST, 0);
  if (sent_bytes == -1)
  {
    printf("Error while sending: %s.\n", strerror(errno));
    exit(1);
  }
  //printf("Sending %i bytes to socket_number %i.\n", sent_bytes, socket_number); /*For debugging*/

/*################### MAIN WHILE LOOP STARTS ###################*/
  while(rp != NULL) //While connection is established and pointer rp to address socket exists.
  {
    bzero(buffer, 1024); // Clear, zero, the bits in the buffer.
    
    /* Call the readchunck function */
    read_bytes = readchunck(socket_number, buffer, user_length);
    
    /* When we have no more bytes to read we have reached the eof */
    if (read_bytes <= 0)
    {
      break;
    }
    
    char* haystack = buffer; // Set the buffer as haystack 
    char* needle = argv[2]; // Set the input from command line argv[2] as the needle
  
    /* Search for the tag from the beg. of the string, that's currently in the buffer, to the end */
    while((haystack = strstr(haystack, needle)) != NULL)
    {
      count++; // We found a match, add it to our count.
      haystack = haystack + strlen(needle); // Search the rest of the string.
    }
    //printf("%s\n", buffer); /*For debugging*/
  }
/*################### MAIN WHILE LOOP ENDS #####################*/

  /* Output count to user */
  if (count == 0)
  {
    // If no matches were found.
    printf("Sorry, no matches found for %s\n", argv[2]);
  }
  else
  {
    printf("Number of %s instances: %i\n", argv[2], count);
  }

  /* Close the socket after we are done with the file. Note: readchunck already takes care of this after
   * its last call returns a 0 byte. */ 
  //close(socket_number);

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
  bzero(buffer, len); // Clear the buffer. 
  recv_bytes = recv(sockfd, buffer, len, 0);
  
  if (recv_bytes == -1) //Error if recv returns -1
  {
    printf("Error: %s.\n", strerror(errno));
    return -1;
  }
  /*If recv return 0, close socket and return recv_bytes. 
    A recv_bytes of zero may mean socket was closed normally */
  else if (recv_bytes == 0) 
  {
    close_status = close(sockfd);
    if (close_status == -1)
    {
      printf("Error while closing: %s.\n", strerror(errno));
      return -1;
    }
    return recv_bytes;
  }
  else if (recv_bytes == len) // If recv returns users length then return the recv_bytes.
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
    while(recv_bytes != len)
    {
      temp_len = len;
      temp_recv_bytes = recv_bytes;
      new_len = temp_len - temp_recv_bytes; // New len = len - len already parse.
      add_bytes = recv(sockfd, (char*)buffer + recv_bytes, new_len, 0);
      
      if (add_bytes == -1)
      {
        printf("Error while calling recv to fill buffer to user's length: %s\n", strerror(errno));
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
      
      //printf("New bytes to add to recv_bytes is: %i\n", add_bytes);  /*For Debugging*/
      //printf("RECV_BYTES IS NOW: %i\n", recv_bytes);                   /*For Debugging*/
    }
    return recv_bytes;
  }
}
