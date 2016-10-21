/* Athit Vue and Chris White
 * CSCI 446
 * LAB 5: p1.cpp
 * Created: 10/05/2016
 */

/**
 * @brief This program uses system call functions 
 * to read a file and output the contents of the file 
 * to the terminal screen.
 */
#include <iostream>
#include <errno.h> // Needed for system call error
#include <sys/types.h> // Needed for open() and close()
#include <fcntl.h> //Needed for open()
#include <unistd.h> // Needed for read and close()
#include <string.h>
using namespace std;

int main(int argc, char* argv[])
{
 /* Check to see if there is one and only one argument.
  * If argc doesn't equal two, then output to standard
  * error and return status error 1. */
 if(argc != 2)  //Making sure there is only one argument
 {
  if (argc > 2)
  {
    cerr << "Error. Only one argument is permitted." << endl;
    return 1;
   }
  cerr << "Error. Need to specify a filename." << endl;
  return 1;
 }

 //Declaration of variables
 int in_file;
 int read_status; //For read status checking
 int close_status; //For close status checking
 int SIZE = 1;
 char buffer[SIZE];

 /* File open; open file with filename stored at argv[1].
  * Set the flag for open() to read only for the file and 
  * set the return value (discripter) to variable in_file. */
 in_file =  open(argv[1], O_RDONLY);
 if (in_file == -1)
 {
  // Use strerror() to print the error message instead of a number.
  cerr << "Error. Cannot open input file. " << strerror(errno) << "!" << endl;
  return 1;
 }
 
 /* File read; read the contents of the file and store it 
  * in the buffer. While return status for read() does not
  * equal 0 or -1 then there are more things in the buffer
  * to read. */
 while(read_status = read(in_file, buffer, SIZE) > 0) 
 {
  //If the read_status is -1 then file cannot be read.
  if (read_status == -1)
  {
   cerr << "Error. Cannot read input file. " << strerror(errno) << "!" << endl;
   return 1;
  }
  /* Output the char to standard output in the buffer 
   * one at a time until we we hit end of file. */
  cout << buffer;
  memset(buffer, 0, SIZE);
 }

 /* File close; close the file and release the resource.
  * If close() returns -1 we cannot close the file. Output 
  * standard error and return 1. */
 close_status = close(in_file);
 if (close_status == -1)
 { 
  cerr << "Error. Cannot close output file. " << strerror(errno) << "!" << endl;
  return 1;
 }
 //Return 0 if all is well.
 return 0;
}

