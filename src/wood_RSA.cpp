/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include "RSACipher.hpp"
using namespace std;

//As specified in NIST Special Publication 500-87 Part 1, Page 53
//For keystrength 1024: <= 80, for 2048: 112, for 3072: 128

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
  srand(time(NULL));
  cout << "FIPS COMPLIANT RSA - GEORGE WOOD" << endl;

  RSA();

  return 0;
}