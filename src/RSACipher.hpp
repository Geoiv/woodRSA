/*
RSACipher.hpp - George Wood
*/

#ifndef __RSACIPHER_H__
#define __RSACIPHER_H__

#include <string>
#include <map>
#include "gmp/gmp.h"

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
    const int outLen = 512;
    int securityStrength;
    mpz_t e;
    mpz_t d;
    mpz_t n;
    int nLen;
    mpz_t q;
    mpz_t p;

    void hashAlg(mpz_t outputX, const mpz_t inputX);
    std::string genRandBits(int stringSize);
    bool sieveProcedure(mpz_t limitVal);
    bool primalityTest(mpz_t potentialPrime);
    bool randomPrime(int length, mpz_t seed, mpz_t outputPrime,
      mpz_t outputSeed);
    bool genFirstSeed(mpz_t seed);
    bool genPrimeFromAuxiliaries(int l, int n1, int n2,
      mpz_t firstSeed, const mpz_t e, mpz_t outputPrime, mpz_t outputSeed);
    bool genPrimes(const mpz_t e, const mpz_t seed,
      mpz_t p, mpz_t q);

  public:
    RSACipher();
    RSACipher(int inputNLen);
    bool genRSAKeys();
    bool encrypt(mpz_t e, mpz_t n, std::string plainText);
    bool decrypt(mpz_t d, mpz_t n, std::string cryptText);
};

#endif