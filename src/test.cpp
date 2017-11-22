#include <gmpxx.h>
#include <iostream>
#include <math.h>
using namespace std;

  typedef mpz_class BigInt;
  typedef mpf_class BigFloat;

int main()
{
  BigInt x = 5;
  string binary = x.get_str(2);
  cout << x << " " << binary << " " << binary.length() << endl;
}