/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include "RSACipher.hpp"
using namespace std;



bool RSA()
{
  RSACipher cipher = RSACipher();

  if (!cipher.genRSAKeys())
  {
    return false;
  }

  return true;
}

int main()
{
  //srand(time(NULL));
  cout << "FIPS COMPLIANT RSA - GEORGE WOOD" << endl;

  //Displays title
  cout << "Q-LEARNING AND SARSA" << endl;

  //User input choice
  string userChoice;

  //Main menu string
  string menuString1 = "\nPlease select your desired option "
    "(or input 'q' to quit): \n1. Encryption\n2. Decryption\n"
    "\n3. Key Generation\n";

  //Repeatedly prompts user for desired function until
  //user quits or closes the program.
  bool keepLoop = true;
  while (keepLoop)
  {
    //Displays menu
    cout << menuString1 << endl;

    //Gets user menu choice
    getline(cin, userChoice);

    //User is quitting
    if (userChoice == "q" || userChoice == "Q")
    {
      keepLoop = false;
    }

    //User is not quitting, wants q-learning
    else if (userChoice == "1")
    {
      cout << "Encryption selected." << endl;
      //Number of iterations
      int learningIterations;
      string learningItrStr;

      cout << "Text input or input from file? \n1. Text Input \n"
        "\n2. File Input\n"; 
      //Loops until valid input is given
      bool smallLoop = true;
      while (smallLoop)
      {
        //Gets user specified iteration count
        getline(cin, learningItrStr);
        //If input is not entirely numbers
        if (!(std::all_of(learningItrStr.begin(), learningItrStr.end(),
            ::isdigit)))
        {
          cout << "\nInput must be an integer. Try again: ";
        }
        //Input is entirely numbers
        else
        {
          learningIterations = stoi(learningItrStr);
          smallLoop = false;
        }
      }
      //Begins q-learning with the specified number of iterations
      reinforcmentLearning(learningIterations, Q_LEARN_MODE);
    }
    //User is not quitting, wants SARSA
    else if (userChoice == "2")
    {
      cout << "SARSA selected." << endl;
      //Number of iterations
      int learningIterations;
      string learningItrStr;

      cout << "Input number of learning iterations: ";
      //Loops until valid input is given
      bool smallLoop = true;
      while (smallLoop)
      {
        //Gets user specified iteration count
        getline(cin, learningItrStr);
        //If input is not entirely numbers
        if (!(std::all_of(learningItrStr.begin(), learningItrStr.end(),
            ::isdigit)))
        {
          cout << "\nInput must be an integer. Try again: ";
        }
        //Input is entirely numbers
        else
        {
          learningIterations = stoi(learningItrStr);
          smallLoop = false;
        }
      }
      //Begins SARSA with the specified number of iterations
      reinforcmentLearning(learningIterations, SARSA_MODE);
    }
    //Invalid input
    else
    {
      cout << "Input must be either 1, 2, or 'q'. Try again: ";
    }
  }
}
