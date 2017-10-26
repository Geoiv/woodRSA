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

  RSA();

  return 0;
}
