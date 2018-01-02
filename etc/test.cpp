#include <gmpxx.h>
#include <bitset>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
//#include "SHA224.cpp"
using namespace std;

  typedef mpz_class BigInt;
  typedef mpf_class BigFloat;
  typedef unsigned int uint;

int main()
{
  BigInt x = 5;
  BigInt y = 10;
  BigInt z = 12;

  cout << y % x << endl;
  cout << -y % x << endl;
  cout << (-y * x) % z << endl;

}