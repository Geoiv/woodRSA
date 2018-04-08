/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include "./head/RSACipher.hpp"
using namespace std;
//TODO valid file name
/*Allows user to change between the two FIPS-compliant key sizes*/
void keySizeMenu(RSACipher& cipher)
{
  const string menuString = "\nPlease select your desired option: "
    "\n1. 2048-bit key\n2. 3072-bit key\n3. Cancel change";

  uint choice1 = 2048;
  uint choice2 = 3072;
  //User input choice
  string userChoice;

  cout << "WARNING: Changing key sizes will reset current key information. If "
          "current key information needs to be preserved, cancel the change "
          "and ensure that it is saved first.";
  //Repeatedly prompts user for desired function until
  //user quits or closes the program.
  bool smallLoop = true;
  while (smallLoop)
  {
    //Displays menu
    cout << menuString << endl;
    cout << "Choice: ";
    getline(cin, userChoice);
    //User wants a 2048-bit key
    if (userChoice == "1")
    {
      cipher.changeKeySize(choice1);
      smallLoop = false;
    }
    //User wants a 3072-bit key
    else if (userChoice == "2")
    {
      cipher.changeKeySize(choice2);
      smallLoop = false;
    }
    //User wants to cancel key size change
    else if (userChoice == "3")
    {
      cout << "Key size change cancelled." << endl;
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1, 2, or 3. Try again." << endl;
    }
  }
}

/*Reads and returns list of file names of keys stored. Only gets files with
  keys of same length as is currently used by the cipher*/
vector<string> readKeyDirectory(string keyDir)
{
  //Vals to prevent current directory and parent directory from being added
  //to directory list
  const string curDir = ".";
  const string parDir = "..";
  //Vector to hold file names
  vector<string> keyFileNames;
  //Pointer to current directory
  DIR* keyDirPointer = opendir(keyDir.c_str());
  //Struct to hold information on files accessed by the directory pointer
  struct dirent* keyDirReader;
  //While additional files/directories are present
  while ((keyDirReader = readdir(keyDirPointer)) != NULL)
  {
    //Check if currently viewed directory is current/parent directory
    //If not, add to list of filenames
    if ((curDir.compare(keyDirReader->d_name) != 0) &&
        (parDir.compare(keyDirReader->d_name) != 0))
    {
      keyFileNames.push_back(keyDirReader->d_name);
    }
  }
  //Close directory pointer
  closedir(keyDirPointer);
  //Return list of file names
  return keyFileNames;
}

/*Allows user to save currently used key information*/
bool saveCurrentKey(RSACipher& cipher, string keyDir)
{
  //Characters disallowed in filenames to prevent detrimental file manipulation
  const string disallowedChars = "\\/?*:|\"<>. ";
  //Command to cancel saving
  const string cancelCommand = "c";
  //String to add to end of filename
  const string txtAppend = ".txt";
  //Maximum file name length supported by implementation
  const uint maxFileNameSize = 256;
  //Gets list of all currently stored files and displays them
  vector<string> keyFileNames = readKeyDirectory(keyDir);
  cout << endl << "Current key files: " << endl;
  for (uint n = 0; n < keyFileNames.size(); n++)
  {
    cout << (n + 1) << ". " << keyFileNames.at(n) << endl;
  }
  bool smallLoop = true;
  string newKeyFileName = "";
  cout << endl << "WARNING: Entering filenames identical to a current key file "
          "will overwrite previous data." << endl;
  //Loops until new valid key file name is found
  while (smallLoop)
  {
    cout << "Enter a one-word filename for new key file (without '.txt' "
            "appended, maximum of 252 chars) or input 'c' to cancel: ";
    //Gets user input - already present filenames will be overwritten
    getline(cin, newKeyFileName);
    //Checks for user cancelling save
    if (newKeyFileName.compare(cancelCommand) == 0)
    {
      cout << "Key saving cancelled." << endl;
      return false;
    }
    //Checks that the input file name does not contain any disallowed chars
    size_t charChecker = newKeyFileName.find_first_of(disallowedChars, 0);
    if (charChecker != string::npos)
    {
      cout << "Invalid file name. Try again." << endl;
      newKeyFileName = "";
    }
    //Checks that file name length is within bounds
    else if ((newKeyFileName.length() + txtAppend.length()) > maxFileNameSize)
    {
      cout << "File name too long. Try again." << endl;
      newKeyFileName = "";
    }
    //Valid file name, exit loop
    else
    {
      smallLoop = false;
    }
  }
  //Appends file name with directory info and file type
  newKeyFileName = keyDir + newKeyFileName + txtAppend;
  //Gets current key info
  vector<BigInt> curKeyInfo = cipher.getKeyInfo();
  //Writes encrypted data to file
  ofstream keyFile(newKeyFileName);
  BigInt keyDatum;
  for (uint n = 0; n < curKeyInfo.size(); n++)
  {
    keyDatum = curKeyInfo.at(n);
    keyFile << keyDatum.get_str(hexBase);
    //Writes newline if not at the end of key info
    if (n < curKeyInfo.size() - 1)
    {
      keyFile << endl;
    }
  }
  //Closes key file
  keyFile.close();
  return true;
}

/*Allows user to set key to pre-generated and saved values*/
bool setKey(RSACipher& cipher, string keyDir)
{
  //Command to cancel saving
  const string cancelCommand = "c";
  //String to add to end of filename
  const string txtAppend = ".txt";
  //Count of pieces of data needed for key
  const uint keyDatumCount = 5;
  //Gets and prints list of currently saved files
  vector<string> keyFileNames = readKeyDirectory(keyDir);
  cout << endl << "Current key files: " << endl << endl;
  for (uint n = 0; n < keyFileNames.size(); n++)
  {
    cout << (n + 1) << ". " << keyFileNames.at(n) << endl;
  }

  string userChoice;
  int intChoice;
  bool smallLoop = true;
  //Loops until user inputs valid file choice
  while (smallLoop)
  {
    cout << endl << "Input key file choice number, or input 'c' to cancel: ";
    getline(cin, userChoice);
    //User wants to cancel loading
    if (userChoice.compare(cancelCommand) == 0)
    {
      cout << "Key loading cancelled." << endl;
      return false;
    }
    //Not cancelling loading
    else
    {
      //Converts user input to integer
      intChoice = stoi(userChoice);
      //If input does not correspond to an available choice
      if (intChoice <= 0 || intChoice > (int)keyFileNames.size())
      {
        cout << "Invalid file choice. Try again." << endl;
      }
      //Valid input
      else
      {
        smallLoop = false;
      }
    }
  }

  //Gets full key file directory
  string keyFileName = keyDir + keyFileNames.at(intChoice - 1);
  ifstream keyFile(keyFileName);
  string curKeyLine;
  BigInt curKeyVal;
  vector<BigInt> keyInfo;
  //Gets all key info from file
  for (uint n = 0; n < keyDatumCount; n++)
  {
    getline(keyFile, curKeyLine);
    curKeyVal.set_str(curKeyLine, hexBase);
    keyInfo.push_back(curKeyVal);
  }
  //Closes key file
  keyFile.close();
  //Sets cipher's key info to new values
  cipher.setKeyInfo(keyInfo);
  return true;
}


/*Displays menu for encryption*/
void encryptionMenu(RSACipher& cipher, string ptFileName, string ctFileName)
{
  cout << "Encryption selected." << endl;

  //Open plaintext file and retrieves data
  ifstream ptFile(ptFileName);
  string plainText((istreambuf_iterator<char>(ptFile)),
                   (istreambuf_iterator<char>()));
  ptFile.close();

  string cipherText = "";

  //Performs encryption
  cipher.encrypt(plainText, cipherText);

  //Writes encrypted data to file
  ofstream ctFile(ctFileName);
  ctFile << cipherText;
  ctFile.close();

  cout << endl << "Encrypted output: " << cipherText << endl;
}

/*Displays menu for decryption*/
void decryptionMenu(RSACipher& cipher, string ctFileName, string dtFileName)
{
  cout << "Decryption selected." << endl;

  //Open ciphertext file and retrieves data
  ifstream ctFile(ctFileName);
  string cipherText((istreambuf_iterator<char>(ctFile)),
                   (istreambuf_iterator<char>()));
  ctFile.close();
  string plainText = "";

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

  //Performs decryption
  cipher.decrypt(cipherText, plainText, crtFlag);

  //Writes decrypted data to file
  ofstream dtFile(dtFileName);
  dtFile << plainText;
  dtFile.close();

  cout << endl << "Decrypted output: " << plainText << endl;
}

/*Displays menu for signing*/
void signingMenu(RSACipher& cipher, string ptFileName, string stFileName)
{
  cout << "Signing selected." << endl;

  //Open plaintext file and retrieves data
  ifstream ptFile(ptFileName);
  string plainText((istreambuf_iterator<char>(ptFile)),
                   (istreambuf_iterator<char>()));
  ptFile.close();

  string signedText = "";
  // if(!textInputMenu(cipherText))
  // {
  //   cout << "Text input failed!" << endl;
  // }
  string menuString = "\nPlease select your desired option: "
    "\n1. Standard Signing\n2. CRT Signing\n";

  //User input choice
  string userChoice;

  //Boolean representing whether CRT is to be used or not
  bool crtFlag;

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
    //User wants standard
    if (userChoice == "1")
    {
      crtFlag = true;
      cout << "Standard Signing selected. " << endl;
      smallLoop = false;
    }
    //User wants CRT
    else if (userChoice == "2")
    {
      crtFlag = false;
      cout << "CRT Signing selected. " << endl;
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1 or 2. Try again. " << endl;
    }
  }

  //Signs input data
  cipher.sign(plainText, signedText, crtFlag);

  //Writes signed data to file
  ofstream stFile(stFileName);
  stFile << signedText;
  stFile.close();

  cout << endl << "Signing output: " << signedText << endl;
}

/*Displays menu for authentication*/
void authenticationMenu(RSACipher& cipher, string stFileName, string atFileName)
{
  cout << "Authentication selected." << endl;
  //Open signedtext file and retrieves data
  ifstream stFile(stFileName);
  string cipherText((istreambuf_iterator<char>(stFile)),
                   (istreambuf_iterator<char>()));
  stFile.close();
  string authText = "";

  //Authenticates input data
  cipher.auth(cipherText, authText);
  //Writes authenticated data to file
  ofstream atFile(atFileName);
  atFile << authText;
  atFile.close();

  cout << "Authentication output: " << authText << endl;
}

/*Displays menu for keys*/
void keyOptMenu(RSACipher& cipher)
{
  cout << "Key Options selected." << endl;
  const string keyDirBase = "./keys/";

  //Prompt message
  string menuString = "Select from the following options:\n1. Generate keys"
    "\n2. View keys\n3. Change key size\n4. Save keys\n5. Load keys\n6. "
    "View all saved key files for current key size\n7. Return to main menu\n";

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
    //User wants to change cipher key size
    else if (userChoice == "3")
    {
      keySizeMenu(cipher);
    }
    //User wants to save current keys
    else if (userChoice == "4")
    {
      //Gets complete directory for key saving based on cipher's nLen
      string keyDir = keyDirBase + to_string(cipher.getNLen()) + "/";
      saveCurrentKey(cipher, keyDir);
    }
    //User wants to load saved keys
    else if (userChoice == "5")
    {
      //Gets complete directory for key loading based on cipher's nLen
      string keyDir = keyDirBase + to_string(cipher.getNLen()) + "/";
      setKey(cipher, keyDir);
    }
    //User wants to view currently available saved key files
    else if (userChoice == "6")
    {
      //Gets complete directory for key loading based on cipher's nLen
      string keyDir = keyDirBase + to_string(cipher.getNLen()) + "/";
      //Gets all relevant file names
      vector<string> keyFileNames = readKeyDirectory(keyDir);
      //Prints file names
      cout << endl << "Current key files: " << endl;
      for (uint n = 0; n < keyFileNames.size(); n++)
      {
        cout << (n + 1) << ". " << keyFileNames.at(n) << endl;
      }
    }
    //User wants to return to main menu
    else if (userChoice == "7")
    {
      smallLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must either be 1, 2, 3, 4, 5, 6, or 7. Try again." << endl;
    }
  }
}

/*Displays menu for overall RSA algorithm*/
int main()
{
  cout << "FIPS-COMPLIANT* RSA - GEORGE WOOD" << endl;
  cout << "*With one exception" << endl;
  cout << "Data to be manipulated can be user-altered by modifying the "
          "'pt.txt' file in the 'results' folder. Input must be in "
          "hexadecimal format." << endl;
  cout << "Default key size is 2048 bits. This can be changed in the key "
          "options menu." << endl;
  const string ptFileName = "./results/pt.txt";
  const string ctFileName = "./results/ct.txt";
  const string dtFileName = "./results/dt.txt";
  const string stFileName = "./results/st.txt";
  const string atFileName = "./results/at.txt";

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
      encryptionMenu(cipher, ptFileName, ctFileName);
    }
    //User wants to decrypt
    else if (userChoice == "2")
    {
      decryptionMenu(cipher, ctFileName, dtFileName);
    }
    //User wants to sign
    else if (userChoice == "3")
    {
      signingMenu(cipher, ptFileName, stFileName);
    }
    //User wants to authenticate
    else if (userChoice == "4")
    {
      authenticationMenu(cipher, stFileName, atFileName);
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
      cout << "Input must be either 1, 2, 3, 4, 5, or 6. Try again. " << endl;
    }
  }
}
