/*
RSACipher.hpp - George Wood
*/

#ifndef __RSACIPHER_HPP__
#define __RSACIPHER_HPP__

#include <string>
#include <vector>
#include <map>
#include "RSAConsts.hpp"
#include "RSABigTypes.hpp"

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
  const uint nLen0 = 512;
  const uint s0 = 80;
  const uint nLen1 = 1024;
  const uint s1 = 80;
  const uint nLen2 = 2048;
  const uint s2 = 112;
  const uint nLen3 = 3072;
  const uint s3 = 128;
  const uint nLen4 = 4096;
  const uint s4 = 128;

  //Pairs of key sizes to security strengths
  std::map<uint, uint> secStrengthPairs =
  {
    {nLen0, s0},
    {nLen1, s1},
    {nLen2, s2},
    {nLen3, s3},
    {nLen4, s4}
  };
  //Pairs of key sizes to SHA output block sizes
  std::map<uint, uint> shaBlockPairs =
  {
    {nLen0, sha224OutLen},
    {nLen1, sha224OutLen},
    {nLen2, sha224OutLen},
    {nLen3, sha256OutLen},
    {nLen4, sha256OutLen}
  };

  //Vals for using in hash algorithms during OAEP mechanisms
  const std::string sha224LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                  "649b934ca495991b";
  const std::string sha256LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                  "649b934ca495991b7852b855";

  //RSA key values
  //Public key
  BigInt e = 0;
  //Private key
  BigInt d = 0;
  //Large primes used for calculating d
  BigInt p;
  BigInt q;
  //Value used in calculating d
  BigInt n = 0;
  //Length in bits of n
  uint nLen;
  //Security strength of this RSA system instance
  uint securityStrength;
  //Length of the output blocks of the SHA algorithm this instance uses
  uint shaOutLen;

  //Hashes the input integer
  BigInt hashAlg(const BigInt inputX);
  //Utilizes a sieve procedure to find primes below an input value
  std::vector<bool> sieveProcedure(uint limitVal);
  //Tests if an input value is prime
  bool primalityTest(BigInt c);
  //Generates a random prime number
  bool randomPrime(const uint length, const BigInt seed, BigInt& outputPrime,
    BigInt& outputSeed, BigInt& pGenCounter);
  //Generates a prime given auxiliary primes
  bool genPrimeFromAuxiliaries(const uint l, const uint n1, const uint n2,
    const BigInt firstSeed, BigInt& outputPrime, BigInt& outputSeed);
  //Generates p and q to later generate d with.
  bool genPrimes(BigInt seed);

public:
  //Default constructor
  RSACipher();
  //Constructor for an input nLen
  RSACipher(uint inputNLen);
  //Generates public and private RSA keys
  bool genKeys(BigInt inputE);
  //Sets keys to values set in a vector
  bool setKeyInfo(std::vector<BigInt> keyInfo);
  //Returns key info in a vector
  std::vector<BigInt> getKeyInfo();
  //Displays key information to the user
  void displayKeyInfo();
  //Generates masks for OAEP mechanisms
  bool maskGenFunc(BigInt seed, uint maskLen, BigInt& mask);
  //Standard RSA encryption
  bool encrypt(std::string plainTextString, std::string& cipherTextString);
  //RSA encryption using OAEP mechanisms
  bool encryptOAEP(std::string plainTextString, std::string& cipherTextString);
  //Standard RSA decryption w/ CRT option
  bool decrypt(std::string cipherTextString, std::string& plainTextString,
               bool crtFlag);
  //RSA decryption using OAEP mechanisms w/ CRT option
  bool decryptOAEP(std::string cipherTextString, std::string& plainTextString,
                   bool crtFlag);
  //Standard RSA signing w/ CRT option
  bool sign(std::string plainTextString, std::string& cipherTextString,
            bool crtFlag);
  //RSA signing using RSASSA-PSS mechanisms w/ CRT option
  bool signPSS(std::string plainTextString, std::string& cipherTextString,
            bool crtFlag);
  //Standard RSA authentication
  bool auth(std::string cipherTextString, std::string& plainTextString);
  //RSA authentication using RSASSA-PSS mechanisms
  bool authPSS(std::string cipherTextString, std::string& plainTextString);
};

#endif
