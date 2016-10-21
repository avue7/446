#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(int argc, char* argv[])
{
 if(argc != 2)
 {
  cout << "Need to specify filename" << endl;
  return 1;
 }

 //Declaration of variables
 int file_temp;
 int read_status;
 int close_status;
 int SIZE = 255;
 char buffer[SIZE];

 //File open 
 file_temp =  open(argv[1], O_RDONLY);
 if (file_temp == -1)
 {
  cerr << "ERROR, CANNOT OPEN FILE." << endl;
  return 1;
 }
 
 //File read
 while(read_status = read(file_temp, buffer, SIZE) > 0) 
 {
  if (read_status == -1)
  {
   cerr << "ERROR, CANNOT READ FILE." << endl;
   return 1;
  }
  cout << buffer;
  memset(buffer, 0, SIZE);
 }
 cout << endl;

 //File close
 close_status = close(file_temp);
 if (close_status == -1)
 { 
  cerr << "ERROR, CANNOT CLOSE FILE." << endl;
  return 1;
 }

 return 0;
}

