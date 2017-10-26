/*
RSACipher.cpp - George Wood
*/

#include <random>
#include <math.h>
#include <climits>
#include <iostream>
#include "RSACipher.hpp"
using namespace std;

//TODO - ALL MPZs ARE BY REFERENCE
/*Default constructor for the RSA cipher*/
RSACipher::RSACipher()
{

}

/*Constructor for specified nLen*/
RSACipher::RSACipher(int inputNLen)
{
  mpz_inits(e, d, n, p, q);


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

//TODO hash alg
void RSACipher::hashAlg( mpz_t outputX, const mpz_t inputX)
{

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

bool sieveProcedure(mpz_t limitVal)
{
  return true;
}

bool primalityTest(mpz_t potentialPrime)
{
  //TODO 1. Prepare a table of primes less than sqrt(c) using the sieve procedure in (C.8)
  //vector<mpz_t> smallerPrimes = sieveProcedure(modexp(potentialPrime, 1/2, 1));
  //2. Divide potential prime by all values in table. If divisible, return failure
  //3. If not divisible, return success

  return true;
}

//Appendix C.6 0 Shawe-Taylor Random Prime Routine
bool RSACipher::randomPrime(int length, mpz_t inputSeed,
  mpz_t outputPrime, mpz_t primeSeed)
{
  mpz_t c;
  mpz_t primeGenCounter;
  mpz_t hashOutput0;
  mpz_t hashOutput1;
  mpz_t primeSeedPlus1;
  mpz_t exp2_lengthMinus1;
  mpz_inits(c, primeGenCounter, hashOutput0, hashOutput1, primeSeedPlus1, exp2_lengthMinus1);

  //1. If (length < 2), then return failure
  if (length < 2)
  {
    return false;
  }
  //2. If (length ≥ 33), then go to step 14.
  if (length < 33)
  {
    //3. primeSeed = inputSeed
    mpz_set(primeSeed, inputSeed);
    //4. primeGenCounter = 0;

    mpz_t cModExp2;
    mpz_t floorCDiv2;
    mpz_t floorCMult2;
    mpz_inits(cModExp2, floorCDiv2, floorCMult2);
    mpz_add_ui(primeSeedPlus1, primeSeed, 1);
    hashAlg(hashOutput0, primeSeed);
    hashAlg(hashOutput1, primeSeedPlus1);
    mpz_ui_pow_ui(exp2_lengthMinus1, 2, length - 1);
    mpz_mod(cModExp2, c, exp2_lengthMinus1);


    bool loopStep5 = true;
    while (loopStep5)
    {
      //5. c = hashAlg(primeSeed)^hashAlg(primeSeed + 1)
      mpz_xor(c, hashOutput0, hashOutput1);
      //6. c = 2^length-1 + (c mod 2^length-1)
      mpz_add(c, exp2_lengthMinus1, cModExp2);
      //7. c = (2 * floor(c / 2)) + 1
      mpz_fdiv_q_ui(floorCDiv2, c, 2);
      mpz_mul_ui(floorCMult2, floorCDiv2, 2);
      mpz_add_ui(c, floorCMult2, 1);
      //8.primeGenCounter++
      mpz_add_ui(primeGenCounter, primeGenCounter, 1);
      //9.primeSeed += 2
      mpz_add_ui(primeSeed, primeSeed, 2);
      //10. TODO prove that c is prime, appendix c.7
      //11. if c is prime,
      if (primalityTest(c))
      {
        //11.1&2 outputPrime = c, return success
        mpz_set(outputPrime, c);
        return true;
      }

      //12. if primeGenCounter > 4*length, return failure
      if (mpz_cmp_ui(primeGenCounter, (4 * length)) > 1)
      {
        return false;
      }
      //13. Go to step 5
    }
  }
  //14. randomPrime(floor(length / 2) + 1, inputSeed)
  mpz_t c0;
  mpz_init(c0);
  if (!randomPrime(floor(length / 2) + 1, inputSeed, c0, primeSeed))
  {
    //15. If failure is returned, return failure
    return false;
  }
  //16. iterations = ceil(length / outlen) - 1;
  int iterations = ceil(length / outLen) - 1;
  //17. oldCounter = primeGenCounter
  mpz_t oldCounter;
  mpz_init_set(oldCounter, primeGenCounter);
  //18. x = 0
  mpz_t x;
  mpz_t exp2_iOutLen;

  mpz_inits(x, exp2_iOutLen);

  mpz_add_ui(primeSeedPlus1, primeSeed, 1);
  hashAlg(hashOutput1, primeSeedPlus1);

  //19. For i = 0 to iterations, do x += hashAlg(primeSeed + i) * 2^i*outlen
  for (int i = 0; i < iterations; i++)
  {
    mpz_ui_pow_ui(exp2_iOutLen, 2, i * outLen);
    mpz_addmul(x, hashOutput1, exp2_iOutLen);
  }
  //20. primeSeed += iterations + 1
  primeSeed += iterations + 1;
  //21. x = 2^length-1 + x mod 2^(length-1)
  mpz_t xModExp2_lengthMinus1;
  mpz_ui_pow_ui(exp2_lengthMinus1, 2, length - 1);
  mpz_mod(xModExp2_lengthMinus1, x, exp2_lengthMinus1);
  //22.t = ceil(x / 2 * c0)

  mpz_t twoC0;
  mpz_t t;
  mpz_inits(twoC0, t);
  mpz_mul_ui(twoC0, c0, 2);
  mpz_cdiv_q(t, x, twoC0);

  mpz_t twoTC0;
  mpz_t twoTC0Plus1;
  mpz_inits(twoTC0, twoTC0Plus1);
  mpz_mul(twoTC0, t, twoC0);
  mpz_add_ui(twoTC0Plus1, twoTC0, 1);

  while (true)
  {
    //23. if (2 * t * c0) + 1 > 2^length) then t = [2^length-1 / (2 * c0)]
    if (mpz_cmp(twoTC0Plus1, exp2_lengthMinus1) > 0)
    {
      mpz_cdiv_q(t, exp2_lengthMinus1, twoC0);
    }
    //24. c = 2tc0 + 1
    mpz_set(c, twoTC0Plus1);
    //25. primeGenCounter++
    mpz_add_ui(primeGenCounter, primeGenCounter, 1);
    //26. a = 0
    mpz_t a;
    mpz_init(a);

    mpz_add_ui(primeSeedPlus1, primeSeed, 1);
    hashAlg(hashOutput1, primeSeedPlus1);
    //27. For i = 0 to iterations, do:
    for (int i = 0; i < iterations; i++)
    {
      //a += hashAlg(primeSeed + i) * 2^i*outlen
      mpz_ui_pow_ui(exp2_iOutLen, 2, i * outLen);
      mpz_addmul(a, hashOutput1, exp2_iOutLen);
    }
    //28. primeSeed += iterations + 1
    primeSeed += iterations + 1;
    //29. a = 2 + a mod (c - 3)
    mpz_t cMinus3;
    mpz_init(cMinus3);
    mpz_sub_ui(cMinus3, c, 3);
    mpz_mod(a, a, cMinus3);
    mpz_add_ui(a, a, 2);
    //30. z = a^2t mod c
    mpz_t z;
    mpz_t twoT;
    mpz_inits(z, twoT);
    mpz_mul_ui(twoT, t, 2);
    mpz_powm(z, a, twoT, c);

    //31. if (gcd(z-1, c) == 1) and (z^c0 mod c == 1) then
    mpz_t zMinus1;
    mpz_t expZ_c0ModC;
    mpz_t gcdZMinus1_c;
    mpz_inits(zMinus1, expZ_c0ModC, gcdZMinus1_c);
    mpz_sub_ui(zMinus1, z, 1);
    mpz_powm(expZ_c0ModC, z, c0, c);
    mpz_gcd(gcdZMinus1_c, zMinus1, c);
    if ((mpz_cmp_ui(gcdZMinus1_c, 1) == 0) && (mpz_cmp_ui(expZ_c0ModC, 1) == 0))
    {
      //31.1 prime = c
      outputPrime = c;
      //31.2 return success
      return true;
    }
    //32. if primeGenCounter >= (4 * length + oldCounter) return failure
    if (primeGenCounter >= (oldCounter + (length * 4)))
    {
      return false;
    }
    //33. t++
    mpz_add_ui(t, t, 1);
    //34. Go to step 23
  }
}

bool RSACipher::genFirstSeed(mpz_t seed)
{
  //1. If invalid nLen, return failure (done in constructor)
  //2. Set security_strength to corresponding val for nLen (done in constructor)
  //3. Obtain (2 * security_strength) bit string from a valid RBG
  mpz_set_str (seed, genRandBits(securityStrength * 2).c_str(), 2);
  //4. Return success
  return true;
}

//Appendix C.10
bool RSACipher::genPrimeFromAuxiliaries(int l, int n1, int n2,
  mpz_t firstSeed, const mpz_t e, mpz_t outputPrime, mpz_t outputSeed)
{
  mpz_t p0;
  mpz_t p1;
  mpz_t p2;
  mpz_t pSeed;
  mpz_t p0Seed;
  mpz_t p1Seed;
  mpz_t p2Seed;
  mpz_inits(p0, p1, p2, pSeed, p0Seed, p1Seed, p2Seed);

  //1. If l, n1, or n2 are not valid, return failure (already handled)
  //2. If n1 == 1
  if (n1 == 1)
  {
    //2.1 p1 = 1
    mpz_set_ui(p1, 1);
    //2.2 p2Seed = firstSeed
    mpz_set(p2Seed, firstSeed);
  }
  //TODO 3. If n1 >= 2
  else if (n1 >= 2)
  {
    //3.1  Using n1 as the length and firstSeed as the input_seed, use the random prime
    // generation routine in Appendix C.6 to obtain p1 and p2Seed.
    //3.2 If failure is returned, return failure
    if (!randomPrime(n1, firstSeed, p1, p2Seed))
    {
      return false;
    }
  }
  //4. If n2 == 1
  if (n2 == 1)
  {
    //4.1 p2 = 1
    mpz_set_ui(p2, 1);
    //4.2 p0Seed = p2Seed
    mpz_set(p0Seed, p2Seed);
  }
  //TODO 5. If n2 >= 2
  else if (n2 >= 2)
  {
    //5.1  Using n2 as the length and p2Seed as the inputSeed, use the random prime
    // generation routine in Appendix C.6 to obtain p2 and p0Seed.
    //5.2 If fairlure is returned, return failure
    if (!randomPrime(n2, p2Seed, p2, p0Seed))
    {
      return false;
    }
  }
  //6. TODO Using ceil(l / 2) + 1 as the length and p0Seed as the inputSeed, use the random prime
  //generation routine in Appendix C.6 to obtain p0 and pSeed. If failure is returned,
  //then return failure
  if (!randomPrime(ceil(l/2) + 1, p0Seed, p0, pSeed))
  {
    return false;
  }
  //TODO 7. 7. iterations = ceil(l / outLen) − 1
  //TODO what is outLen?
  int iterations = ceil(l / outLen) - 1;
  //8. pGenCounter = 0
  mpz_t pGenCounter;
  mpz_init(pGenCounter);
  //9. x = 0
  mpz_t x;
  mpz_init(x);
  //10. For i = 0 to iterations: x = x + (hashAlg(pSeed + i)) ∗ 2^(i * outLen)
  mpz_t hashOutput0;
  mpz_t pSeedPlus1;
  mpz_t exp2_iOutlen;
  mpz_inits(hashOutput0, pSeedPlus1, exp2_iOutlen);

  mpz_add_ui(pSeedPlus1, pSeed, 1);
  hashAlg(hashOutput0, pSeedPlus1);


  for (int i = 0; i < iterations; i++)
  {
    mpz_ui_pow_ui(exp2_iOutlen, 2, i * outLen);
    mpz_addmul(x, hashOutput0, exp2_iOutlen);
  }
  //11. pSeed = pSeed + iterations + 1.
  mpz_add_ui(pSeed, pSeed, iterations + 1);
  //12. x = floor(sqrt(2)*(2^(l−1))) + (x mod (2^(l) − floor(sqrt(2)(2^(l−1)))))
  mpz_t exp2_lMinus1;
  mpz_t exp2_l;
  mpz_t xModExp2_l;
  mpz_t rootTwoXModExp2_l;
  mpz_t rootTwo_exp2_lMinus1;
  mpz_inits(exp2_lMinus1, exp2_l, xModExp2_l, rootTwoXModExp2_l, rootTwo_exp2_lMinus1);
  mpz_ui_pow_ui(exp2_lMinus1, 2, l - 1);
  mpz_ui_pow_ui(exp2_l, 2, l);
  mpz_mod(xModExp2_l, x, exp2_l);
  mpz_mul_ui(rootTwoXModExp2_l, xModExp2_l, sqrt(2));
  mpz_fdiv_q_ui(rootTwoXModExp2_l, rootTwoXModExp2_l, 1 / sqrt(2));
  mpz_fdiv_q_ui(rootTwo_exp2_lMinus1, exp2_l, 1 / sqrt(2));

  mpz_add(x, rootTwoXModExp2_l, xModExp2_l);
  mpz_sub(x, x, rootTwo_exp2_lMinus1);
  //13. If (gcd(p0*p1, p2) ≠ 1), then return failure
  mpz_t p0P1;
  mpz_t gcdP0P1_p2;
  mpz_inits(p0P1, gcdP0P1_p2);
  mpz_mul(p0P1, p0, p1);
  mpz_gcd(gcdP0P1_p2, p0P1, p2);

  if (mpz_cmp_ui(gcdP0P1_p2, 1) != 0)
  {
    return false;
  }
  //TODO 14. Compute y in the interval [1, p2] such that 0 = ( y*p0*p1–1) mod p2.
  mpz_t y;
  mpz_t twoYP0P1;
  mpz_t p0P1P2;
  mpz_inits(y, twoYP0P1, p0P1P2);
  //15. t = ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2)).
  mpz_t t = ceil(((y * p0 * p1 * 2) + x) / (p0 * p1 * p2 * 2));
  while (true)
  {
    //16. If ((2(t*p2 − y)*p0*p1 + 1) > 2^l)
    if (mpz_cmp(((t * p2 - y) * p0 * p1 * 2 + 1), exp2_l) > 0)
    {
      //then t = ceil((2*y*p0*p1) + floor((sqrt(2))(2^(l−1))) / (2*p0*p1*p2))
      t = ceil((y * 2 * p0 * p1) + floor(modexp(2, l - 1, 1)) / (p0 * p1 * p2 * 2));
    }
    //17. p = 2(t*p2 − y)*p0*p1 + 1.
    p = (t * p2 - y) * p0 * p1 * 2 + 1;
    //18. pGenCounter = pGenCounter + 1.
    pGenCounter++;
    //19. If (GCD(p–1, e) = 1), then
    if (gcd(p - 1, e) == 1)
    {
      //19.1 a = 0
      mpz_t a = 0;
      //19.2 For i = 0 to iterations do: a = a + (Hash(pseed + i))∗ 2 i * outlen.
      for (int i = 0; i < iterations; i++)
      {
        a += hashAlg(pSeed + 1) * modexp(2, i * outLen, 1);
      }
      //19.3 pSeed = pSeed + iterations + 1.
      pSeed += iterations + 1;
      //19.4 a = 2 + (a mod (p–3)).
      a = modexp(a, 1, p - 3) + 2;
      //19.5 z = a^(2(t*p2 − y)*p1) mod p.
      mpz_t z = modexp(a, (t * p2 - y) * p1 * 2, p);
      //19.6 If ((1 = GCD(z–1, p)) and (1 = (z^(p0) mod p)), then return success
      if ((gcd(z - 1, p) == 1) && (modexp(z, p0, p) == 1))
      {
        return true;
      }
      //20. If (pgen_counter ≥ 5L), then return failure
      if (pGenCounter >= l * 5)
      {
        return false;
      }
      //21. t = t + 1
      t++;
      //22. Go to step 16.
    }
    pGenCounter++;
    return true;
  }
}

/*
Generates the p & q, the seeds needed for RSA key generation.
seedLen must be larger than N.
*/
bool RSACipher::genPrimes(const mpz_t e, const mpz_t seed,
  mpz_t p, mpz_t q)
{
  //1. If nLen isn't 2048 nor 3072, then return (FAILURE, 0, 0) (separate func)
  if (nLen != nLen1 && nLen != nLen2)
  {
    return false;
  }
  //2. If (e ≤ 2^16 OR e ≥ 2^256 OR e is not odd), then return failure (already handled)
  //3. Set securityStrength to appropriate val (handled in constructor)
  //4. If (len(seed) != 2 * securityStrength), then return failure (handled in constructor)

  //5. working_seed = seed.
  mpz_t workingSeed;
  mpz_init_set(workingSeed, seed);

  mpz_t pSeed;
  mpz_t qSeed;
  mpz_t firstSeed;
  mpz_inits(pSeed, qSeed);
  mpz_init_set(firstSeed, workingSeed);

  //6. Generate p:
  //6.1 Using l = nlen/2, n1 = 1, n2 = 1, firstSeed = working_seed and e,
  //use the provable prime construction method in Appendix C.10 to obtain p and
  //pSeed. If failure is returned, then return failure

  int l = nLen / 2;
  int n1 = 1;
  int n2 = 1;

  if (!genPrimeFromAuxiliaries(l, n1, n2, firstSeed, e, p, pSeed))
  {
    cout << "Generation of the seed for prime p generation failed!" << endl;
    return false;
  }

  //6.2 working_seed = pseed.
  mpz_set(workingSeed, pSeed);

  mpz_t absVal;
  mpz_init(absVal);
  mpz_t comparisonVal;
  mpz_init(comparisonVal);
  //ComparisonVal = 2^((nLen/2) - 100)
  mpz_ui_pow_ui(comparisonVal, 2, ((nLen / 2) - 100));

  while(mpz_cmp(absVal, comparisonVal) <= 0)
  {
    //7. Generate q:
    //7.1 Using L = nlen/2, n1 = 1, n2 = 1, first_seed = working_seed and e,
    //use the provable prime construction method in Appendix C.10 to obtain q
    // and qseed. If FAILURE is returned, then return (FAILURE, 0, 0).
    if (!genPrimeFromAuxiliaries(l, n1, n2, firstSeed, e, q, qSeed))
    {
      cout << "Generation of the seed for prime q generation failed!" << endl;
      return false;
    }

    //7.2 working_seed = qseed.
    mpz_set(workingSeed, qSeed);

    //8. If ( |p – q| ≤ 2^(nlen/2 – 100)), then go to step 7.
    mpz_sub(absVal, p, q);
    mpz_abs(absVal, absVal);
  }
  mpz_clears(pSeed, qSeed, firstSeed, workingSeed, absVal, comparisonVal);
  //9. Zeroize the internally generated seeds: (Not needed)
  //10. Return (SUCCESS, p, q).
  return true;
}

bool RSACipher::genRSAKeys()
{
  //Initializes seed for random number gen
  random_device randSeeder;
  //Creates random generator engine using Mersenne-Twister engine
  //TODO investigate what this is
  mt19937 numGen(randSeeder());
  //Number distribution to select from, 'guaranteed unbiased'
  uniform_int_distribution<int> unbiasedGen(pow(2, 16), UINT_MAX);

  //Gets randomized e value, must be odd
  //TODO must be declared later, if FIPS allows for it
  mpz_t exp2_16;
  mpz_t exp2_256;
  mpz_t eMod2;
  mpz_inits(exp2_16, exp2_256, eMod2);

  mpz_ui_pow_ui(exp2_16, 2, 16);
  mpz_ui_pow_ui(exp2_256, 2, 256);
  mpz_fdiv_r_ui(eMod2, e, 2);
  while ((mpz_cmp(e, exp2_16) <=0) || (mpz_cmp(e, exp2_256) >= 0) ||
    (mpz_cmp_ui(eMod2, 1) != 0))
  {
    mpz_set_ui(e, unbiasedGen(numGen));
  }

  mpz_clears(exp2_16, exp2_256, eMod2);

  //Seed used for generating primes
  mpz_t seed;
  mpz_init(seed);

  //Generates seed for prime generation
  if(!genFirstSeed(seed))
  {
    cout << "Generation of the first prime generation seed failed!" << endl;
    return false;
  }
  //Generates primes p and q for key generation
  if(!genPrimes(e, seed, p, q))
  {
    cout << "Generation of prime values p & q failed!" << endl;
    return false;
  }
  //Gets n value - n = pq
  mpz_mul(n, p, q);

  //Gets phi value for n - phiN = (p - 1)(n - 1)
  mpz_t phiN;
  mpz_t pMinus1;
  mpz_t qMinus1;
  mpz_inits(phiN, pMinus1, qMinus1);

  mpz_sub_ui(pMinus1, p, 1);
  mpz_sub_ui(qMinus1, q, 1);
  mpz_mul(phiN, pMinus1, qMinus1);;

  //Gets private key (d)
  mpz_powm_ui(d, e, -1, phiN);

  mpz_clears(seed, phiN, pMinus1, qMinus1);
  //Returns success
  return true;
}

//TODO input text separated into blocks
bool RSACipher::encrypt(mpz_t e, mpz_t n, string plainText)
{
  mpz_t binaryPlainText;
  mpz_init_set_str(binaryPlainText, plainText.c_str());
  mpz_t binarycryptText;// = modexp(binaryPlainText, e, n);
  return true;
}

bool RSACipher::decrypt(mpz_t d, mpz_t n, string cryptText)
{
  mpz_t binaryCryptText;
  mpz_t binaryPlainText;// = modexp(binaryCryptText, d, n);
  return true;
}

