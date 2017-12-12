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

  /*
  The security strength of the hashing algorithm should be the same as the
  security strength of the cipher's chosen nLen.
  Even though SHA224 has a greater security strength than RSA-1024,
  a hash algorithm with a lower security strength than the nLen should not be
  used. As a result it is used instead of SHA-1, which is the next lowest
  security strength (of < 67), in addition to the fact that SHA-1 has been
  depricated.
  */
  const uint nLen0 = 1024;
  const uint s0 = 80;
  const uint sha0 = 224;
  const uint nLen1 = 2048;
  const uint s1 = 112;
  const uint sha1 = 224;
  const uint nLen2 = 3072;
  const uint s2 = 128;
  const uint sha2 = 256;

  const uint binBase = 2;
  const uint hexBase = 16;

  const std::string sha224LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                  "649b934ca495991b";
  const std::string sha256LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                  "649b934ca495991b7852b855";

  std::map<uint, uint> secStrengthPairs =
  {
    {nLen0, s0},
    {nLen1, s1},
    {nLen2, s2}
  };
  std::map<uint, uint> shaBlockPairs =
  {
    {nLen0, sha0},
    {nLen1, sha1},
    {nLen2, sha2}
  };

  BigInt e = 0;
  BigInt d = 0;
  BigInt p;
  BigInt q;
  BigInt n = 0;
  uint nLen;
  uint securityStrength;
  uint shaOutLen;

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
  bool maskGenFunc(BigInt seed, uint maskLen, BigInt& mask);
  bool encrypt(std::string plainTextString, std::string& cipherTextString);
  bool decrypt(std::string cipherTextString, std::string& plainTextString);
  bool sign(std::string plainTextString, std::string& cipherTextString);
  bool auth(std::string cipherTextString, std::string& plainTextString);
};

#endif
