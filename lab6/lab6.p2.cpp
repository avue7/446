/**
 * Lab6: program 2
 * 
 * @brief 
 * 	 This program uses the select() system call to 
 * 	 take in standard input and echoes the data to 
 * 	 standard output within a time-interval of 2.
 * 	 If user has not enter any input for 2 seconds, 
 * 	 then the program times out and exits. 
 */

#include <iostream>
#include <stdio.h>  
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> 

using namespace std;

int main(void)
{
  fd_set readfds; //Declaring the readfds set
  struct timeval t_value;  //Instiating a timveval object
  int return_val;
  char buffer[1024];  //Need this buffer for printing
  int read_value;    // Read_value for read() for error
  

  FD_ZERO(&readfds);
  FD_SET(0, &readfds);

  t_value.tv_sec = 2;
  t_value.tv_usec = 0;

  
  while(return_val = select(1, &readfds, NULL, NULL, &t_value))
  {
    if(return_val == -1)
    {
     cerr << "Error. " << strerror(errno) << endl;
     return 1;
    }
    if (return_val)
    {
     if(FD_ISSET(0, &readfds)) //Making sure stdin is in the readfds set.
     {
      read_value = read(0, buffer, 1024);
      cout << "Data recieved is: " << buffer << endl;
      t_value.tv_sec = 2;
     }
     else
     {
      cerr << "STANDARD OUTPUT IS NOT IN THE READFDS SET!" << endl;
      return 1;
     }
    }
  }
  cout << "Time interval ended with no data recieved." << endl;
  
  return 0;
}
