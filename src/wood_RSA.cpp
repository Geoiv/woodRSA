/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include "../head/RSACipher.hpp"
using namespace std;

string globalCipher;

uint inputBaseMenu()
{
  const uint asciiBase = 0;
  const uint decBase = 10;
  const uint binBase = 2;
  const uint hexBase = 16;

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

    //User wants to input text
    if (userChoice == "1")
    {
      return decBase;
    }
    //User wants file input
    else if (userChoice == "2")
    {
        return binBase;
    }
    else if (userChoice == "3")
    {
      return hexBase;
    }
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


bool getTextFromUser(string& outputString)
{
  // uint inputBase = inputBaseMenu();
  return true;
}
bool getTextFromFile(string& outputString)
{
  // uint inputBase = inputBaseMenu();
  return true;
}

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
    else if (userChoice == "3")
    {
      return false;
    }
    else
    {
      cout << "Input must either be 1, 2, or 3. Try again. " << endl;
    }
  }
}

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

void decryptionMenu(RSACipher& cipher)
{
  cout << "Decryption selected." << endl;
  string cipherText = globalCipher;
  string plainText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }

  cipher.decrypt(cipherText, plainText);
  cout << plainText << endl;
}
void signingMenu(RSACipher& cipher)
{
  cout << "Signing selected." << endl;
  string plainText = "07d0";
  string cipherText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }

  cipher.sign(plainText, cipherText);
  cout << cipherText << endl;
}
void authenticationMenu(RSACipher& cipher)
{
  cout << "Authentication selected." << endl;
  string cipherText = globalCipher;
  string plainText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }

  cipher.decrypt(cipherText, plainText);
  cout << plainText << endl;
}

//TODO user input keys, clear keys
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
  cout << "FIPS COMPLIANT RSA 2048 & SHA-224 - GEORGE WOOD" << endl;

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
      signingMenu(cipher);
    }
    else if (userChoice == "4")
    {
      authenticationMenu(cipher);
    }
    else if (userChoice == "5")
    {
      keyOptMenu(cipher);
    }
    else if (userChoice == "6")
    {
      keepLoop = false;
    }
    else
    {
      cout << "Input must be either 1, 2, 3, or 4. Try again. " << endl;
    }
  }
}
