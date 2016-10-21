#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
 if(argc != 2)  //Making sure there is only one argument
 {
  cerr << "Need to specify filename" << endl;
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
 int found;
 char upper_buffer[SIZE];

 //Converting C-style string to reg string
 string new_name(argv[1]);

 //Need to rename the extension of "filename" to .up
 found = new_name.find(".");
 new_name.replace(found,new_name.length(), ".up"); 
 
 //Must convert reg string back to c-style string
 //since first argument for open() must be const char*.
 const char* out_name = new_name.c_str();
 int position = 0;

 //File open with read_only option.
 in_file =  open(argv[1], O_RDONLY);
 if (in_file == -1)
 {
  cerr << "ERROR, CANNOT OPEN INPUT FILE." << endl;
  return 1;
 }

 //File open with new extension. 
 out_file = open(out_name, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
 if (out_file == -1)
 {
  cerr << "ERROR, CANNOT OPEN OUTPUT FILE." << endl;
  return 1;
 }

 /* Read the chars one at a time if error status does not 
    equal -1 or 0. 0 means that there more more things to
    be read in the buffer. Then convert the contents inside
    of the file and call write() to the new extension
    filename. */   
 while(read_status = read(in_file, buffer, SIZE) > 0) 
 {
  if (read_status == -1)
  {
   cerr << "ERROR, CANNOT READ INPUT FILE." << endl;
   return 1;
  }
  upper_buffer = toupper(buffer);
  //Writing to output file
  write_status = write(out_file, buffer, SIZE);
  if (write_status == -1)
  { 
   cerr << "ERROR, CANNOT WRITE TO OUTPUT FILE." << endl;
   return 1;
  }
 }


 //File close In_file
 close_status = close(in_file);
 if (close_status == -1)
 { 
  cerr << "ERROR, CANNOT CLOSE FILE." << endl;
  return 1;
 }

 //File close Out_file
 close_status = close(out_file);
 if (close_status == -1)
 { 
  cerr << "ERROR, CANNOT CLOSE FILE." << endl;
  return 1;
 }

 //Return 0 if program has no error. 
 return 0;
}

