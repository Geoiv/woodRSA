#include "./head/RSAConsts.hpp"
#include <fstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <gmpxx.h>
using namespace std;

  typedef mpz_class BigInt;

//Key sizes of 512b, 1024b, 2048b, 4096b
const vector<uint> keySizes = {512, 1024, 2048, 4096};
int main()
{
  string plainTextString = "c";
  string cipherTextString;

  BigInt e = 5;
  BigInt d;
  BigInt p = 63;
  BigInt q = 39;
  BigInt n = p * q;
  BigInt phiN = (p - 1) * (q - 1);
  cout << phiN << endl;

  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phiN.get_mpz_t());
  cout << d << endl;

  BigInt pt;
  BigInt ct;
  pt.set_str(plainTextString, hexBase);
  mpz_powm(ct.get_mpz_t(), pt.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  cipherTextString = ct.get_str(hexBase);
  mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

  cout << cipherTextString << endl;
  cout << plainTextString << endl;

}
