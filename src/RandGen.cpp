/*
RandGen.cpp - George Wood
*/

#include <iostream>
#include <time.h>
#include <math.h>
#include <sstream>
#include "./head/RandGen.hpp"
#include "./head/SHAHash.hpp"
#include "./head/RSABigTypes.hpp"
using namespace std;

/*Gets the base-2 entropy used to instantiate the generator. An unapproved RNG
  is used for this, which is permitted as stated in FIPS 140-2*/
bool RandGen::getEntropyInput(string& entropyInput, uint entropySize,
                              uint iters)
{
  srand(time(NULL));
  BigInt converter;
  BigInt converter2;
  string outputEntropy = "";
  uint temp;
  bool firstIter = true;
  for (uint i = 0; i < iters; i++)
  {
  //Pseudo-randomly selects a 0 or a 1 and appends it to the output string.
    for (uint j = 0; j < entropySize; j++)
    {
      temp = (rand() % 2);
      outputEntropy += to_string(temp);
    }
    converter.set_str(outputEntropy, binBase);
    if (firstIter)
    {
      converter2.set_str(outputEntropy, binBase);
      firstIter = false;
    }
    else
    {
      converter2 = converter^converter2;
    }
  }
  entropyInput = converter2.get_str(hexBase);
  return true;
}

/*HashDF function, as specified in FIPS SP 800-90A*/
bool RandGen::hashDf(string inputHex, uint bitsToReturn, string& outputHex)
{

  SHAHash sha(shaType);
  //Value to hold hashed data until substring is taken
  string temp = "";
  //Iterations of loop
  uint len = ceil(bitsToReturn / shaType);
  uint counter = 1;
  for (uint i = 1; i <= len; i++)
  {
    //Converts integers to hex
    stringstream hexStream;
    hexStream << hex << counter;
    hexStream << hex << bitsToReturn;
    //Concatenates temp w/ output hashed data
    temp += sha.hash(hexStream.str() + inputHex);
    counter++;
  }
  outputHex = temp.substr(0, bitsToReturn / bitsInHexChar);
  return true;
}

/*Constructor with parameter for the SHA output block size to be used*/
RandGen::RandGen(uint shaOutLen)
{
  //Sets SHA type to use
  if (shaOutLen == sha224OutLen)
  {
    shaType = shaOutLen;
  }
  else if (shaOutLen == sha256OutLen)
  {
    shaType = shaOutLen;
  }
  else
  {
    shaType = sha256OutLen;
    cout << "Invalid SHA output length. Set to 256." << endl;
  }
  //Gets randomized instantiation nonce
  instantiationNonce = time(NULL);
}

/*Instantiates the RBG with the requested security strength as stated in
  FIPS SP 800-90A*/
bool RandGen::instantiate()
{
  //1. Handle invalid/too weak security strength;
  //(Also handles step 3)
  if (shaType == 224)
  {
    securityStrength = 192;
  }
  else if (shaType == 256)
  {
    securityStrength = 256;
  }
  else
  {
    cout << "Invalid SHA type or security strength." << endl;
    return false;
  }


  //2. If personalization string length is greater than 512, return failure.
  // No personalization string utilized
  //3. If security strength too low, return failure(Security strength too low)

  //4. entropyInput = getEntropyInput()
  //5. If it fails, return failure
  string entropyInput;
  if (!getEntropyInput(entropyInput, entropyBitLen, securityStrength))
  {
    cout << "Getting entropy input failed." << endl;
    return false;
  }

  //6. instantiationNonce++
  instantiationNonce++;

  //7. seedMaterial = entropyInput + instantiationNonce + persStr
  //  No personalization string utilized
  stringstream hexStream;
  hexStream << hex << entropyInput;
  hexStream << hex << instantiationNonce;
  string seedMaterial = hexStream.str();

  //8. seed = hashDf(seedMaterial, 440)
  //9. V = seed
  if (!hashDf(seedMaterial, seedBitLen, V))
  {
    cout << "HashDF for V failed!" << endl;
    return false;
  }

  //10. C = hashDf((0x00 + V), 440)
  if (!hashDf(("00" + V), seedBitLen, C))
  {
    cout << "HashDF for C failed!" << endl;
    return false;
  }
  //12. stateHandle = findStateSpace()
  //13. If it fails, return false
  //  State handles not needed
  //14. Save internal state (already done)
  return true;
}

/*Generates a random number of a specified number of bits, as stated in
  FIPS SP 800-90A*/
bool RandGen::genRandom(uint requestedBitCount, string& pseudoRandNum)
{
  BigInt twoExp440;
  BigInt temp;
  BigInt temp2;
  mpz_ui_pow_ui(twoExp440.get_mpz_t(), 2, 440);
  SHAHash sha(shaType);
  //1. If invalid state handle, return failuer
  //  State handles not used
  //2. Get internal state vars, already handled with instance vars
  //3. If requestedBitCount > 5000, return failure
  if (requestedBitCount > maxReqBits)
  {
    cout << "Too many bits requested." << endl;
    return false;
  }
  //4. If requestedSecurityStrength > securityStrength, return FAILURE
  //  Handled in instantiation

  //5. If length of additInput > 512, return failure
  //  Additional input not utilized
  //6. If reseedCounter > 100,000, reseed and nullify the additional input
  //   (Reseeding not needed)
  string w;
  //7. If additInput != null, then do
  //  Additional input not utilized in this implementation

  //8. m = ceil(requestedBitCount / outlen)
  uint m = ceil((double)requestedBitCount / shaType);
  //9. data = V
  string data = V;
  //10. W = null string
  string W = "";
  //11. For i = 1 to m
  for (uint i = 1; i <= m; i++)
  {
    w = sha.hash(data);
    W = W + w;
    //data = (data + 1) % (2^440);
    temp.set_str(data, hexBase);
    temp++;
    temp = temp % twoExp440;
    data = temp.get_str(hexBase);
    //data = (data + 1) % (2^440);
  }
  //12.
  pseudoRandNum = W.substr(0, (requestedBitCount / bitsInHexChar));
  //13.
  string H = sha.hash("03" + V);
  //14. V = (V + H + C + reseedCounter) % (2^440)
  temp.set_str(V, hexBase);
  temp2.set_str(H, hexBase);
  temp += temp2;
  temp2.set_str(C, hexBase);
  temp += temp2;
  temp = temp % twoExp440;
  V = temp.get_str(hexBase);
  //15. reseedCounter++ (Reseeding not needed)
  //16. Update internal state vals, handled by instance vars
  //17. Return success
  return true;
}
