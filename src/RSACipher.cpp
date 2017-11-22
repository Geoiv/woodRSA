/*
RSACipher.cpp - George Wood - RSA 2048 and SHA 256
*/

#include <random>
#include <math.h>
#include <climits>
#include <iostream>
#include "RSACipher.hpp"
#include "SHA256.hpp"
using namespace std;

/*Default constructor for the RSA cipher*/
RSACipher::RSACipher()
{
  nLen = nLen1;
  securityStrength = validPairs.at(nLen1);
}

/*Constructor for specified nLen*/
RSACipher::RSACipher(int inputNLen)
{
  if (inputNLen == nLen0 || inputNLen == nLen1 || inputNLen == nLen2)
  {
    nLen = inputNLen;
    securityStrength = validPairs.at(inputNLen);
  }
  else
  {
    cout << "Invalid nLen input. Please try a different option." << endl;
  }
}

BigInt RSACipher::hashAlg(const BigInt inputX)
{

  SHA256 sha;
  string inputBinary = inputX.get_str(2);
  vector<vector<int>> inputBlocks;
  //Length of string returned  by above function is strlen + 1
  string hashedBinary = sha.hash(inputBinary);
  BigInt hashedInt;
  hashedInt.set_str(hashedBinary, 2);
  return hashedInt;
}

//TODO sieve procedure?
// vector<BigInt> RSACipher::sieveProcedure(BigInt limitVal)
// {
//   bool stillLooping = true;
//   vector<BigInt> primes;
//   while (stillLooping)
//   {
//     BigInt itr;
//
//     BigInt p;
//
//     //for (itr = p; cmp(itr, limitVal) < 0; itr += p)
//     {
//
//     }
//   }
//
//   return primes;
// }

bool RSACipher::primalityTest(BigInt c)
{
  //1. Prepare a table of primes < sqrt(c) using the sieve procedure in (C.8)
  //vector<BigInt> smallerPrimes = sieveProcedure(sqrt(c));

  //2. Divide potential prime by all values in table.
  //If divisible, return failure
  BigInt testPrime = 2;
  BigInt rootC = sqrt(c);
  while (cmp(testPrime, rootC) < 0)
  {
    if (cmp(gcd(testPrime, c), 1) != 0)
    {
      return false;
    }
    mpz_nextprime(testPrime.get_mpz_t(), testPrime.get_mpz_t());
  }
  //3. If not divisible, return success
  return true;
}

string RSACipher::genRandBits(int stringSize)
{
  string outputString = "";
  string currentString = "";
  string oldString = "";
  for (int i = 0; i < stringSize; i++)
  {
    while (currentString.compare(oldString) == 0)
    {
      currentString = to_string(rand() % 2);
    }

    outputString += currentString;
    oldString = currentString;
  }
  return outputString;
}



//Appendix C.6 0 Shawe-Taylor Random Prime Routine
bool RSACipher::randomPrime(const int length, const BigInt inputSeed,
  BigInt& outputPrime, BigInt& primeSeed)
{
  BigInt c;
  BigInt primeGenCounter;

  //Commonly used in later equations
  BigInt exp2_lMinus1;
  mpz_ui_pow_ui(exp2_lMinus1.get_mpz_t(), 2, length - 1);
  BigInt exp2_l;
  mpz_ui_pow_ui(exp2_l.get_mpz_t(), 2, length);



  //1. If (length < 2), then return failure
  if (length < 2)
  {
    return false;
  }
  //2. If (length ≥ 33), then go to step 14.
  if (length < 33)
  {
    //3. primeSeed = inputSeed
    primeSeed = inputSeed;
    //4. primeGenCounter = 0;
    primeGenCounter = 0;

    bool loopStep5 = true;
    while (loopStep5)
    {
      //5. c = hashAlg(primeSeed)^hashAlg(primeSeed + 1)
      c = hashAlg(primeSeed)^(primeSeed + 1);
      //6. c = 2^length-1 + (c mod 2^length-1)
      c = exp2_lMinus1 + (c % exp2_lMinus1);

      //7. c = (2 * floor(c / 2)) + 1
      BigFloat cFloat(c);
      c = ((2 * floor(cFloat / 2)) + 1);
      //8.primeGenCounter++
      primeGenCounter++;
      //9.primeSeed += 2
      primeSeed += 2;
      //11. if c is prime,
      if (primalityTest(c))
      {
        //11.1&2 outputPrime = c, return success
        outputPrime = c;
        return true;
      }

      //12. if primeGenCounter > 4*length, return failure
      if (cmp(primeGenCounter, (4 * length)) > 0)
      {
        return false;
      }
      //13. Go to step 5
    }
  }
  //14. randomPrime(floor(length / 2) + 1, inputSeed)
  BigInt c0;
  //TODO THIS CAN AFFECT primeGenCounter, it must be passed by reference!
  //But other than this recursive call, primeGenCounter is not needed.
  //Pass garbage value?
  if (!randomPrime(floor(length / 2) + 1, inputSeed, c0, primeSeed))
  {
    //15. If failure is returned, return failure
    return false;
  }
  //16. iterations = ceil(length / outlen) - 1;
  int outLen = SHA256::blockSize;
  int iterations = ceil((float)length / outLen) - 1;
  //17. oldCounter = primeGenCounter
  BigInt oldCounter = primeGenCounter;
  //18. x = 0
  BigInt x = 0;

  //19. For i = 0 to iterations, do x += hashAlg(primeSeed + i) * 2^i*outlen
  for (int i = 0; i < iterations; i++)
  {
    BigInt exp2_ixOutLen;
    mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
    x += hashAlg(primeSeed + i) * exp2_ixOutLen;
  }
  //20. primeSeed += iterations + 1
  primeSeed += iterations + 1;
  //21. x = 2^length-1 + x mod 2^(length-1)
  x = exp2_lMinus1 + (x % exp2_lMinus1);

  //22.t = ceil(x / (2 * c0))
  BigInt twoC0 = 2 * c0;
  BigInt t;
  mpz_cdiv_q(t.get_mpz_t(), x.get_mpz_t(), twoC0.get_mpz_t());

  while (true)
  {
    //23. if (2 * t * c0) + 1 > 2^length) then t = ceil(2^length-1 / (2 * c0))
    if (cmp(((2 * t * c0) + 1), exp2_l) > 0)
    {
      mpz_cdiv_q(t.get_mpz_t(), exp2_lMinus1.get_mpz_t(), twoC0.get_mpz_t());
    }
    //24. c = 2tc0 + 1
    c = (2 * t * c0) + 1;
    //25. primeGenCounter++
    primeGenCounter++;
    //26. a = 0
    BigInt a = 0;

    //27. For i = 0 to iterations, do:
    for (int i = 0; i < iterations; i++)
    {
      BigInt exp2_ixOutLen;
      mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
      a += hashAlg(primeSeed + i) * exp2_ixOutLen;
    }
    //28. primeSeed += iterations + 1
    primeSeed += iterations + 1;
    //29. a = 2 + a mod (c - 3)
    a = 2 + (a % (c - 3));

    //30. z = a^2t mod c
    BigInt z;
    BigInt twoT = (2 * t);
    mpz_powm(z.get_mpz_t(), a.get_mpz_t(), twoT.get_mpz_t(), c.get_mpz_t());

    //31. if (gcd(z-1, c) == 1) and (z^c0 mod c == 1) then
    BigInt expZ_c0ModC;
    mpz_powm(expZ_c0ModC.get_mpz_t(), z.get_mpz_t(), c0.get_mpz_t(),
      c.get_mpz_t());
    if ((cmp(gcd(z - 1, c), 1) == 0) && (cmp(expZ_c0ModC, 1) == 0))
    {
      //31.1 prime = c
      outputPrime = c;
      //31.2 return success
      return true;
    }
    //32. if primeGenCounter >= (4 * length + oldCounter) return failure

    if (cmp(primeGenCounter, ((4 * length) + oldCounter)) >= 0)
    {
      return false;
    }
    //33. t++
    t++;
    //34. Go to step 23
  }
}

bool RSACipher::genFirstSeed(BigInt& seed)
{
  //1. If invalid nLen, return failure (done in constructor)
  //2. Set security_strength to corresponding val for nLen (done in constructor)
  //3. Obtain (2 * security_strength) bit string from a valid RBG
  seed.set_str(genRandBits(securityStrength * 2), 2);
  //4. Return success
  return true;
}

//Appendix C.10
bool RSACipher::genPrimeFromAuxiliaries(const int l, const int n1, const int n2,
  const BigInt firstSeed, BigInt& outputSeed)
{
  BigInt p0;
  BigInt p1;
  BigInt p2;
  BigInt pSeed;
  BigInt p0Seed;
  BigInt p2Seed;

  //Commonly used in equations
  BigInt exp2_lMinus1;
  mpz_ui_pow_ui(exp2_lMinus1.get_mpz_t(), 2, l - 1);
  BigInt exp2_l;
  mpz_ui_pow_ui(exp2_l.get_mpz_t(), 2, l);

  //1. If l, n1, or n2 are not valid, return failure (already handled)
  //2. If n1 == 1
  if (n1 == 1)
  {
    //2.1 p1 = 1
    p1 = 1;
    //2.2 p2Seed = firstSeed
    p2Seed = firstSeed;
  }
  else if (n1 >= 2)
  {
    //3.1  Using n1 as the length and firstSeed as the input_seed, use
    //the random prime  generation routine in Appendix C.6 to obtain
    //p1 and p2Seed.
    if (!randomPrime(n1, firstSeed, p1, p2Seed))
    {
      //3.2 If failure is returned, return failure
      return false;
    }
  }
  //4. If n2 == 1
  if (n2 == 1)
  {
    //4.1 p2 = 1
    p2 = 1;
    //4.2 p0Seed = p2Seed
    p0Seed = p2Seed;
  }
  else if (n2 >= 2)
  {
    //5.1  Using n2 as the length and p2Seed as the inputSeed, use
    //the random prime generation routine in Appendix C.6 to obtain
    // p2 and p0Seed.
    if (!randomPrime(n2, p2Seed, p2, p0Seed))
    {
      //5.2 If fairlure is returned, return failure
      return false;
    }
  }
  //6. Using ceil(l / 2) + 1 as the length and p0Seed as the inputSeed,
  //use the random prime generation routine in Appendix C.6 to obtain
  //p0 and pSeed. If failure is returned, then return failure
  if (!randomPrime(ceil((float)l/2) + 1, p0Seed, p0, pSeed))
  {
    return false;
  }
  //7. iterations = ceil(l / outLen) − 1, where outLen is the length of the
  //hash function output block
  int outLen = SHA256::blockSize;
  int iterations = ceil((float)l / outLen) - 1;
  //8. pGenCounter = 0
  BigInt pGenCounter = 0;
  //9. x = 0
  BigInt x = 0;
  //10. For i = 0 to iterations: x = x + (hashAlg(pSeed + i)) ∗ 2^(i * outLen)
  for (int i = 0; i < iterations; i++)
  {
    BigInt exp2_ixOutLen;
    mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
    x += (hashAlg(pSeed + i) * exp2_ixOutLen);
  }
  //11. pSeed = pSeed + iterations + 1.
  pSeed += iterations + 1;

  //12. x = floor(sqrt(2)*(2^(l−1))) + (x mod (2^(l) − floor(sqrt(2)(2^(l−1)))))
  //floor(sqrt(2)*(2^(l−1)))
  BigFloat exp2_lMinus1Float(exp2_lMinus1);
  BigFloat floorRoot2xExp2_lMinus1Float = sqrt(2) * exp2_lMinus1Float;
  BigInt floorRoot2xExp2_lMinus1(floor(floorRoot2xExp2_lMinus1Float));
  BigInt leftHalf = floorRoot2xExp2_lMinus1;

  //(x mod (2^(l) − floor(sqrt(2)(2^(l−1)))))
  BigInt rightHalf = (x % (exp2_l - floorRoot2xExp2_lMinus1));
  //Final assignment
  x = leftHalf + rightHalf;

  //13. If (gcd(p0*p1, p2) ≠ 1), then return failure
  if (cmp(gcd(p0 * p1, p2), 1) != 0)
  {
    return false;
  }

  //TODO 14. Compute y in the interval [1, p2] such that 0 = (y*p0*p1–1) mod p2.
  BigInt y;

  //15. t = ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2)).
  BigInt twoxYxP0xP1 = (2 * y * p0 * p1);
  BigInt twoxP0xP1xP2 = (2 * p0 * p1 * p2);
  leftHalf = twoxYxP0xP1 + x;
  rightHalf = twoxP0xP1xP2;
  BigInt t;
  mpz_cdiv_q(t.get_mpz_t(), leftHalf.get_mpz_t(), rightHalf.get_mpz_t());

  while (true)
  {
    //16. If ((2(t*p2 − y)*p0*p1 + 1) > 2^l)
    BigInt possibleP = ((2 * ((t * p2) - y) * p0 * p1) + 1);
    if (cmp(possibleP, exp2_l) > 0)
    {
      leftHalf = twoxYxP0xP1;
      mpz_cdiv_q(rightHalf.get_mpz_t(), floorRoot2xExp2_lMinus1.get_mpz_t(),
      twoxP0xP1xP2.get_mpz_t());
      //then t = ceil((2*y*p0*p1) + floor((sqrt(2))(2^(L−1))) / (2*p0*p1*p2))
      t = leftHalf + rightHalf;
    }
    //17. p = 2(t*p2 − y)*p0*p1 + 1.
    p = possibleP;
    //18. pGenCounter = pGenCounter + 1.
    pGenCounter++;
    //19. If (GCD(p–1, e) = 1), then
    if (cmp(gcd(p - 1, e), 1) == 0)
    {
      //19.1 a = 0
      BigInt a = 0;

      //19.2 For i = 0 to iterations do: a += (Hash(pseed + i))∗ 2 i * outlen.
      for (int i = 0; i < iterations; i++)
      {
        BigInt exp2_ixOutLen;
        mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
        a += hashAlg(pSeed + i) * exp2_ixOutLen;
      }
      //19.3 pSeed = pSeed + iterations + 1.
      pSeed += iterations + 1;
      //19.4 a = 2 + (a mod (p–3)).
      a = 2 + (a % (p - 3));

      //19.5 z = a^(2(t*p2 − y)*p1) mod p.
      BigInt z;
      BigInt expVal = (2 * ((t * p2) - y) * p1);
      mpz_powm(z.get_mpz_t(), a.get_mpz_t(), expVal.get_mpz_t(), p.get_mpz_t());


      //19.6 If ((1 = GCD(z–1, p)) and (1 = (z^(p0) mod p)), then return success
      BigInt expZ_p0ModP;
      mpz_powm(expZ_p0ModP.get_mpz_t(), z.get_mpz_t(), p0.get_mpz_t(),
        p.get_mpz_t());
      if ((cmp(gcd(z - 1, p), 1) == 0) && (cmp(expZ_p0ModP, 1) == 0))
      {
        return true;
      }
      //20. If (pgen_counter ≥ 5L), then return failure
      if (cmp(pGenCounter, l * 5) >= 0)
      {
        return false;
      }
      //21. t = t + 1
      t++;
      //22. Go to step 16.
    }

    return true;
  }
}

/*
Generates the p & q, the seeds needed for RSA key generation.
seedLen must be larger than N.
*/
bool RSACipher::genPrimes(const BigInt seed)
{
  //1. If nLen isn't 2048 nor 3072, then return (FAILURE, 0, 0) (separate func)
  if (nLen != nLen1 && nLen != nLen2)
  {
    return false;
  }
  //2. If (e ≤ 2^16 OR e ≥ 2^256 OR e is not odd), then return failure
  //   (already handled)
  //3. Set securityStrength to appropriate val (handled in constructor)
  //4. If (len(seed) != 2 * securityStrength), then return failure
  //   (handled in constructor)

  //5. working_seed = seed.
  BigInt workingSeed = seed;

  BigInt pSeed;
  BigInt qSeed;

  int l = nLen / 2;
  int n1 = 1;
  int n2 = 1;
  BigInt firstSeed = workingSeed;

  //6. Generate p:
  //6.1 Using l = nlen/2, n1 = 1, n2 = 1, firstSeed = working_seed and e,
  //use the provable prime construction method in Appendix C.10 to obtain p and
  //pSeed. If failure is returned, then return failure

  if (!genPrimeFromAuxiliaries(l, n1, n2, firstSeed, pSeed))
  {
    cout << "Generation of the seed for prime p generation failed!" << endl;
    return false;
  }

  //6.2 working_seed = pseed.
  workingSeed = pSeed;

  BigInt absVal = 0;
  BigInt comparisonVal;
  //ComparisonVal = 2^((nLen/2) - 100)
  mpz_ui_pow_ui(comparisonVal.get_mpz_t(), 2, ((nLen / 2) - 100));

  while(cmp(absVal, comparisonVal) <= 0)
  {
    //7. Generate q:
    //7.1 Using L = nlen/2, n1 = 1, n2 = 1, first_seed = working_seed and e,
    //use the provable prime construction method in Appendix C.10 to obtain q
    // and qseed. If FAILURE is returned, then return (FAILURE, 0, 0).
    l = nLen / 2;
    n1 = 1;
    n2 = 1;
    firstSeed = workingSeed;

    if (!genPrimeFromAuxiliaries(l, n1, n2, firstSeed, qSeed))
    {
      cout << "Generation of the seed for prime q generation failed!" << endl;
      return false;
    }

    //7.2 working_seed = qseed.
    workingSeed = qSeed;

    //8. If ( |p – q| ≤ 2^(nlen/2 – 100)), then go to step 7.
    absVal = abs(p - q);
  }
  //9. Zeroize the internally generated seeds: (Not needed)
  //10. Return (SUCCESS, p, q).
  return true;
}

bool RSACipher::genRSAKeys()
{
  //Initializes seed for random number gen
  random_device randSeeder;
  //Creates random generator engine using Mersenne-Twister engine
  mt19937 numGen(randSeeder());
  //Number distribution to select from, 'guaranteed unbiased'
  uniform_int_distribution<int> unbiasedGen(pow(2, 16), UINT_MAX);

  //Seed used for generating primes
  BigInt seed;

  //Generates seed for prime generation
  if(!genFirstSeed(seed))
  {
    cout << "Generation of the first prime generation seed failed!" << endl;
    return false;
  }

  //Gets randomized e value, must be odd
  BigInt exp2_16 = pow(2, 16);
  BigInt exp2_256;
  mpz_ui_pow_ui(exp2_256.get_mpz_t(), 2, 256);

  while ((cmp(e, exp2_16) <=0) || (cmp(e, exp2_256) >= 0) ||
    (e % 2 != 1))
  {
    e = unbiasedGen(numGen);
  }

  //Generates primes p and q for key generation
  if(!genPrimes(seed))
  {
    cout << "Generation of prime values p & q failed!" << endl;
    return false;
  }
  //Gets n value - n = pq
  n = p * q;

  //Gets phi value for n - phiN = (p - 1)(n - 1)
  BigInt phiN = ((p - 1) * (q - 1));

  //Gets private key (d)
  mpz_powm_ui(d.get_mpz_t(), e.get_mpz_t(), -1, phiN.get_mpz_t());
  //Returns success
  return true;
}

bool RSACipher::encrypt(string plainText)
{
  return true;
}

bool RSACipher::decrypt(string cryptText)
{
  return true;
}


