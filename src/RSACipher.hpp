/*
RSACipher.hpp - George Wood
*/

#ifndef __RSACIPHER_H__
#define __RSACIPHER_H__

#include <string>
#include <vector>
#include <map>
#include <gmpxx.h>

  typedef mpz_class BigInt;
  typedef mpf_class BigFloat;
  typedef unsigned int uint;

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
    const uint nLen0 = 1024;
    const uint s0 = 80;
    const uint nLen1 = 2048;
    const uint s1 = 112;
    const uint nLen2 = 3072;
    const uint s2 = 128;
    std::map<uint, uint> validPairs =
    {
      {nLen0, s0},
      {nLen1, s1},
      {nLen2, s2}
    };
    uint securityStrength;
    BigInt e = 0;
    BigInt d = 0;
    BigInt n;
    uint nLen;
    BigInt p;
    BigInt q;


    BigInt hashAlg(const BigInt inputX);
    std::string genRandBits(uint stringSize);
    std::vector<bool> sieveProcedure(uint limitVal);
    bool primalityTest(BigInt c);
    bool randomPrime(const uint length, const BigInt seed, BigInt& outputPrime,
      BigInt& outputSeed, BigInt& pGenCounter);
    bool genFirstSeed(BigInt& seed);
    bool genPrimeFromAuxiliaries(const uint l, const uint n1, const uint n2,
      const BigInt firstSeed, BigInt& outputPrime, BigInt& outputSeed);
    bool genPrimes(BigInt seed);

  public:
    RSACipher();
    RSACipher(uint inputNLen);
    bool genKeys();
    void displayKeyInfo();
    bool encrypt(std::string plainText);
    bool decrypt(std::string cryptText);
};

#endif