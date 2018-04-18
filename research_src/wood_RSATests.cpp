/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include <fstream>
#include <istream>
#include <ctime>
#include "./head/RSAResearchCipher.hpp"
using namespace std;

//Key sizes of 512b, 1024b, 2048b, 4096b
const vector<uint> keySizes = {512, 1024, 2048, 4096};
//Data sizes of 64B, 1kB, 32kB, 64kB, 128kB, 1MB
const vector<uint> fileSizes = {64, 1024, 32768, 65536, 131072, 1048576};

/*Gets text input from files*/
bool getTextFromFile(string& outputString)
{
  // uint inputBase = inputBaseMenu();
  return true;
}



void runTests(string keyFileName, string dataFileName)
{
  const bool runCRT = true;
  const bool noCRT = false;
  const uint bitsInByte = 8;
  //Number of key sizes to test for
  const uint keySizeCount = keySizes.size();
  //Number of parameters to set key on cipher
  const uint keyDataCount = 5;
  //Current size of key being worked with
  uint curKeySize;
  //Gets key info
  fstream keyFile(keyFileName, fstream::in);
  //Holds all key info
  vector<vector<BigInt>> allKeyInfo;
  //Gets all key info from file
  for (uint i = 0; i < keySizes.size(); i++)
  {
    vector<BigInt> tempVec;
    string tempStr;
    BigInt tempBig;
    //Gets e, d, p, q, and n in that order
    for (uint j = 0; j < keyDataCount; j++)
    {
      getline(keyFile, tempStr);
      tempVec.push_back(tempBig.set_str(tempStr, hexBase));
    }
    allKeyInfo.push_back(tempVec);
    getline(keyFile, tempStr);
  }

  keyFile.close();

  fstream dataFile(dataFileName, fstream::in);
  vector<string> allTestData;
  for (uint i = 0; i < fileSizes.size(); i++)
  {
    string tempStr;
    getline(dataFile, tempStr);
    allTestData.push_back(tempStr);
  }
  dataFile.close();
  clock_t begin;
  clock_t end;
  for (uint curKeyNum = 0; curKeyNum < keySizeCount; curKeyNum++)
  {
    curKeySize = keySizes.at(curKeyNum);
    cout << "Running tests for key size " << curKeySize << " bits." <<endl;
    RSACipher cipher(curKeySize);
    cipher.setKeyInfo(allKeyInfo.at(curKeyNum));
    cout << "  Running tests for encryption." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.encrypt(dataVec.at(j), tempStr);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
    cout << endl;

    cout << "  Running tests for decryption." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.decrypt(dataVec.at(j), tempStr, noCRT);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
    cout << endl;

    cout << "  Running tests for decryption with CRT." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.decrypt(dataVec.at(j), tempStr, runCRT);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
    cout << endl;

    cout << "  Running tests for signing." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.sign(dataVec.at(j), tempStr, noCRT);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
    cout << endl;

    cout << "  Running tests for signing with CRT." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.sign(dataVec.at(j), tempStr, runCRT);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
    cout << endl;

    cout << "  Running tests for authentication." << endl;
    for (uint i = 0; i < fileSizes.size(); i++)
    {
      int curFileSize = fileSizes.at(i);
      string dataStr = allTestData.at(i);
      vector<string> dataVec;
      for (uint j = 0; j < (curFileSize * bitsInByte); j+= curKeySize)
      {
        dataVec.push_back(dataStr.substr(j, curKeySize - 1));
      }
      cout << "    Test for file size of " << curFileSize << " bytes." << endl;
      cout << "      ";
      system("pause");
      begin = clock();
      for (uint j = 0; j < dataVec.size(); j++)
      {
        string tempStr;
        cipher.auth(dataVec.at(j), tempStr);
      }
      end = clock();
      cout << "      Time elapsed: " << ((double)(end - begin)) / CLOCKS_PER_SEC << "s.";
    }
  }
  cout << "Tests complete!" << endl << endl;
}

/*Displays menu for overall RSA algorithm*/
int main()
{
  cout << "RSA Efficiency Testing - George Wood" << endl;

  //User input choice
  string userChoice;

  //Main menu string
  string menuString = "\nPlease select your desired option: "
    "\n1. Generate new keys\n2. Run test\n3. Quit";

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

    //User wants to generate new keys
    if (userChoice == "1")
    {
      genNewKeys(keyFileName);
    }
    //User wants to run tests
    else if (userChoice == "2")
    {
      runTests(keyFileName, dataFileName);
    }
    //User wants to quit
    else if (userChoice == "3")
    {
      keepLoop = false;
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1, 2, or 3. Try again. " << endl;
    }
  }
}
