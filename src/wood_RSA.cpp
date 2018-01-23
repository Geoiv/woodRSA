/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include "./head/RSACipher.hpp"
using namespace std;

string globalCipher;

/*Menu for user to input the base format of their input*/
uint inputBaseMenu()
{

  string menuString = "Please select the format your input is in \n1. Decimal"
    "\n2. Binary\n3. Hexadecimal\n4. ASCII";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  while (true)
  {
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants decimal base input
    if (userChoice == "1")
    {
      return decBase;
    }
    //User wants binary base input
    else if (userChoice == "2")
    {
        return binBase;
    }
    //User wants hexidecimal base input
    else if (userChoice == "3")
    {
      return hexBase;
    }
    //User wants ascii input
    else if (userChoice == "4")
    {
      return asciiBase;
    }
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

/*Gets text input from user*/
bool getTextFromUser(string& outputString)
{
  // uint inputBase = inputBaseMenu();
  return true;
}
/*Gets text input from files*/
bool getTextFromFile(string& outputString)
{
  // uint inputBase = inputBaseMenu();
  return true;
}

/*Gets user choice of whether to input data manually or from a file*/
bool textInputMenu(string& outputString)
{
  string menuString = "Text input or input from file? \n1. Text Input"
    "\n2. File Input\n3. Cancel input";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  while (true)
  {
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants to input text
    if (userChoice == "1")
    {
      if(!getTextFromUser(outputString))
      {
        cout << "Reading text from user input failed!" << endl;
        return false;
      }
      return true;
    }
    //User wants file input
    else if (userChoice == "2")
    {
      if(!getTextFromFile(outputString))
      {
        cout << "Reading text from file failed!" << endl;
        return false;
      }
      return true;
    }
    //User wants to return to main menu
    else if (userChoice == "3")
    {
      return false;
    }
    //Invalid input
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

/*Displays menu for encryption*/
void encryptionMenu(RSACipher& cipher)
{
  cout << "Encryption selected." << endl;
  string plainText = "07d0";
  string cipherText = "";
  // if(!textInputMenu(plainText))
  // {
  //   cout << "Text input failed or was cancelled." << endl;
  // }

  cipher.encrypt(plainText, cipherText);

  globalCipher = cipherText;
  cout << cipherText << endl;
}

/*Displays menu for decryption*/
void decryptionMenu(RSACipher& cipher)
{
  cout << "Decryption selected." << endl;


  string cipherText = globalCipher;
  string plainText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }

  string menuString = "\nPlease select your desired option: "
    "\n1. Standard Decryption\n2. CRT Decryption\n";

  //User input choice
  string userChoice;

  //Boolean representing whether CRT is to be used or not
  bool crtFlag;

  //Finds if user wants standard or CRT decryption
  bool smallLoop = true;
  while (smallLoop)
  {
    //Displays menu

    cout << menuString << endl;
    cout << "Choice: ";
    getline(cin, userChoice);
    if (userChoice == "1")
    {
      crtFlag = true;
      cout << "Standard decryption selected. " << endl;
      smallLoop = false;
    }
    //User wants CRT
    else if (userChoice == "2")
    {
      crtFlag = false;
      cout << "CRT decryption selected. " << endl;
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1 or 2. Try again. " << endl;
    }
  }

  cipher.decrypt(cipherText, plainText, crtFlag);

  cout << plainText << endl;
}

/*Displays menu for signing*/
void signingMenu(RSACipher& cipher)
{
  cout << "Signing selected." << endl;
  string plainText = "07d0";
  string cipherText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }
  string menuString = "\nPlease select your desired option: "
    "\n1. Standard Signing\n2. CRT Signing\n";

  //User input choice
  string userChoice;

  //Repeatedly prompts user for desired function until
  //user quits or closes the program.
  bool smallLoop = true;
  while (smallLoop)
  {
    //Displays menu
    cout << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);
    bool crtFlag;
    //User wants standard
    if (userChoice == "1")
    {
      crtFlag = true;
      cout << "Standard Signing selected. " << endl;
      cipher.sign(plainText, cipherText, crtFlag);
      smallLoop = false;
    }
    //User wants CRT
    else if (userChoice == "2")
    {
      crtFlag = false;
      cout << "CRT Signing selected. " << endl;
      cipher.sign(plainText, cipherText, crtFlag);
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1 or 2. Try again. " << endl;
    }
  }
  globalCipher = cipherText;
  cout << cipherText << endl;
}

/*Displays menu for authentication*/
void authenticationMenu(RSACipher& cipher)
{
  cout << "Authentication selected." << endl;
  string cipherText = globalCipher;
  string plainText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }

  cipher.auth(cipherText, plainText);
  cout << plainText << endl;
}

/*Displays menu for keys*/
void keyOptMenu(RSACipher& cipher)
{
  cout << "Key Options selected." << endl;
  //Number of iterations

  //Prompt message
  string menuString = "Select from the following options:\n1. Generate keys"
    "\n2. View keys\n3. Return to main menu";

  //User input choice
  string userChoice;

  //Loops until valid input is given
  bool smallLoop = true;
  while (smallLoop)
  {
    cout << endl << menuString << endl;
    cout << "Choice: ";
    //Gets user menu choice
    getline(cin, userChoice);

    //User wants to input text
    if (userChoice == "1")
    {
      cipher.genKeys();
    }
    //User wants file input
    else if (userChoice == "2")
    {
        cipher.displayKeyInfo();
    }
    //User wants to return to main menu
    else if (userChoice == "3")
    {
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

/*Displays menu for overall RSA algorithm*/
int main()
{
  cout << "FIPS COMPLIANT RSA 2048 & SHA-224 - GEORGE WOOD" << endl;

  //The cipher to be used for any upcoming encyrption or decryption
  RSACipher cipher;

  //User input choice
  string userChoice;

  //Main menu string
  string menuString = "\nPlease select your desired option: "
    "\n1. Encryption\n2. Decryption\n3. Signing \n4. Authentication"
    "\n5. Key Options\n6. Quit";

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

    //User wants to encrypt
    if (userChoice == "1")
    {
      encryptionMenu(cipher);
    }
    //User wants to decrypt
    else if (userChoice == "2")
    {
      decryptionMenu(cipher);
    }
    //User wants to sign
    else if (userChoice == "3")
    {
      signingMenu(cipher);
    }
    //User wants to authenticate
    else if (userChoice == "4")
    {
      authenticationMenu(cipher);
    }
    //User wants to view key options
    else if (userChoice == "5")
    {
      keyOptMenu(cipher);
    }
    //User wants to quit
    else if (userChoice == "6")
    {
      keepLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1, 2, 3, or 4. Try again. " << endl;
    }
  }
}
