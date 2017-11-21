/*
RSACipher.hpp - George Wood
*/

#ifndef __RSACIPHER_H__
#define __RSACIPHER_H__

#include <string>
#include <map>
#include <gmpxx.h>

  typedef mpz_class BigInt;
  typedef mpf_class BigFloat;

class RSACipher
{
  private:
    /*
    As specified in NIST Special Publication 800-57 Part 1, Page 53
    The first two vals are not approved to have probable primes generated,
    only probable primes - as such they are not currently implemented
    N vals are lengths in bits of the n in modulo n
    s values are the corresponding security strengths to be used
    */
    const int nLen0 = 1024;
    const int s0 = 80;
    const int nLen1 = 2048;
    const int s1 = 112;
    const int nLen2 = 3072;
    const int s2 = 128;
    std::map<int, int> validPairs =
    {
      {nLen0, s0},
      {nLen1, s1},
      {nLen2, s2}
    };
    int securityStrength;
    BigInt e = 0;
    BigInt d = 0;
    BigInt n;
    int nLen;
    BigInt p;
    BigInt q;


    BigInt hashAlg(const BigInt inputX);
    std::string genRandBits(int stringSize);
    bool sieveProcedure(BigInt limitVal);
    bool primalityTest(BigInt potentialPrime);
    bool randomPrime(const int length, const BigInt seed, BigInt& outputPrime,
      BigInt& outputSeed);
    bool genFirstSeed(BigInt& seed);
    bool genPrimeFromAuxiliaries(const int l, const int n1, const int n2,
      const BigInt firstSeed, BigInt& outputSeed);
    bool genPrimes(BigInt seed);

  public:
    RSACipher();
    RSACipher(int inputNLen);
    bool genRSAKeys();
    bool encrypt(std::string plainText);
    bool decrypt(std::string cryptText);
};

#endif