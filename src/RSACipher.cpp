/*
RSACipher.cpp - George Wood - RSA 2048 and SHA 224
*/

#include <random>
#include <time.h>
#include <math.h>
#include <climits>
#include <iostream>
#include "RSACipher.hpp"
#include "SHA224.hpp"
using namespace std;

//TODO approvaed random bit generator?
/*Default constructor for the RSA cipher*/
RSACipher::RSACipher()
{
  nLen = nLen1;
  securityStrength = validPairs.at(nLen1);
}

/*Constructor for specified nLen*/
RSACipher::RSACipher(uint inputNLen)
{
  if (inputNLen == nLen0 || inputNLen == nLen1 || inputNLen == nLen2)
  {
    nLen = inputNLen;
    securityStrength = validPairs.at(inputNLen);
  }
  else
  {
    cout << "Invalid nLen input." << endl;
  }
}

BigInt RSACipher::hashAlg(const BigInt inputX)
{
  //TODO maybe use SHA224?
  uint hexBase = 16;
  SHA224 sha;
  string inputHex = inputX.get_str(hexBase);
  //Length of string returned  by above function is strlen + 1
  string hashedHex = sha.hash(inputHex);
  BigInt hashedInt;
  hashedInt.set_str(hashedHex, hexBase);
  return hashedInt;
}

vector<bool> RSACipher::sieveProcedure(uint limitVal)
{
  const int minPrime = 2;
  vector<bool> primes(limitVal, true);
  primes.at(0) = false;
  primes.at(1) = false;
  for (uint i = minPrime; i < limitVal; i++)
  {
    if (primes.at(i))
    {
      for (uint j = i * 2; j < limitVal; j += i)
      {
        primes.at(j) = false;
      }
    }
  }

  return primes;
}

bool RSACipher::primalityTest(BigInt c)
{
  //1. Prepare a table of primes < sqrt(c) using the sieve procedure in (C.8)
  //vector<BigInt> smallerPrimes = sieveProcedure(sqrt(c));

  //2. Divide potential prime by all values in table.
  //If divisible, return failure
  BigInt testPrime = 2;
  BigInt rootC = sqrt(c);
  vector<bool> primes = sieveProcedure(rootC.get_ui());
  for (uint i = 0; i < primes.size(); i++)
  {
    if (primes.at(i))
    {
      uint testPrime = i;
      if ((mpz_divisible_ui_p(c.get_mpz_t(), testPrime)) != 0)
      {
        //cout << "  " << c << " is divisible by " << testPrime << endl;
        return false;
      }
    }
  }
  // while (cmp(testPrime, rootC) < 0)
  // {
  //   if (cmp(gcd(testPrime, c), 1) != 0)
  //   {
  //     //cout << "  " << c << " is divisible by " << testPrime << endl;
  //     return false;
  //   }
  //   mpz_nextprime(testPrime.get_mpz_t(), testPrime.get_mpz_t());
  // }
  //3. If not divisible, return success
  return true;
}





//Appendix C.6 0 Shawe-Taylor Random Prime Routine
bool RSACipher::randomPrime(const uint length, const BigInt inputSeed,
  BigInt& outputPrime, BigInt& primeSeed, BigInt& pGenCounter)
{
  BigInt c;

  //Commonly used in later equations
  BigInt exp2_lMinus1;
  mpz_ui_pow_ui(exp2_lMinus1.get_mpz_t(), 2, length - 1);
  BigInt exp2_l;
  mpz_ui_pow_ui(exp2_l.get_mpz_t(), 2, length);


  //cout << length << endl;
  //1. If (length < 2), then return failure
  if (length < 2)
  {
    return false;
  }
  //2. If (length ≥ 33), then go to step 14.
  if (length < 33)
  {
    // cout << "less than 33" << endl;
    //3. primeSeed = inputSeed
    primeSeed = inputSeed;
    //4. pGenCounter = 0;
    pGenCounter = 0;

    bool loopStep5 = true;
    while (loopStep5)
    {
      //5. c = hashAlg(primeSeed)^hashAlg(primeSeed + 1)
      c = hashAlg(primeSeed)^hashAlg(primeSeed + 1);
      // cout << c << endl;
      //6. c = 2^length-1 + (c mod 2^length-1)
      c = exp2_lMinus1 + (c % exp2_lMinus1);


      //7. c = (2 * floor(c / 2)) + 1
      BigFloat cFloat(c);
      c = ((2 * floor(cFloat / 2)) + 1);
      //8.pGenCounter++
      pGenCounter++;
      //9.primeSeed += 2
      primeSeed += 2;
      //11. if c is prime,
      // cout << endl << " new c attempt: " << endl << c << endl;
      //cout << "testing primality of " << c << endl;
      if (primalityTest(c))
      {
        // cout << "prime!" << endl;
        //11.1&2 outputPrime = c, return success
        outputPrime = c;
        return true;
      }

      //12. if pGenCounter > 4*length, return failure
      if (cmp(pGenCounter, (4 * length)) > 0)
      {
        return false;
      }
      //13. Go to step 5
    }
  }
  //14. randomPrime(ceil(length / 2) + 1, inputSeed)
  BigInt c0;
  //pGenCounter getting passed by reference is important only here
  if (!randomPrime(ceil(length / 2) + 1, inputSeed, c0, primeSeed,
                   pGenCounter))
  {
    //15. If failure is returned, return failure
    return false;
  }
  // cout << "c0: " << c0 << endl;
  //16. iterations = ceil(length / outlen) - 1;
  uint outLen = SHA224::outputBlockSize;
  uint iterations = ceil((float)length / outLen) - 1;
  //17. oldCounter = pGenCounter
  BigInt oldCounter = pGenCounter;
  //18. x = 0
  BigInt x = 0;

  //19. For i = 0 to iterations, do x += hashAlg(primeSeed + i) * 2^i*outlen
  for (uint i = 0; i < iterations; i++)
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
    // cout << "c after computation: " << c << endl;
    //25. pGenCounter++
    pGenCounter++;
    //26. a = 0
    BigInt a = 0;

    //27. For i = 0 to iterations, do:
    for (uint i = 0; i < iterations; i++)
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
      // cout << "  Output prime: " << outputPrime << endl;
      //31.2 return success
      return true;
    }
    //32. if pGenCounter >= (4 * length + oldCounter) return failure

    if (cmp(pGenCounter, ((4 * length) + oldCounter)) >= 0)
    {
      return false;
    }
    //33. t++
    t++;
    //34. Go to step 23
  }
}

// bool RSACipher::genFirstSeed(BigInt& seed)
// {
//   //1. If invalid nLen, return failure (done in constructor)
//   //2. Set security_strength to corresponding val for nLen
//        (done in constructor)
//   //3. Obtain (2 * security_strength) bit string from a valid RBG
//   seed.set_str(genRandBits(securityStrength * 2), 2);
//   //4. Return success
//   return true;
// }

//Appendix C.10
bool RSACipher::genPrimeFromAuxiliaries(const uint l, const uint n1,
  const uint n2, const BigInt firstSeed, BigInt& outputPrime, BigInt& primeSeed)
{
  BigInt p0;
  BigInt p1;
  BigInt p2;
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
    BigInt temp = 0;
    if (!randomPrime(n1, firstSeed, p1, p2Seed, temp))
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
    BigInt temp = 0;
    if (!randomPrime(n2, p2Seed, p2, p0Seed, temp))
    {
      //5.2 If fairlure is returned, return failure
      return false;
    }
  }
  // cout << p1 << endl << p2Seed << endl << p2 << endl << p0Seed << endl;
  //6. Using ceil(l / 2) + 1 as the length and p0Seed as the inputSeed,
  //use the random prime generation routine in Appendix C.6 to obtain
  //p0 and primeSeed. If failure is returned, then return failure
  BigInt temp = 0;
  if (!randomPrime(ceil((float)l/2) + 1, p0Seed, p0, primeSeed, temp))
  {
    return false;
  }
  // cout << ceil((float)l/2) << endl;
  // cout << mpz_sizeinbase(p0.get_mpz_t(), 2) << endl;

  // cout << "p0: " << p0 << endl;
  // cout << "p1: " << p1 << endl;
  // cout << "p2: " << p2 << endl;
  //7. iterations = ceil(l / outLen) − 1, where outLen is the length of the
  //hash function output block
  uint outLen = SHA224::outputBlockSize;
  uint iterations = ceil((float)l / outLen) - 1;
  //8. pGenCounter = 0
  BigInt pGenCounter = 0;
  //9. x = 0
  BigInt x = 0;
  //10. For i = 0 to iterations: x = x + (hashAlg(primeSeed + i)) ∗ 2^(i * outLen)
  for (uint i = 0; i < iterations; i++)
  {
    BigInt exp2_ixOutLen;
    mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
    x += (hashAlg(primeSeed + i) * exp2_ixOutLen);
  }
  //11. primeSeed = primeSeed + iterations + 1.
  primeSeed += iterations + 1;

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

  //14. Compute y in the interval [1, p2] such that 0 = (y*p0*p1–1) mod p2.
  BigInt y = 0;
  BigInt p0p1 = p0 * p1;

  if (!mpz_invert(y.get_mpz_t(), p0p1.get_mpz_t(), p2.get_mpz_t()))
  {
    cout << "No inverse for p0p1 mod p2 = 1!" << endl;
    return false;
  }

  // cout << "p0: " << p0 << endl;
  // cout << endl << "p1: " << p1 << endl;
  // cout << endl << "p0p1: " << p0p1 << endl;
  // cout << endl << "p2: " << p2 << endl;
  // cout << endl << "y: " << y << endl;

  //15. t = ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2)).
  // cout << "y: " << y << endl;
  // cout << "x: " << x << endl;
  BigInt twoxYxP0xP1 = (2 * y * p0 * p1);
  BigInt twoxP0xP1xP2 = (2 * p0 * p1 * p2);
  leftHalf = twoxYxP0xP1 + x;
  rightHalf = twoxP0xP1xP2;
  BigInt t;
  mpz_cdiv_q(t.get_mpz_t(), leftHalf.get_mpz_t(), rightHalf.get_mpz_t());
  // cout << "t: " << t << endl;

  while (true)
  {
    //16. If ((2(t*p2 − y)*p0*p1 + 1) > 2^l)
    BigInt compVal = ((2 * ((t * p2) - y) * p0 * p1) + 1);
    if (cmp(compVal, exp2_l) > 0)
    {
      //then t = ceil(((2*y*p0*p1) + floor((sqrt(2))(2^(L−1)))) / (2*p0*p1*p2))
      leftHalf = twoxYxP0xP1 + floorRoot2xExp2_lMinus1;
      mpz_cdiv_q(t.get_mpz_t(), leftHalf.get_mpz_t(), rightHalf.get_mpz_t());

    }

    //17. outputPrime = 2(t*p2 − y)*p0*p1 + 1.
    outputPrime = ((2 * ((t * p2) - y) * p0 * p1) + 1);
    //18. pGenCounter = pGenCounter + 1.
    pGenCounter++;
    //19. If (GCD(outputPrime–1, e) = 1), then
    if (cmp(gcd(outputPrime - 1, e), 1) == 0)
    {
      //19.1 a = 0
      BigInt a = 0;

      //19.2 For i = 0 to iterations do: a += (Hash(pseed + i))∗ 2 i * outlen.
      for (uint i = 0; i < iterations; i++)
      {
        BigInt exp2_ixOutLen;
        mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * outLen);
        a += hashAlg(primeSeed + i) * exp2_ixOutLen;
      }
      //19.3 primeSeed = primeSeed + iterations + 1.
      primeSeed += iterations + 1;
      //19.4 a = 2 + (a mod (outputPrime–3)).
      a = 2 + (a % (outputPrime - 3));

      //19.5 z = a^(2(t*p2 − y)*p1) mod outputPrime.
      BigInt z;
      BigInt expVal = (2 * ((t * p2) - y) * p1);
      mpz_powm(z.get_mpz_t(), a.get_mpz_t(), expVal.get_mpz_t(),
        outputPrime.get_mpz_t());


      //19.6 If ((1 = GCD(z–1, outputPrime)) and (1 = (z^(p0) mod outputPrime)),
      //then return success
      BigInt expZ_p0ModP;
      mpz_powm(expZ_p0ModP.get_mpz_t(), z.get_mpz_t(), p0.get_mpz_t(),
        outputPrime.get_mpz_t());
      if ((cmp(gcd(z - 1, outputPrime), 1) == 0) && (cmp(expZ_p0ModP, 1) == 0))
      {
        return true;
      }
    }
    //20. If (pgen_counter ≥ 5L), then return failure
    if (cmp(pGenCounter, l * 5) >= 0)
    {
      cout << "Too many iterations generating prime. Greater than 5L." << endl;
      return false;
    }
    //21. t = t + 1
    t++;
    //22. Go to step 16.
  }

  return true;
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
  // cout << "ps working seed: " << workingSeed << endl;
  // cout << "w: " << workingSeed << endl;

  BigInt pSeed = 0;
  BigInt qSeed = 0;


  uint l = nLen / 2;
  uint n1 = 1;
  uint n2 = 1;

  //6. Generate p:
  //6.1 Using l = nlen/2, n1 = 1, n2 = 1, firstSeed = working_seed and e,
  //use the provable prime construction method in Appendix C.10 to obtain p and
  //pSeed. If failure is returned, then return failure

  if (!genPrimeFromAuxiliaries(l, n1, n2, workingSeed, p, pSeed))
  {
    cout << "Generation of the seed for prime p generation failed!" << endl;
    return false;
  }

  cout << "p:  " << p << endl;
  //cout << "ps: " << mpz_sizeinbase(p.get_mpz_t(), 2) << endl;

  //6.2 working_seed = pseed.
  workingSeed = pSeed;
  // cout << "qs working seed: " << workingSeed << endl;

  BigInt absVal = 0;
  BigInt comparisonVal;
  //ComparisonVal = 2^((nLen/2) - 100)
  // bool firstTime = true;
  // BigInt firstQ;
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

    if (!genPrimeFromAuxiliaries(l, n1, n2, workingSeed, q, qSeed))
    {
      cout << "Generation of the seed for prime q generation failed!" << endl;
      return false;
    }
    // if (firstTime)
    // {
    //   firstQ = q;
    //   firstTime = false;
    // }
    // else
    // {
    //   cout << q - firstQ << endl;
    // }
    // cout << workingSeed - qSeed << endl;
    //cout << "qs: " << mpz_sizeinbase(q.get_mpz_t(), 2) << endl;

    //7.2 working_seed = qseed.
    workingSeed = qSeed;
    // cout << "new q working seed: " << endl << workingSeed << endl;

    //8. If ( |p – q| ≤ 2^(nlen/2 – 100)), then go to step 7.
    absVal = abs(p - q);

    // cout << "CurP: " << p << endl;
    // cout << "CurQ: " << q << endl;
    // cout << "abs: " << absVal << endl;
    // cout << "new q attempt: " << endl;
    // cout << hex << absVal - comparisonVal << endl;
  }
  // cout << "q: " << q << endl;
  //9. Zeroize the internally generated seeds: (Not needed)
  //10. Return (SUCCESS, p, q).
  return true;
}

bool RSACipher::genKeys()
{
  const uint binaryBase = 2;
  gmp_randclass randGen(gmp_randinit_mt);
  randGen.seed(clock());
  //Seed used for generating primes
  BigInt seed = randGen.get_z_bits(securityStrength * 2);
  //Generates seed for prime generation
  // if(!genFirstSeed(seed))
  // {
  //   cout << "Generation of the first prime generation seed failed!" << endl;
  //   return false;
  // }

  //Gets randomized e value, must be odd
  BigInt exp2_16 = pow(2, 16);
  BigInt exp2_256;
  mpz_ui_pow_ui(exp2_256.get_mpz_t(), 2, 256);
  e = randGen.get_z_range(exp2_256);
  while ((cmp(e, exp2_16) <=0) || (cmp(e, exp2_256) >= 0) ||
    (e % 2 != 1))
  {
    // cout << "hello" << endl;
    e = randGen.get_z_range(exp2_256);
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

void RSACipher::displayKeyInfo()
{
  const uint outputBase = 16;
  cout << "Public key (e): " << e.get_str(outputBase) << endl;
  cout << "Private key (d): " << d.get_str(outputBase) << endl;
  cout << "p: " << p.get_str(outputBase) << endl;
  cout << "q: " << q.get_str(outputBase) << endl;
}

//TODO max input size 245b (research this)
bool RSACipher::encrypt(string plainText)
{
  return true;
}

//TODO RSA CRT
bool RSACipher::decrypt(string cryptText)
{
  return true;
}


