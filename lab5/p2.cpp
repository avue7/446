/* Athit Vue and Chris White
 * CSCI 446
 * LAB 5: p2.cpp
 * Created: 10/05/2016
 */

/**
 * @brief This program uses system call functions 
 * to read a file and write the contents of the file 
 * to a new file with the same name. It also changes 
 * the extension of the file to ".up".
 */
#include <iostream>
#include <errno.h> // Needed for system call error
#include <stdio.h> 
#include <ctype.h> // Needed for c-style conversion
#include <sys/types.h> // Needed for open() and close()
#include <fcntl.h> // Needed for open()
#include <unistd.h> // Needed for read() and close()
#include <string.h>
#include <string>
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
 int out_file;
 int read_status;
 int write_status;
 int close_status;
 int SIZE = 1;
 char buffer[SIZE];
 int position_found;
 char upper_buffer[SIZE]; //Initializing for new upper-case char

/*#######EXTENSION CONVERSION BEGINS########*/
 //Converting C-style string to reg string
 string new_name(argv[1]);

 //Need to rename the extension of "filename" to .up
 position_found = new_name.find(".");

 /* Let's make sure argv[1] contains a "." for specifying the 
  * extension of a file. If it doesn't, let's exit the program and 
  * let the user know that the command line argument must 
  * contain an extension.  */
 if (position_found == std::string::npos)
 {
   cerr << "Error. You must specify the file extension!" << endl;
   return 1;
 }

 new_name.replace(position_found,new_name.length(), ".up"); 
 
 /* Must convert reg string back to c-style string
     since first argument for open() must be const char*. */
  const char* out_name = new_name.c_str();
/*#######EXTENSION CONVERSION ENDS#########*/

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

 /* File open with new extension. Set flags to write only and
  * create if file does not exist. Make sure users have permission to 
  * read the file and write to the file. */
 out_file = open(out_name, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
 if (out_file == -1)
 {
  cerr << "Error. Cannot open write-to file. " << strerror(errno) << "!" << endl;
  return 1;
 }

/*#####READ FILE AND WRITE TO NEW_FILE BEGINS#####*/
/**
 * Read the chars one at a time - if error status does not 
 * equal -1 or 0. 0 means that there more more things to
 * be read in the buffer. Then uppercase the contents of the
 * file and call write() to write the upper-cased contents
 * to the new file. 
*/   
 while(read_status = read(in_file, buffer, SIZE) > 0) 
 {
  if (read_status == -1)
  {
   cerr << "Error. Cannot read input file. " << strerror(errno) << "!" << endl;
   return 1;
  }
  
  /* Please NOTE: argument two for write() is a const void*. To change 
   * the data this pointer is pointing to, we must dereference it first. Then we 
   * can upper-case the letters and set it to another dereference pointer.
   * The pointer points to the first element in the array. It tells us where the 
   * first element is in memory. FORGOT ABOUT THIS from 211! 
   */
  *upper_buffer = toupper(*buffer); //Could not finish in lab because of this!!!

  //cout << upper_buffer; ---for testing upper-case.

  // Write the contents in upper-case letters to the new file.
  write_status = write(out_file, upper_buffer, SIZE);
  if (write_status == -1)
  { 
   cerr << "Error. Cannot write to new file. " << strerror(errno) << "!" << endl;
   return 1;
  }
 }
/*######READ FILE AND WRITE TO NEW_FILE ENDS#####*/

 /**
  * File close (in_file); close the file and release the resource.
  * If close() returns -1, we cannot close the file. Output to 
  * standard error and return 1.
  */
 close_status = close(in_file);
 if (close_status == -1)
 { 
  cerr << "Error. Cannot close input file. " << strerror(errno) << "!" << endl;
  return 1;
 }

 //File close (out_file)
 close_status = close(out_file);
 if (close_status == -1)
 { 
  cerr << "Error. Cannot close output file. " << strerror(errno) << "!" << endl;
  return 1;
 }

 //Return 0 if program has no error. 
 return 0;
}

