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

}

/*Constructor for specified nLen*/
RSACipher::RSACipher(int inputNLen)
{
  mpz_inits(e, d, n, p, q);
  mpz_set_ui(e, 0);


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
  SHA256 sha;
  const int hashVal0 = 0x6a09e667;
  const int hashVal1 = 0xbb67ae85;
  const int hashVal2 = 0x3c6ef372;
  const int hashVal3 = 0xa54ff53a;
  const int hashVal4 = 0x510e527f;
  const int hashVal5 = 0x9b05688c;
  const int hashVal6 = 0x1f83d9ab;
  const int hashVal7 = 0x5be0cd19;

  char* inputBinary;
  mpz_get_str(inputBinary, 2, inputX);

  vector<vector<char*>> inputBlocks;
  //Length of string returned  by above function is strlen + 1
  int l = sizeof(inputBinary) - 1;
  inputBlocks = sha.padParseInput(inputBinary, l);
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

bool RSACipher::sieveProcedure(mpz_t limitVal)
{
  return true;
}

bool RSACipher::primalityTest(mpz_t potentialPrime)
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
  mpz_init(c);

  mpz_t primeGenCounter;
  mpz_init(primeGenCounter);

  mpz_t exp2_lengthMinus1;
  mpz_init(exp2_lengthMinus1);
  mpz_ui_pow_ui(exp2_lengthMinus1, 2, length - 1);

  //hashOutput0 = hashAlg(primeSeed)
  mpz_t hashOutput0;
  mpz_init(hashOutput0);

  //hashOutput1 = hashAlg(primeSeed + 1)
  mpz_t hashOutput1;
  mpz_init(hashOutput1);

  mpz_t primeSeedPlus1;
  mpz_init(primeSeedPlus1);

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
    mpz_set_ui(primeGenCounter, 0);

    bool loopStep5 = true;
    while (loopStep5)
    {
      //hashOutput0 = hashAlg(primeSeed)
      hashAlg(hashOutput0, primeSeed);

      //hashOutput1 = hashAlg(primeSeed + 1)

      mpz_add_ui(primeSeedPlus1, primeSeed, 1);
      hashAlg(hashOutput1, primeSeedPlus1);
      mpz_clear(primeSeedPlus1);
      //5. c = hashAlg(primeSeed)^hashAlg(primeSeed + 1)
      mpz_xor(c, hashOutput0, hashOutput1);
      //6. c = 2^length-1 + (c mod 2^length-1)
      mpz_t cModExp2_lengthMinus1;
      mpz_init(cModExp2_lengthMinus1);
      mpz_mod(cModExp2_lengthMinus1, c, exp2_lengthMinus1);

      mpz_add(c, exp2_lengthMinus1, cModExp2_lengthMinus1);
      mpz_clear(cModExp2_lengthMinus1);
      //7. c = (2 * floor(c / 2)) + 1
      mpz_t floorC_div_2;
      mpz_init(floorC_div_2);
      mpz_fdiv_q_ui(floorC_div_2, c, 2);

      mpz_t twoFloorCMult2_plusOne;
      mpz_init(twoFloorCMult2_plusOne);
      mpz_mul_ui(twoFloorCMult2_plusOne, floorC_div_2, 2);
      mpz_add_ui(twoFloorCMult2_plusOne, twoFloorCMult2_plusOne, 1);
      //8.primeGenCounter++
      mpz_add_ui(primeGenCounter, primeGenCounter, 1);
      //9.primeSeed += 2
      mpz_add_ui(primeSeed, primeSeed, 2);
      //11. if c is prime,
      if (primalityTest(c))
      {
        //11.1&2 outputPrime = c, return success
        mpz_set(outputPrime, c);
        return true;
      }

      //12. if primeGenCounter > 4*length, return failure
      if (mpz_cmp_ui(primeGenCounter, (4 * length)) > 0)
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
  mpz_add_ui(primeSeed, primeSeed, iterations + 1);
  //21. x = 2^length-1 + x mod 2^(length-1)
  mpz_t xModExp2_lengthMinus1;
  mpz_ui_pow_ui(exp2_lengthMinus1, 2, length - 1);
  mpz_mod(xModExp2_lengthMinus1, x, exp2_lengthMinus1);

  //22.t = ceil(x / (2 * c0))
  mpz_t twoC0;
  mpz_init(twoC0);
  mpz_mul_ui(twoC0, c0, 2);
  mpz_t t;
  mpz_init(t);
  mpz_cdiv_q(t, x, twoC0);

  while (true)
  {
    //23. if (2 * t * c0) + 1 > 2^length) then t = ceil(2^length-1 / (2 * c0))
    mpz_t twoTC0;
    mpz_init(twoTC0);
    mpz_mul(twoTC0, t, twoC0);

    mpz_t twoTC0Plus1;
    mpz_init(twoTC0Plus1);
    mpz_add_ui(twoTC0Plus1, twoTC0, 1);
    mpz_set(c, twoTC0Plus1);

    if (mpz_cmp(twoTC0Plus1, exp2_lengthMinus1) > 0)
    {
      mpz_cdiv_q(t, exp2_lengthMinus1, twoC0);
    }
    //24. c = 2tc0 + 1
    mpz_mul(twoTC0, t, twoC0);
    mpz_add_ui(c, twoTC0, 1);
    //25. primeGenCounter++
    mpz_add_ui(primeGenCounter, primeGenCounter, 1);
    //26. a = 0
    mpz_t a;
    mpz_init_set_ui(a, 0);

    mpz_t primeSeedPlusI;
    mpz_init(primeSeedPlusI);

    //hashOutput2 = hashAlg(primeSeedPlusI)
    mpz_t hashOutput2;
    mpz_init(hashOutput2);

    //27. For i = 0 to iterations, do:
    for (int i = 0; i < iterations; i++)
    {
      mpz_add_ui(primeSeedPlusI, primeSeed, i);
      hashAlg(hashOutput2, primeSeedPlusI);
      //a += hashAlg(primeSeed + i) * 2^i*outlen
      mpz_ui_pow_ui(exp2_iOutLen, 2, i * outLen);
      mpz_addmul(a, hashOutput2, exp2_iOutLen);
    }
    //28. primeSeed += iterations + 1
    mpz_add_ui(primeSeed, primeSeed, iterations + 1);
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
    mpz_init(zMinus1);
    mpz_sub_ui(zMinus1, z, 1);
    mpz_t expZ_c0ModC;
    mpz_init(expZ_c0ModC);
    mpz_powm(expZ_c0ModC, z, c0, c);
    mpz_t gcdZMinus1_c;
    mpz_init(gcdZMinus1_c);
    mpz_gcd(gcdZMinus1_c, zMinus1, c);

    if ((mpz_cmp_ui(gcdZMinus1_c, 1) == 0) && (mpz_cmp_ui(expZ_c0ModC, 1) == 0))
    {
      //31.1 prime = c
      mpz_set(outputPrime, c);
      //31.2 return success
      return true;
    }
    //32. if primeGenCounter >= (4 * length + oldCounter) return failure
    mpz_t oldCounterPlusLengthx4;
    mpz_init(oldCounterPlusLengthx4);
    mpz_add_ui(oldCounterPlusLengthx4, oldCounter, length * 4);
    if (mpz_cmp(primeGenCounter, oldCounterPlusLengthx4) >= 0)
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
  mpz_t p2Seed;
  mpz_inits(p0, p1, p2, pSeed, p0Seed, p2Seed);

  //1. If l, n1, or n2 are not valid, return failure (already handled)
  //2. If n1 == 1
  if (n1 == 1)
  {
    //2.1 p1 = 1
    mpz_set_ui(p1, 1);
    //2.2 p2Seed = firstSeed
    mpz_set(p2Seed, firstSeed);
  }
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
  //6. Using ceil(l / 2) + 1 as the length and p0Seed as the inputSeed, use the random prime
  //generation routine in Appendix C.6 to obtain p0 and pSeed. If failure is returned,
  //then return failure
  if (!randomPrime(ceil(l/2) + 1, p0Seed, p0, pSeed))
  {
    return false;
  }
  mpz_clears(p0Seed, p2Seed);
  int iterations = ceil(l / outLen) - 1;
  //8. pGenCounter = 0
  mpz_t pGenCounter;
  mpz_init_set_ui(pGenCounter, 0);
  //9. x = 0
  mpz_t x;
  mpz_init_set_ui(x, 0);
  //10. For i = 0 to iterations: x = x + (hashAlg(pSeed + i)) ∗ 2^(i * outLen)

  mpz_t pSeedPlus1;
  mpz_init(pSeedPlus1);
  mpz_add_ui(pSeedPlus1, pSeed, 1);

  mpz_t hashOutput0;
  mpz_init(hashOutput0);
  hashAlg(hashOutput0, pSeedPlus1);

  mpz_t exp2_iOutLen;
  mpz_init(exp2_iOutLen);


  for (int i = 0; i < iterations; i++)
  {
    mpz_ui_pow_ui(exp2_iOutLen, 2, i * outLen);
    mpz_addmul(x, hashOutput0, exp2_iOutLen);
  }
  //11. pSeed = pSeed + iterations + 1.
  mpz_add_ui(pSeed, pSeed, iterations + 1);
  //12. x = floor(sqrt(2)*(2^(l−1))) + (x mod (2^(l) − floor(sqrt(2)(2^(l−1)))))

  //floor(sqrt(2)*(2^(l−1)))
  mpf_t root2;
  mpf_init(root2);
  mpf_sqrt_ui(root2, 2);
  mpf_t two;
  mpf_init_set_ui(two, 2);
  mpf_t exp2_lMinus1;
  mpf_init(exp2_lMinus1);
  mpf_pow_ui(exp2_lMinus1, two, l - 1);

  mpf_t floor_Root2xExp2_lMinus1Float;
  mpf_init(floor_Root2xExp2_lMinus1Float);
  mpf_mul(floor_Root2xExp2_lMinus1Float, root2, exp2_lMinus1);
  mpf_floor(floor_Root2xExp2_lMinus1Float, floor_Root2xExp2_lMinus1Float);

  mpz_t floor_Root2xExp2_lMinus1;
  mpz_init(floor_Root2xExp2_lMinus1);
  mpz_set_f(floor_Root2xExp2_lMinus1, floor_Root2xExp2_lMinus1Float);

  mpf_clears(root2, two, exp2_lMinus1);
  //2^(l) − floor(sqrt(2)(2^(l−1))))
  mpz_t exp2_l;
  mpz_init(exp2_l);
  mpz_ui_pow_ui(exp2_l, 2, l);

  mpz_t exp2_l_minus_floor_Root2xExp2_lMinus1;
  mpz_init(exp2_l_minus_floor_Root2xExp2_lMinus1);
  mpz_sub(exp2_l_minus_floor_Root2xExp2_lMinus1, exp2_l, floor_Root2xExp2_lMinus1);

  mpz_t xModexp2_l_minus_floor_Root2xExp2_lMinus1;
  mpz_init(xModexp2_l_minus_floor_Root2xExp2_lMinus1);
  mpz_mod(xModexp2_l_minus_floor_Root2xExp2_lMinus1, x,
    exp2_l_minus_floor_Root2xExp2_lMinus1);

  //floor(sqrt(2)*(2^(l−1))) + (x mod (2^(l) − floor(sqrt(2)(2^(l−1)))))
  mpz_add(x, floor_Root2xExp2_lMinus1, xModexp2_l_minus_floor_Root2xExp2_lMinus1);




  //13. If (gcd(p0*p1, p2) ≠ 1), then return failure
  mpz_t p0xP1;
  mpz_init(p0xP1);
  mpz_mul(p0xP1, p0, p1);

  mpz_t gcdP0xP1_p2;
  mpz_init(gcdP0xP1_p2);
  mpz_gcd(gcdP0xP1_p2, p0xP1, p2);

  if (mpz_cmp_ui(gcdP0xP1_p2, 1) != 0)
  {
    return false;
  }
  mpz_clears(p0xP1, gcdP0xP1_p2);

  //TODO 14. Compute y in the interval [1, p2] such that 0 = ( y*p0*p1–1) mod p2.

  mpz_t y;
  mpz_init(y);
  //15. t = ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2)).

  //(2*y*p0*p1)
  mpz_t twoYxP0xP1;
  mpz_init(twoYxP0xP1);
  mpz_mul(twoYxP0xP1, p0, p1);
  mpz_mul(twoYxP0xP1, twoYxP0xP1, y);
  mpz_mul_ui(twoYxP0xP1, twoYxP0xP1, 2);

  //(2*y*p0*p1) + x
  mpz_t twoYxP0xP1PlusX;
  mpz_init(twoYxP0xP1PlusX);
  mpz_add(twoYxP0xP1PlusX, twoYxP0xP1, x);

  //(2*p0*p1*p2)
  mpz_t twoP0xP1xP2;
  mpz_init(twoP0xP1xP2);
  mpz_mul(twoP0xP1xP2, p0, p1);
  mpz_mul(twoP0xP1xP2, twoP0xP1xP2, p2);
  mpz_mul_ui(twoP0xP1xP2, twoP0xP1xP2, 2);

  //Conversion to floats
  mpf_t twoYxP0xP1PlusXFloat;
  mpf_t twoP0xP1xP2Float;
  mpf_inits(twoYxP0xP1PlusXFloat, twoP0xP1xP2Float);
  mpf_set_z(twoYxP0xP1PlusXFloat, twoYxP0xP1PlusX);
  mpf_set_z(twoP0xP1xP2Float, twoP0xP1xP2);

  //ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2))
  mpf_t ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2;
  mpf_init(ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2);
  mpf_div(ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2, twoYxP0xP1PlusXFloat, twoP0xP1xP2Float);
  mpf_ceil(ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2, ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2);

  //Assignment to t
  mpz_t t;
  mpz_init(t);
  mpz_set_f(t, ceil_twoYxP0xP1PlusX_div_twoP0xP1xP2);


  mpz_t txP2;
  mpz_init (txP2);


  mpz_t txP2MinusY;
  mpz_init(txP2MinusY);
  mpz_mul(txP2MinusY, t, p2);
  mpz_sub(txP2MinusY, txP2MinusY, y);

  mpz_t twoTxP2MinusYxP0xP1Plus1;
  mpz_init(twoTxP2MinusYxP0xP1Plus1);
  mpz_cdiv_q(t, twoYxP0xP1, twoP0xP1xP2);


  mpz_mul(twoTxP2MinusYxP0xP1Plus1, txP2MinusY, p0);
  mpz_mul(twoTxP2MinusYxP0xP1Plus1, twoTxP2MinusYxP0xP1Plus1, p1);
  mpz_mul_ui(twoTxP2MinusYxP0xP1Plus1, twoTxP2MinusYxP0xP1Plus1, 2);
  mpz_add_ui(twoTxP2MinusYxP0xP1Plus1, twoTxP2MinusYxP0xP1Plus1, 1);

  mpz_t divResult;
  mpz_init(divResult);
  while (true)
  {
    //16. If ((2(t*p2 − y)*p0*p1 + 1) > 2^l)
    if (mpz_cmp(twoTxP2MinusYxP0xP1Plus1, exp2_l) > 0)
    {
      mpf_t twoP0xP1xP2Float;
      mpf_init(twoP0xP1xP2Float);
      mpf_set_z(twoP0xP1xP2Float, twoP0xP1xP2);

      mpf_t floor_root2xExp2_lMinus1_div_twoP0xP1xP2;
      mpf_init(floor_root2xExp2_lMinus1_div_twoP0xP1xP2);
      mpf_div(floor_root2xExp2_lMinus1_div_twoP0xP1xP2,
        floor_Root2xExp2_lMinus1Float, twoP0xP1xP2Float);

      mpf_t twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2;
      mpf_init(twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2);
      mpf_add(twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2,
        twoP0xP1xP2Float, floor_root2xExp2_lMinus1_div_twoP0xP1xP2);
      mpf_ceil(twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2,
        twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2);
      mpz_set_f(t,
        twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2);

      mpf_clears(twoP0xP1xP2Float, floor_root2xExp2_lMinus1_div_twoP0xP1xP2,
        twoYxP0xP1_plus_floor_Root2xExp2_lMinus1Float_div_twoP0xP1xP2);
    }
    //17. p = 2(t*p2 − y)*p0*p1 + 1.
    mpz_set(p, twoTxP2MinusYxP0xP1Plus1);
    //18. pGenCounter = pGenCounter + 1.
    mpz_add_ui(pGenCounter, pGenCounter, 1);
    //19. If (GCD(p–1, e) = 1), then
    mpz_t pMinus1;
    mpz_t gcd_pMinus1_e;
    mpz_inits(pMinus1, gcd_pMinus1_e);
    mpz_sub_ui(pMinus1, p, 1);
    mpz_gcd(gcd_pMinus1_e, pMinus1, e);
    if (mpz_cmp_ui(gcd_pMinus1_e, 1) == 0)
    {
      //19.1 a = 0
      mpz_t a;
      mpz_t hashOutput;
      mpz_inits(a, hashOutput);
      mpz_add_ui(pSeedPlus1, pSeed, 1);
      hashAlg(hashOutput, pSeedPlus1);
      //19.2 For i = 0 to iterations do: a = a + (Hash(pseed + i))∗ 2 i * outlen.
      for (int i = 0; i < iterations; i++)
      {
        mpz_ui_pow_ui(exp2_iOutLen, 2, i * outLen);
        mpz_addmul(a, hashOutput, exp2_iOutLen);
      }
      //19.3 pSeed = pSeed + iterations + 1.
      mpz_add_ui(pSeed, pSeed, iterations + 1);
      //19.4 a = 2 + (a mod (p–3)).
      mpz_t pMinus3;
      mpz_t zMinus1;
      mpz_t gcd_zMinus1_p;
      mpz_inits(pMinus3, zMinus1);
      mpz_sub_ui(pMinus3, p, 3);

      mpz_mod(a, a, pMinus3);

      mpz_gcd(gcd_zMinus1_p, zMinus1, p);
      //19.5 z = a^(2(t*p2 − y)*p1) mod p.
      mpz_t z;
      mpz_t expZ_p0ModP;
      mpz_inits(z, expZ_p0ModP);
      mpz_powm(z, a, twoTxP2MinusYxP0xP1Plus1, p);
      mpz_powm(expZ_p0ModP, z, p0, p);
      mpz_sub_ui(zMinus1, z, 1);
      //19.6 If ((1 = GCD(z–1, p)) and (1 = (z^(p0) mod p)), then return success
      if ((mpz_cmp_ui(gcd_zMinus1_p, 1) == 0) && (mpz_cmp_ui(expZ_p0ModP, 1) == 0))
      {
        return true;
      }
      //20. If (pgen_counter ≥ 5L), then return failure
      if (mpz_cmp_ui(pGenCounter, l * 5) >= 0)
      {
        return false;
      }
      //21. t = t + 1
      mpz_add_ui(t, t, 1);
      //22. Go to step 16.
    }
    mpz_add_ui(pGenCounter, pGenCounter, 1);

    mpz_clear(pSeed);
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
  mpz_set_ui(absVal, 0);
  mpz_t comparisonVal;
  mpz_inits(comparisonVal);
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
  mt19937 numGen(randSeeder());
  //Number distribution to select from, 'guaranteed unbiased'
  uniform_int_distribution<int> unbiasedGen(pow(2, 16), UINT_MAX);

  //Seed used for generating primes
  mpz_t seed;
  mpz_init(seed);

  //Generates seed for prime generation
  if(!genFirstSeed(seed))
  {
    cout << "Generation of the first prime generation seed failed!" << endl;
    return false;
  }

  //Gets randomized e value, must be odd
  mpz_t exp2_16;
  mpz_t exp2_256;
  mpz_inits(exp2_16, exp2_256);

  mpz_ui_pow_ui(exp2_16, 2, 16);
  mpz_ui_pow_ui(exp2_256, 2, 256);
  while ((mpz_cmp(e, exp2_16) <=0) || (mpz_cmp(e, exp2_256) >= 0) ||
    (mpz_odd_p(e) == 0))
  {
    mpz_set_ui(e, unbiasedGen(numGen));
  }

  mpz_clears(exp2_16, exp2_256);


  //Generates primes p and q for key generation
  if(!genPrimes(e, seed, p, q))
  {
    cout << "Generation of prime values p & q failed!" << endl;
    return false;
  }
  //Gets n value - n = pq
  mpz_mul(n, p, q);

  //Gets phi value for n - phiN = (p - 1)(n - 1)

  mpz_t pMinus1;
  mpz_init(pMinus1);
  mpz_sub_ui(pMinus1, p, 1);
  mpz_t qMinus1;
  mpz_init(qMinus1);
  mpz_sub_ui(qMinus1, q, 1);

  mpz_t phiN;
  mpz_init(phiN);

  mpz_mul(phiN, pMinus1, qMinus1);;

  //Gets private key (d)
  mpz_powm_ui(d, e, -1, phiN);

  mpz_clears(seed, phiN, pMinus1, qMinus1);
  //Returns success
  return true;
}

bool RSACipher::encrypt(mpz_t e, mpz_t n, string plainText)
{
  return true;
}

bool RSACipher::decrypt(mpz_t d, mpz_t n, string cryptText)
{
  return true;
}


