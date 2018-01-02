/*
RSACipher.cpp - George Wood - RSA 2048 and SHA 224
*/

#include <random>
#include <time.h>
#include <math.h>
#include <climits>
#include <iostream>
#include "./head/RSACipher.hpp"
#include "./head/SHAHash.hpp"
#include "./head/RandGen.hpp"
using namespace std;

/*Default constructor for the RSA cipher - defaults to RSA-2048
  Sets nLen and securityStrength accordingly.*/
RSACipher::RSACipher()
{
  nLen = nLen1;
  securityStrength = secStrengthPairs.at(nLen);
  shaOutLen = shaBlockPairs.at(nLen);
}

/*Constructor for specified nLen*/
RSACipher::RSACipher(uint inputNLen)
{
  //Sets nLen and securityStrength according to the requested nLen
  if (inputNLen == nLen0 || inputNLen == nLen1 || inputNLen == nLen2)
  {
    nLen = inputNLen;
    securityStrength = secStrengthPairs.at(inputNLen);
    shaOutLen = shaBlockPairs.at(inputNLen);
  }
  //Invalid nLen request
  else
  {
    cout << "Invalid nLen input." << endl;
  }
}

/*Hashes the input integer*/
BigInt RSACipher::hashAlg(const BigInt inputX)
{
  //SHA hasher
  SHAHash sha(shaOutLen);
  //String for input hex
  string inputHex = inputX.get_str(hexBase);
  //Gets hash value
  string hashedHex = sha.hash(inputHex);
  //Loads hash value into a BigInt and returns it
  BigInt hashedInt;
  hashedInt.set_str(hashedHex, hexBase);
  return hashedInt;
}

/*Utilizes a sieve procedure to find primes below input limit value*/
vector<bool> RSACipher::sieveProcedure(uint limitVal)
{
  //Smallest prime number is 2
  const uint minPrime = 2;
  //Vector of all values less than the limitVal (index of each bool is the #)
  vector<bool> primes(limitVal, true);
  //0 and 1 are trivially nonprime
  primes.at(0) = false;
  primes.at(1) = false;
  //Primality checks each val in the vector, marks those that are not as false
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
  //Returns vector of marked prime indicies
  return primes;
}

/*Tests input number c for primality*/
bool RSACipher::primalityTest(BigInt c)
{
  //1. Prepare a table of primes < sqrt(c) using the sieve procedure in (C.8)
  //vector<BigInt> smallerPrimes = sieveProcedure(sqrt(c));
  BigInt testPrime = 2;
  BigInt rootC = sqrt(c);
  vector<bool> primes = sieveProcedure(rootC.get_ui());

  //2. Divide potential prime by all values in table.
  //If divisible, return failure
  for (uint i = 0; i < primes.size(); i++)
  {
    if (primes.at(i))
    {
      uint testPrime = i;
      if ((mpz_divisible_ui_p(c.get_mpz_t(), testPrime)) != 0)
      {
        return false;
      }
    }
  }
  //Return success
  return true;
}

/*Uses the Shawe-Taylor random prime routine specified in FIPS 186-4
  Appendix C.6 to generate a random prime of a specified bit length*/
bool RSACipher::randomPrime(const uint length, const BigInt inputSeed,
  BigInt& outputPrime, BigInt& primeSeed, BigInt& pGenCounter)
{
  BigInt c;

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
    //4. pGenCounter = 0;
    pGenCounter = 0;

    //Loop to return to step 5
    bool loopStep5 = true;
    while (loopStep5)
    {
      //5. c = hashAlg(primeSeed)^hashAlg(primeSeed + 1)
      c = hashAlg(primeSeed)^hashAlg(primeSeed + 1);
      //6. c = 2^length-1 + (c mod 2^length-1)
      BigInt cMod = (c % exp2_lMinus1);
      if (cMod < 0)
      {
        cMod += exp2_lMinus1;
      }
      c = exp2_lMinus1 + cMod;
      //7. c = (2 * floor(c / 2)) + 1
      BigFloat cFloat(c);
      c = ((2 * floor(cFloat / 2)) + 1);
      //8.pGenCounter++
      pGenCounter++;
      //9.primeSeed += 2
      primeSeed += 2;
      //10. Perform a deterministic primality test on c. For example,
      //since c is small, its primality can be tested by trial division.
      //Appendix C.7.
      //11. if c is prime,
      if (primalityTest(c))
      {
        //11.1&2 outputPrime = c, return success
        outputPrime = c;
        return true;
      }
      //12. if pGenCounter > 4*length, return failure
      if (pGenCounter > (4 * length))
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
  //16. iterations = ceil(length / outlen) - 1;
  uint iterations = ceil((float)length / shaOutLen) - 1;
  //17. oldCounter = pGenCounter
  BigInt oldCounter = pGenCounter;
  //18. x = 0
  BigInt x = 0;

  //19. For i = 0 to iterations, do x += hashAlg(primeSeed + i) * 2^i*outlen
  for (uint i = 0; i <= iterations; i++)
  {
    BigInt exp2_ixOutLen;
    mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * shaOutLen);
    x += hashAlg(primeSeed + i) * exp2_ixOutLen;
  }
  //20. primeSeed += iterations + 1
  primeSeed += iterations + 1;
  //21. x = 2^length-1 + x mod 2^(length-1)
  BigInt xMod = (x % exp2_lMinus1);
  if (xMod < 0)
  {
    xMod += exp2_lMinus1;
  }
  x = exp2_lMinus1 + xMod;

  //22.t = ceil(x / (2 * c0))
  BigInt twoC0 = 2 * c0;
  BigInt t;
  mpz_cdiv_q(t.get_mpz_t(), x.get_mpz_t(), twoC0.get_mpz_t());

  while (true)
  {
    //23. if (2 * t * c0) + 1 > 2^length) then t = ceil(2^length-1 / (2 * c0))
    if (((2 * t * c0) + 1) > exp2_l)
    {
      mpz_cdiv_q(t.get_mpz_t(), exp2_lMinus1.get_mpz_t(), twoC0.get_mpz_t());
    }
    //24. c = 2tc0 + 1
    c = (2 * t * c0) + 1;
    //25. pGenCounter++
    pGenCounter++;
    //26. a = 0
    BigInt a = 0;

    //27. For i = 0 to iterations, do:
    for (uint i = 0; i <= iterations; i++)
    {
      BigInt exp2_ixOutLen;
      mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * shaOutLen);
      a += hashAlg(primeSeed + i) * exp2_ixOutLen;
    }
    //28. primeSeed += iterations + 1
    primeSeed += iterations + 1;
    //29. a = 2 + a mod (c - 3)
    BigInt aMod = (a % (c - 3));
    if (aMod < 0)
    {
      aMod += (c - 3);
    }
    a = 2 + aMod;

    //30. z = a^2t mod c
    BigInt z;
    BigInt twoT = (2 * t);
    mpz_powm(z.get_mpz_t(), a.get_mpz_t(), twoT.get_mpz_t(), c.get_mpz_t());

    //31. if (gcd(z-1, c) == 1) and (z^c0 mod c == 1) then
    BigInt expZ_c0ModC;
    mpz_powm(expZ_c0ModC.get_mpz_t(), z.get_mpz_t(), c0.get_mpz_t(),
      c.get_mpz_t());
    if ((gcd(z - 1, c) == 1) && (expZ_c0ModC == 1))
    {
      //31.1 prime = c
      outputPrime = c;
      //31.2 return success
      return true;
    }
    //32. if pGenCounter >= (4 * length + oldCounter) return failure
    if (pGenCounter >= ((4 * length) + oldCounter))
    {
      return false;
    }
    //33. t++
    t++;
    //34. Go to step 23
  }
}

/*Generates a prime based on auxiliary primes using the method specified in
  FIPS 186-4 Appendix C.10*/
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

  //1. If l, n1, or n2 are not valid, return failure (handled in calling func)
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
  //6. Using ceil(l / 2) + 1 as the length and p0Seed as the inputSeed,
  //use the random prime generation routine in Appendix C.6 to obtain
  //p0 and primeSeed. If failure is returned, then return failure
  BigInt temp = 0;
  if (!randomPrime(ceil((float)l/2) + 1, p0Seed, p0, primeSeed, temp))
  {
    return false;
  }
  //7. iterations = ceil(l / outLen) − 1, where outLen is the length of the
  //hash function output block
  uint iterations = ceil((float)l / shaOutLen) - 1;
  //8. pGenCounter = 0
  BigInt pGenCounter = 0;
  //9. x = 0
  BigInt x = 0;
  //10. For i = 0 to iterations: x = x + (hashAlg(primeSeed + i)) ∗ 2^(i * outLen)
  for (uint i = 0; i <= iterations; i++)
  {
    BigInt exp2_ixOutLen;
    mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * shaOutLen);
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
  BigInt xMod = (x % (exp2_l - floorRoot2xExp2_lMinus1));
  if (xMod < 0)
  {
    xMod += (exp2_l - floorRoot2xExp2_lMinus1);
  }
  BigInt rightHalf = xMod;

  //Final assignment
  x = leftHalf + rightHalf;

  //13. If (gcd(p0*p1, p2) ≠ 1), then return failure
  if (gcd(p0 * p1, p2) != 1)
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

  //15. t = ceil(((2*y*p0*p1) + x)/(2*p0*p1*p2)).
  BigInt twoxYxP0xP1 = (2 * y * p0 * p1);
  BigInt twoxP0xP1xP2 = (2 * p0 * p1 * p2);
  leftHalf = twoxYxP0xP1 + x;
  rightHalf = twoxP0xP1xP2;
  BigInt t;
  mpz_cdiv_q(t.get_mpz_t(), leftHalf.get_mpz_t(), rightHalf.get_mpz_t());
  //Loop to return to step 16
  while (true)
  {
    //16. If ((2(t*p2 − y)*p0*p1 + 1) > 2^l)
    BigInt compVal = ((2 * ((t * p2) - y) * p0 * p1) + 1);
    if (compVal > exp2_l)
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
    if (gcd(outputPrime - 1, e) == 1)
    {
      //19.1 a = 0
      BigInt a = 0;

      //19.2 For i = 0 to iterations do: a += (Hash(pseed + i))∗ 2 i * outlen.
      for (uint i = 0; i <= iterations; i++)
      {
        BigInt exp2_ixOutLen;
        mpz_ui_pow_ui(exp2_ixOutLen.get_mpz_t(), 2, i * shaOutLen);
        a += hashAlg(primeSeed + i) * exp2_ixOutLen;
      }
      //19.3 primeSeed = primeSeed + iterations + 1.
      primeSeed += iterations + 1;
      // incCount += iterations + 1;
      //19.4 a = 2 + (a mod (outputPrime–3)).
      BigInt aMod = (a % (outputPrime - 3));
      if (aMod < 0)
      {
        aMod += (outputPrime - 3);
      }
      a = 2 + aMod;

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
      if ((gcd(z - 1, outputPrime) == 1) && (expZ_p0ModP == 1))
      {
        cout << "Tests: " << endl;
        BigInt outputPrimePlus1 = outputPrime + 1;
        BigInt outputPrimeMinus1 = outputPrime - 1;
        cout << "  Prime - 1 divisible by p0: " <<
          (mpz_divisible_p(outputPrimeMinus1.get_mpz_t(), p0.get_mpz_t()) ?
          "Passed!" : "Failed!") << endl;
        cout << "  Prime - 1 divisible by p1: " <<
          (mpz_divisible_p(outputPrimeMinus1.get_mpz_t(), p1.get_mpz_t()) ?
          "Passed!" : "Failed!") << endl;
        cout << "  Prime + 1 divisible by p2: " <<
          (mpz_divisible_p(outputPrimePlus1.get_mpz_t(), p2.get_mpz_t()) ?
          "Passed!" : "Failed!") << endl;
        return true;
      }
    }
    //20. If (pgen_counter ≥ 5L), then return failure
    if (pGenCounter >= (l * 5))
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

/*Generates the p & q, the seeds needed for RSA key generation, using the
  method given in FIPS 186-4 Section B.3.4.
  seedLen must be larger than N.*/
bool RSACipher::genPrimes(const BigInt seed)
{
  //Bitlength for auxiliary primes (p1, p2, q1, and q2)
  const uint bitLength = 198;
  //1. If nLen isn't 2048 nor 3072, then return (FAILURE, 0, 0)
  //     (handled in constructor)
  if (nLen != nLen1 && nLen != nLen2)
  {
    return false;
  }
  //2. If (e ≤ 2^16 OR e ≥ 2^256 OR e is not odd), then return failure
  //     (handled in calling function)
  //3. Set securityStrength to appropriate val
  //     (handled in constructor)
  //4. If (len(seed) != 2*securityStrength) return failure
  //     (handled in constructor)

  //5. working_seed = seed.
  BigInt workingSeed = seed;

  BigInt pSeed = 0;
  BigInt qSeed = 0;

  //6. Generate p:
  //6.1 Using l = nlen/2, n1 = bitLength, n2 = bitLength,
  //firstSeed = working_seed and e, use the provable prime construction method
  //in Appendix C.10 to obtain p and pSeed. If failure is returned,
  //then return failure
  uint l = nLen / 2;
  uint n1 = bitLength;
  uint n2 = bitLength;
  cout << "Generating p." << endl;
  if (!genPrimeFromAuxiliaries(l, n1, n2, workingSeed, p, pSeed))
  {
    cout << "Generation of the seed for prime p generation failed!" << endl;
    return false;
  }

  //6.2 working_seed = pseed.
  workingSeed = pSeed;

  BigInt absVal = 0;
  BigInt comparisonVal;
  //ComparisonVal = 2^((nLen/2) - 100)
  cout << "Generating q." << endl;
  mpz_ui_pow_ui(comparisonVal.get_mpz_t(), 2, ((nLen / 2) - 100));
  //Loop to return to step 7 if needed
  while(absVal <= comparisonVal)
  {
    //7. Generate q:
    //7.1 Using L = nlen/2, n1 = bitLength, n2 = bitLength,
    //first_seed = working_seed and e,use the provable prime construction method
    //in Appendix C.10 to obtain q and qseed. If FAILURE is returned,
    //then return (FAILURE, 0, 0).
    l = nLen / 2;
    n1 = bitLength;
    n2 = bitLength;

    if (!genPrimeFromAuxiliaries(l, n1, n2, workingSeed, q, qSeed))
    {
      cout << "Generation of the seed for prime q generation failed!" << endl;
      return false;
    }

    //7.2 working_seed = qseed.
    workingSeed = qSeed;

    //8. If ( |p – q| ≤ 2^(nlen/2 – 100)), then go to step 7.
    absVal = abs(p - q);

  }
  cout << "Key generation complete!" << endl;
  //9. Zeroize the internally generated seeds: (Not needed)
  //10. Return (SUCCESS, p, q).
  return true;
}

/*Generates keys e and d, as well as n, for the RSA system*/
bool RSACipher::genKeys()
{
  //Section B.3.2.1
  //1. If nLen is not valid (see Section 5.1), then return (FAILURE, 0).
  //     (handled in constructor)
  //2. Let security_strength be the security strength associated with nlen,
  //     as specified in SP 800-57, Part 1. (handled in constructor)

  //3. Obtain a string seed of (2 * securityStrength) bits from an RBG
  //     that supports the securityStrength. (TODO approved RBG)
  //Random generator
  gmp_randclass randGen(gmp_randinit_mt);
  randGen.seed(clock());
  //Seed used for generating primes
  BigInt seed = randGen.get_z_bits(securityStrength * 2);
  //4. Return (SUCCESS, seed). (not needed, not a separate function)

  //Gets randomized e value, must be odd
  BigInt exp2_16 = pow(2, 16);
  BigInt exp2_256;
  mpz_ui_pow_ui(exp2_256.get_mpz_t(), 2, 256);
  e = randGen.get_z_range(exp2_256);
  //If the value is negative (check for modulus irregularities in GMP library)
  if ((e % 2) < 0)
  {
    e += 2;
  }
  //If e is too small, too large, or is not odd
  while ((e <= exp2_16) || (e >= exp2_256) || ((e % 2) != 1))
  {
    e = randGen.get_z_range(exp2_256);
    if ((e % 2) < 0)
    {
      e += 2;
    }
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
  if (!mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phiN.get_mpz_t()))
  {
    cout << "Error getting d from e and phi(n)!" << endl;
    return false;
  }
  //Returns success
  return true;
}

/*Displays key info for this RSA system instance*/
void RSACipher::displayKeyInfo()
{
  cout << "Public key (e): " << e.get_str(hexBase) << endl;
  cout << endl << "Private key (d): " << d.get_str(hexBase) << endl;
  cout << endl << "n: " << n.get_str(hexBase) << endl;
  cout << endl << "p: " << p.get_str(hexBase) << endl;
  cout << endl << "q: " << q.get_str(hexBase) << endl << endl;
}

/*Mask generation function for OAEP mechanisms*/
bool RSACipher::maskGenFunc(BigInt seed, uint maskLen, BigInt& mask)
{
  if (maskLen > (pow(2, 32) * shaOutLen))
  {
    cout << "Requested mask length too long." << endl;
  }
  string t = "";
  const uint cLen = 4 * 8;
  for (uint counter = 0; counter <= (ceil(maskLen / shaOutLen) - 1); counter++)
  {
    BigInt temp = counter;
    string c = temp.get_str(binBase);
    for (uint i = 0; i < cLen - c.size(); i++)
    {
      c = "0" + c;
    }
    temp.set_str(temp.get_str(binBase) + c, binBase);
    t += hashAlg(temp).get_str(binBase);
  }
  BigInt tInt;
  tInt.set_str(t, binBase);
  return true;
}

/*Standard RSA encryption*/
bool RSACipher::encrypt(string plainTextString, string& cipherTextString)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt pt;
  pt.set_str(plainTextString, hexBase);

  BigInt ct;
  //ct = pt^e % n
  mpz_powm(ct.get_mpz_t(), pt.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  cipherTextString = ct.get_str(hexBase);
  if (cipherTextString.size() % 2 != 0)
  {
    cipherTextString = "0" + cipherTextString;
  }
  return true;
}

/*RSAES-OAEP encryption as specified in PKCS #1 version 2.2
  Labels are not utilized, as they are not needed in this context.
  TODO finish pkcs 2.2*/
bool RSACipher::encryptOAEP(string plainTextString, string& cipherTextString)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt pt;
  pt.set_str(plainTextString, hexBase);



  BigInt ct;

  mpz_powm(ct.get_mpz_t(), pt.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  cipherTextString = ct.get_str(hexBase);
  if (cipherTextString.size() % 2 != 0)
  {
    cipherTextString = "0" + cipherTextString;
  }
  return true;
}

/*Standard RSA decryption with options for CRT*/
bool RSACipher::decrypt(string cipherTextString, string& plainTextString,
                        bool crtFlag)
{

  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt ct;
  ct.set_str(cipherTextString, hexBase);

  BigInt pt;

  //If CRT decryption is to be used
  if (crtFlag)
  {
    BigInt dP = d % (p - 1);
    if (dP < 0)
    {
      dP += (p - 1);
    }
    BigInt dQ = d % (q - 1);
    if (dP < 0)
    {
      dQ += (q - 1);
    }
    BigInt qInv;
    if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
    {
      cout << "Error getting qInv!" << endl;
      return false;
    }
    BigInt m1;
    BigInt m2;

    mpz_powm(m1.get_mpz_t(), ct.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
    mpz_powm(m2.get_mpz_t(), ct.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());

    BigInt h = (qInv * (m1 - m2)) % p;
    if (h < 0)
    {
      h += p;
    }
    //pt = m2 + (h * q)
    pt = m2 + (h * q);
  }
  //CRT is not to be used
  else
  {
    //pt = ct^d % n
    mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  }

  plainTextString = pt.get_str(hexBase);
  if (plainTextString.size() % 2 != 0)
  {
    plainTextString = "0" + plainTextString;
  }

  return true;
}

/*RSAES-OAEP decryption as specified in PKCS #1 version 2.2 with options for CRT
  Labels are not utilized, as they are not needed in this context.
  TODO finish pkcs 2.2*/
bool RSACipher::decryptOAEP(string cipherTextString, string& plainTextString,
                            bool crtFlag)
{

  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt ct;
  ct.set_str(cipherTextString, hexBase);

  BigInt pt;

  if (crtFlag)
  {
    BigInt dP = d % (p - 1);
    if (dP < 0)
    {
      dP += (p - 1);
    }
    BigInt dQ = d % (q - 1);
    if (dP < 0)
    {
      dQ += (q - 1);
    }
    BigInt qInv;
    if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
    {
      cout << "Error getting qInv!" << endl;
      return false;
    }
    BigInt m1;
    BigInt m2;

    mpz_powm(m1.get_mpz_t(), ct.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
    mpz_powm(m2.get_mpz_t(), ct.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());

    BigInt h = (qInv * (m1 - m2)) % p;
    if (h < 0)
    {
      h += p;
    }

    pt = m2 + (h * q);
  }
  else
  {
    mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  }

  plainTextString = pt.get_str(hexBase);
  if (plainTextString.size() % 2 != 0)
  {
    plainTextString = "0" + plainTextString;
  }

  return true;
}

/*Standard RSA signing with options for CRT*/
bool RSACipher::sign(string plainTextString, string& cipherTextString,
                     bool crtFlag)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt pt;
  pt.set_str(plainTextString, hexBase);

  if (mpz_sizeinbase(pt.get_mpz_t(), binBase) > nLen)
  {
    cout << "Input too large!" << endl;
  }

  BigInt dP = d % (p - 1);
  if (dP < 0)
  {
    dP += (p - 1);
  }
  BigInt dQ = d % (q - 1);
  if (dP < 0)
  {
    dQ += (q - 1);
  }
  BigInt qInv;
  if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
  {
    cout << "Error getting qInv!" << endl;
    return false;
  }
  BigInt m1;
  BigInt m2;
  mpz_powm(m1.get_mpz_t(), pt.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
  mpz_powm(m2.get_mpz_t(), pt.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());

  BigInt h = (qInv * (m1 - m2)) % p;
  if (h < 0)
  {
    h += p;
  }
  BigInt ct = m2 + (h * q);
  cipherTextString = ct.get_str(hexBase);
  if (cipherTextString.size() % 2 != 0)
  {
    cipherTextString = "0" + cipherTextString;
  }
  return true;
}
/*RSASSA-PSS signing as specified in PKCS #1 version 2.2 with options for CRT
  Labels are not utilized, as they are not needed in this context.
  TODO finish pkcs 2.2*/
bool RSACipher::signPSS(string plainTextString, string& cipherTextString,
                     bool crtFlag)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt pt;
  pt.set_str(plainTextString, hexBase);

  if (mpz_sizeinbase(pt.get_mpz_t(), binBase) > nLen)
  {
    cout << "Input too large!" << endl;
  }

  BigInt dP = d % (p - 1);
  if (dP < 0)
  {
    dP += (p - 1);
  }
  BigInt dQ = d % (q - 1);
  if (dP < 0)
  {
    dQ += (q - 1);
  }
  BigInt qInv;
  if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
  {
    cout << "Error getting qInv!" << endl;
    return false;
  }
  BigInt m1;
  BigInt m2;
  mpz_powm(m1.get_mpz_t(), pt.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
  mpz_powm(m2.get_mpz_t(), pt.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());

  BigInt h = (qInv * (m1 - m2)) % p;
  if (h < 0)
  {
    h += p;
  }
  BigInt ct = m2 + (h * q);
  cipherTextString = ct.get_str(hexBase);
  if (cipherTextString.size() % 2 != 0)
  {
    cipherTextString = "0" + cipherTextString;
  }
  return true;
}
bool RSACipher::auth(string cipherTextString, string& plainTextString)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt ct;
  ct.set_str(cipherTextString, hexBase);

  BigInt pt;
  mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  plainTextString = pt.get_str(hexBase);
  if (plainTextString.size() % 2 != 0)
  {
    plainTextString = "0" + plainTextString;
  }
  return true;
}
/*RSASSA-PSS authentication as specified in PKCS #1 version 2.2 with options
  for CRT.
  Labels are not utilized, as they are not needed in this context.
  TODO finish pkcs 2.2*/
bool RSACipher::authPSS(string cipherTextString, string& plainTextString)
{
  if (e == 0 || d == 0 || n == 0)
  {
    cout << "Key data not available! Please visit the key option menu to "
      "generate RSA keys." << endl;
    return false;
  }

  BigInt ct;
  ct.set_str(cipherTextString, hexBase);

  BigInt pt;
  mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  plainTextString = pt.get_str(hexBase);
  if (plainTextString.size() % 2 != 0)
  {
    plainTextString = "0" + plainTextString;
  }
  return true;
}
