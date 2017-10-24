/*
RSACipher.cpp - George Wood
*/

#include <random>
#include <math.h>
#include <climits>
#include "bigint/BigUnsignedInABase.hh"
#include "RSACipher.hpp"
using namespace std;

/*Default constructor for the RSA cipher*/
RSACipher::RSACipher()
{

}



bool RSACipher::isValidPair(int L, int N)
{
  if (((L == L0) && (N == N0)) || ((L == L1) && (N == N1)) ||
    ((L == L2) && (N == N2)) || ((L == L3) && (N == N3)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//TODO hash alg
BigUnsigned RSACipher::hashAlg(BigUnsigned inputX)
{
  BigUnsigned test;
  return test;
}

//TODO approved RBG
string RSACipher::genRandBits(int stringSize)
{
  string outputString = "";
  for (int i = 0; i < stringSize; i++)
  {
    outputString += to_string(rand() % 2);
  }
  return outputString;
}

//Appendix C.6 TODO
bool RSACipher::randomPrime(int length, BigUnsigned seed,
  BigUnsigned& outputPrime, BigUnsigned& outputSeed, BigUnsigned& outputCounter)
{
  BigUnsigned randPrime;
  bool status;
  return status;
}

bool RSACipher::genRSASeed(BigUnsigned& seed, int seedLen)
{
  //1. If nlen is not valid, then Return (FAILURE, 0). (separate function)
  //2. Set security_strength to val with nlen in SP 800-57, Part 1.
  //3. Obtain (2 * security_strength) bit string from a valid RBG
  seed = BigUnsignedInABase(genRandBits(SECURITY_STRENGTH * 2), 2);

  //4. Return (SUCCESS, seed).
  return true;
}

//TODO validate primes?
/*
Generates the p & q, the seeds needed for RSA key generation.
seedLen must be larger than N.
*/
bool RSACipher::genRSAPrimes(int nLen, BigUnsigned e, BigUnsigned seed,
  BigUnsigned& p, BigUnsigned& q)
{
  //1. If nlen isn't 2048 nor 3072, then return (FAILURE, 0, 0) (separate func)

  //2. If ((e ≤ 2^16) OR (e ≥ 2^256) OR (e is not odd)), then return (FAILURE, 0, 0).
  if  (e <= modexp(2, 16, 1)  || (e >= modexp(2, 256, 1)) ||
    (modexp(e, 1, 2) != 1))
  {
    return false;
  }
  //3. Set security_strength to val with nlen in SP 800-57, Part 1.

  //4. If (len(seed) ≠ 2 * security_strength), then return (FAILURE, 0, 0).
  //dealt with in generation

  //5. working_seed = seed.
  BigUnsigned workingSeed = seed;

  int L = nLen / 2;
  int n1 = 1;
  int n2 = 1;
  BigUnsigned pSeed;
  BigUnsigned qSeed;
  BigUnsigned firstSeed = workingSeed;

  //6. Generate p:
  //6.1 Using L = nlen/2, N1 = 1, N2 = 1, first_seed = working_seed and e,
  //use the provable prime construction method in Appendix C.10 to obtain p and
  //pseed. If FAILURE is returned, then return (FAILURE, 0, 0).

  //TODO appendix c.10 alg

  //6.2 working_seed = pseed.
  workingSeed = pSeed;

  BigUnsigned absVal = 0;
  BigUnsigned comparisonVal = modexp(2, ((nLen / 2) - 100), 1);
  while(absVal <= comparisonVal)
  {
    //7. Generate q:
    //7.1 Using L = nlen/2, N1 = 1, N2 = 1, first_seed = working_seed and e,
    //use the provable prime construction method in Appendix C.10 to obtain q
    // and qseed. If FAILURE is returned, then return (FAILURE, 0, 0).
    //TODO appendix c.10 alg

    //7.2 working_seed = qseed.
    workingSeed = qSeed;
    //BigInteger has no abs val, so have to subtract this way

    if (p > q)
    {
      absVal = p - q;
    }
    else
    {
      absVal = q - p;
    }
    //8. If ( |p – q| ≤ 2^(nlen/2 – 100)), then go to step 7.
  }

  //9. Zeroize the internally generated seeds:
    //9.1 pseed = 0;
    //9.2 qseed = 0;
    //9.3 working_seed = 0.
  pSeed = 0;
  qSeed = 0;
  workingSeed = 0;

  //10. Return (SUCCESS, p, q).
  return true;
}

bool RSACipher::genRSAKeys()
{
  //Initializes seed for random number gen
  random_device randSeeder;
  //Creates random generator engine using Mersenne-Twister engine
  mt19937 numGen(randSeeder());
  //Random number generator, 'guaranteed unbiased'
  uniform_int_distribution<int> unbiasedGen(pow(2, 16), UINT_MAX);

  //Gets randomized e value
  e = unbiasedGen(numGen);

  //Primes used for RSA
  BigUnsigned p;
  BigUnsigned q;

  //TODO initialize nLen better
  int nLen = 3072;

  //Seed used for generating primes
  BigUnsigned seed;

  //Generates seed for prime generation
  if(!genRSASeed(seed, nLen))
  {
    return false;
  }
  //Generates primes p and q for key generation
  if(!genRSAPrimes(nLen, e, seed, p, q))
  {
    return false;
  }

  //Gets n value
  BigUnsigned n = p * q;
  //Gets phi value for n
  BigUnsigned phiN = (p - 1) * (q - 1);

  //Gets private key (d)
  d = modinv(e, phiN);

  //Returns success
  return true;
}

//TODO input text separated into blocks
bool RSACipher::encrypt(BigUnsigned e, BigUnsigned n, string plainText)
{
  BigUnsigned binaryPlainText;
  BigUnsigned binarycryptText = modexp(binaryPlainText, e, n);
  return true;
}

bool RSACipher::decrypt(BigUnsigned d, BigUnsigned n, string cryptText)
{
  BigUnsigned binaryCryptText;
  BigUnsigned binaryPlainText = modexp(binaryCryptText, d, n);
  return true;
}

