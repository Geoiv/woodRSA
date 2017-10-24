/*
RSACipher.hpp - George Wood
*/

#ifndef __RSACIPHER_H__
#define __RSACIPHER_H__

#include <string>
#include "bigint/BigUnsigned.hh"
#include "bigint/BigIntegerAlgorithms.hh"

const int SECURITY_STRENGTH = 128;
//TODO check vals
const int L0 = 1024;
const int N0 = 160;
const int L1 = 2048;
const int N1 = 224;
const int L2 = 2048;
const int N2 = 256;
const int L3 = 3072;
const int N3 = 256;

class RSACipher
{
  private:
    BigUnsigned e;
    BigUnsigned d;
    BigUnsigned n;
    BigUnsigned p;

    bool isValidPair(int L, int N);
    BigUnsigned hashAlg(BigUnsigned inputX);
    std::string genRandBits(int stringSize);
    bool randomPrime(int length, BigUnsigned seed, BigUnsigned& outputPrime,
      BigUnsigned& outputSeed, BigUnsigned& outputCounter);
    bool genRSASeed(BigUnsigned& seed, int seedLen);
    bool genRSAPrimes(int nLen, BigUnsigned e, BigUnsigned seed,
      BigUnsigned& p, BigUnsigned& q);

  public:
    RSACipher();
    bool genRSAKeys();
    bool encrypt(BigUnsigned e, BigUnsigned n, std::string plainText);
    bool decrypt(BigUnsigned d, BigUnsigned n, std::string cryptText);
};

#endif