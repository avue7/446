/**
 * @file convert4to5.cpp
 * 
 * @author   Athit Vue and Chris White
 * @class    CSCI 446
 * @semester Fall 2016
 * @date     09/19/2016
 * @brief
 *           This program will prompt the the user to 
 *           enter a message. The message will be 
 *           encoded using 4B/5B encoding method and 
 *           display the encoded message on the screen.
 */

#include <iostream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <algorithm>
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::bitset;
using std::cerr;
/* declaration for the conversion function */
string convert4Bto5B(string);

/**
 * The main function will check to see if there 
 * are command line arguments. It will proceed
 * or issue an error appropriately. It will take
 * an input from the user and convert it by calling
 * the convert4Bto5B function.
 *
 * @param argc    Counts the number of arguments.
 * @param argv[]  Vector of the arguments.
 *
 * @return 0  The program exited without an error.
 * @return 1  Error status returned.
 */
int main(int argc, char *argv[])
{  

  //variable declarations
  string messageIn;
  string binaryOut;
  string nibble;
  string encodedOut;

  //Check to see if there are command line arguments.
  //If argument count is more than one, then output 
  //to standard error and return with status 1.
  if(argc != 1) {
    cerr << "Error: Incorrect number of arguments, ";
    cerr << "you need to run this program without any arguments."<<endl;
    cerr << "You entered the following arguments: <";
    for(int i=1; i < argc; i++){
      if(i < argc-1)
        cout << argv[i] << ", ";
      else
        cout << argv[i] << ">." << endl;
    }
    return 1;
  }

  //Prompt the user for input.
  cout << "please enter a message:" << endl;
  getline(cin,messageIn); //Use getline to include space char.
  cout << "your message " << messageIn << ":" << endl;
  cout << "in binary: ";
  //Convert user input to binary using bitset from the std library.
  for(size_t i = 0; i < messageIn.size(); ++i) 
  {
    bitset<8> b(messageIn.c_str()[i]);
    binaryOut += b.to_string();
  }
  //Print the binary string to standard output.
  cout << binaryOut << endl;


  //Grab 4 bits at a time from our binary string and assign them to nibble.
  int j = 4;
  for(unsigned int i = 0; i < binaryOut.length(); i+=4) {
    nibble = binaryOut.substr(i,j);
    //For each nibble we call our conversion function and
    // append the result to encodedOut.
    encodedOut.append(convert4Bto5B(nibble));
  }
  //Print the result for the user.
  cout << "is encode using 4B/5B to: ";
  cout << encodedOut << endl;

  return 0;
}

/**
 * The convert4Bto5B function takes a nibble of
 * binary and encodes it into a 4B/5B string.
 *
 * @param nibble  The string to be encoded.
 *
 * @return Returns an appropiate 5B (converted 4B) string.
 */
string convert4Bto5B(string nibble) {
  //Here is our 4B/5B table. We check the incoming 4 bits 
  //and return the appropriate 5 bit pattern.
  if(nibble == "0000")
    return("11110");
  else if(nibble == "0001")
    return("01001");
  else if(nibble == "0010")
    return("10100");
  else if(nibble == "0011")
    return("10101");
  else if(nibble == "0100")
    return("01010");
  else if(nibble == "0101")
    return("01011");
  else if(nibble == "0110")
    return("01110");
  else if(nibble == "0111")
    return("01111");
  else if(nibble == "1000")
    return("10010");
  else if(nibble == "1001")
    return("10011");
  else if(nibble == "1010")
    return("10110");
  else if(nibble == "1011")
    return("10111");
  else if(nibble == "1100")
    return("11010");
  else if(nibble == "1101")
    return("11011");
  else if(nibble == "1110")
    return("11100");
  else if(nibble == "1111")
    return("11101");
  else{
    cerr << "This should never happen, error. Terminating program." << endl;
    exit(1);}//Should we output an error message here?
	     //It should be impossible to reach this.
}
