/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include "RSACipher.hpp"
using namespace std;


const short PLAIN_INPUT = 0;
const short DECIMAL_INPUT = 1;
const short BINARY_INPUT = 2;
const short HEX_INPUT = 3;

string getTextFromUser()
{
  string outputText = "";
  return outputText;
}
string getTextFromFile()
{
  string outputText = "";
  return outputText;
}

void encryptionMenu(RSACipher& cipher)
{
  cout << "Encryption selected." << endl;
  //Number of iterations

  string menuString = "Text input or input from file? \n1. Text Input"
    "\n2. File Input\n";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  bool smallLoop = true;
  while (smallLoop)
  {
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants to input text
    if (userChoice == "1")
    {
      smallLoop = false;
      string plainText = getTextFromUser();
    }
    //User wants file input
    else if (userChoice == "2")
    {
        smallLoop = false;
        string plainText = getTextFromFile();
    }
    else if (userChoice == "3")
    {
      smallLoop = false;
    }
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

void decryptionMenu(RSACipher& cipher)
{
  cout << "Decryption selected." << endl;
  //Number of iterations

  string menuString = "Text input or input from file? \n1. Text Input"
    "\n2. File Input";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  bool smallLoop = true;
  while (smallLoop)
  {
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants to input text
    if (userChoice == "1")
    {
      smallLoop = false;
      string cipherText = getTextFromUser();
    }
    //User wants file input
    else if (userChoice == "2")
    {
      smallLoop = false;
      string cipherText = getTextFromFile();
    }
    else if (userChoice == "3")
    {
      smallLoop = false;
    }
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

void keyOptMenu(RSACipher& cipher)
{
  cout << "Key Options selected." << endl;
  //Number of iterations

  string menuString = "Select from the following options:\n1. Generate keys"
    "\n2. View keys\n3. Return to main menu";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  bool smallLoop = true;
  while (smallLoop)
  {
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants to input text
    if (userChoice == "1")
    {
      smallLoop = false;
      cipher.genKeys();
    }
    //User wants file input
    else if (userChoice == "2")
    {
        cipher.displayKeyInfo();
    }
    else if (userChoice == "3")
    {
      smallLoop = false;
    }
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

int main()
{
  cout << "FIPS COMPLIANT RSA - GEORGE WOOD" << endl;

  RSACipher cipher;

  //User input choice
  string userChoice;

  //Main menu string
  string menuString = "\nPlease select your desired option: "
    "\n1. Encryption\n2. Decryption\n3. Key Options\n4. Quit";

  //Repeatedly prompts user for desired function until
  //user quits or closes the program.
  bool keepLoop = true;
  while (keepLoop)
  {
    //Displays menu
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User is not quitting, wants q-learning
    if (userChoice == "1")
    {
      encryptionMenu(cipher);
    }
    //User is not quitting, wants SARSA
    else if (userChoice == "2")
    {
      decryptionMenu(cipher);
    }
    else if (userChoice == "3")
    {
      keyOptMenu(cipher);
    }
    else if (userChoice == "4")
    {
      keepLoop = false;
    }
    else
    {
      cout << "Input must be either 1, 2, 3, or 4. Try again. " << endl;
    }
  }
}
