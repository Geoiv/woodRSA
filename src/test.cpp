//#include <gmpxx.h>
#include <bitset>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include "SHA224.cpp"
using namespace std;

  //typedef mpz_class BigInt;
  //typedef mpf_class BigFloat;

int main()
{
  int x = (int) 'h';
  SHA224 sha;
  stringstream stream;
  stream << hex << x;

  string hexString = stream.str();
  cout << x << endl;
  cout << hexString << endl;
  string result = sha.hash(hexString);
  cout << result << endl;
}