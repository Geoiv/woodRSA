/*
FIPS Compliant RSA Implementation - George Wood - Capstone Project
*/

#include <iostream>
#include <vector>
#include <bitset>
#include <math.h>
#include <ctime>
#include <random>
#include <climits>
#include "bigint/BigIntegerLibrary.hh"
using namespace std;


/*
L = 1024, N = 160
L = 2048, N = 224
L = 2048, N = 256
L = 3072, N = 256
*/

//As specified in NIST Special Publication 500-87 Part 1, Page 53
//For keystrength 1024: <= 80, for 2048: 112, for 3072: 128
const int SECURITY_STRENGTH = 128;

bool isValidPair(int L, int N)
{
  bool isValid = true;
  return isValid;
}

//TODO
BigUnsigned hashAlg(BigUnsigned inputX)
{

}

string genRandBits(int stringSize)
{
  string outputString = "";
  for (int i = 0; i < stringSize; i++)
  {
    outputString += to_string(rand() % 2);
  }
  return outputString;
}

//Appendix C.6 TODO
bool randomPrime(int length, BigUnsigned seed, BigUnsigned& outputPrime, BigUnsigned& outputSeed, BigUnsigned& outputCounter)
{
  BigUnsigned randPrime;
  bool status;
  return status;
}

//TODO approved RBG

/*
Generates the p & q, the seeds needed for RSA key generation.
seedLen must be larger than N.
*/
bool genRSASeed(BigUnsigned& seed, int seedLen)
{


  //1. If nlen is not valid (see Section 5.1), then Return (FAILURE, 0). (Done in main)
  //2. Let security_strength be the security strength associated with nlen, as specified in SP 800-57, Part 1. (Global var)
  //3. Obtain a string seed of (2 * security_strength) bits from an RBG that supports the security_strength.
  seed = BigUnsignedInABase(genRandBits(SECURITY_STRENGTH * 2), 2);

  //4. Return (SUCCESS, seed).
  return true;
}


bool genRSAPrimes(int nLen, BigUnsigned e, BigUnsigned seed, BigUnsigned& p, BigUnsigned& q)
{
  //1. If nlen is neither 2048 nor 3072, then return (FAILURE, 0, 0). TODO will deal in main
  //2. If ((e ≤ 2^16) OR (e ≥ 2^256) OR (e is not odd)), then return (FAILURE, 0, 0).
  if  (e <= modexp(2, 16, 1)  || (e >= modexp(2, 256, 1)) || (modexp(e, 1, 2) != 1))
  {
    return false;
  }
  //3. Set the value of security_strength in accordance with the value of nlen, as specified in SP 800-57, Part 1. TODO will deal in main

  //4. If (len(seed) ≠ 2 * security_strength), then return (FAILURE, 0, 0). dealt with in generation

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
    //use the provable prime construction method in Appendix C.10 to obtain q and
    //qseed. If FAILURE is returned, then return (FAILURE, 0, 0).
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

bool genRSAKeys(BigUnsigned& e, BigUnsigned& d)
{
  /*
  std::random_device rd;     // only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(min,max); // guaranteed unbiased
  */
  random_device randSeeder;
  mt19937 numGen(randSeeder());
  uniform_int_distribution<int> unbiasedGen(pow(2, 16), UINT_MAX);

  e = unbiasedGen(numGen);

  BigUnsigned p;
  BigUnsigned q;
  //TODO initialize nLen better
  int nLen = 3072;

  BigUnsigned seed;

  if(!genRSASeed(seed, nLen))
  {
    return false;
  }
  if(!genRSAPrimes(nLen, e, seed, p, q))
  {
    return false;
  }

  BigUnsigned n = p * q;
  BigUnsigned phiN = (p - 1) * (q - 1);

  d = modinv(e, phiN);

  return true;
}

//TODO validate primes?

bool encryptRSA(BigUnsigned e, BigUnsigned n, string plainText)
{
  BigUnsigned binaryPlainText;
  BigUnsigned binarycryptText = modexp(binaryPlainText, e, n);
}

bool decryptRSA(BigUnsigned d, BigUnsigned n, string cryptText)
{
  BigUnsigned binaryCryptText;
  BigUnsigned binaryPlainText = modexp(binaryCryptText, d, n);
}

bool RSA()
{
  BigUnsigned e;
  BigUnsigned d;
  bool keyGenSuccess = genRSAKeys(e, d);

  if (!keyGenSuccess)
  {
    return false;
  }


  return true;
}

int main()
{
  srand(time(NULL));
  cout << "FIPS COMPLIANT RSA - GEORGE WOOD" << endl;

  RSA();

  return 0;
}