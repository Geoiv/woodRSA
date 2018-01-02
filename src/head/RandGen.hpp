/*
RandGen.hpp - George Wood
*/

#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include <string>

  typedef unsigned int uint;
class RandGen
{
private:

  uint instantiationNonce = 0;

  const uint binBase = 2;
  const uint hexBase = 16;

  const uint maxSecStrength = 256;
  //TODO what determines this?
  const uint maxReqBits = 5000;
  const uint maxAdditInputLen = 512;
  //TODO check what determines this size
  const uint maxPersStrSize = 512;
  const uint entropyBitLen = 1000;
  const uint seedBitLen = 440;
  //Preimage sec strength of sha 224 is 224
  //Security strengths supported w/ sha 224 are 112, 128, and 192
  const uint sha224OutLen = 224;
  //Preimage sec strength of sha 256 is 256
  //Security strengths supported w/ sha 256 are 112, 128, 192, and 256
  const uint sha256OutLen = 256;
  const uint secStrength0 = 112;
  const uint secStrength1 = 125;
  const uint secStrength2 = 192;
  const uint secStrength3 = 256;
  const uint bitsInHexChar = 4;

  uint shaType;
  uint securityStrength;
  uint stateHandle;
  std::string V;
  std::string C;

  bool getEntropyInput(std::string& entropyInput, uint entropySize);
  bool hashDf(std::string inputHex, uint bitsToReturn, std::string& outputHex);

public:
  RandGen(uint shaOutLen);
  bool instantiate(uint reqSecurityStr, std::string persStr);
  bool genRandom(uint requestedBitCount, std::string additInput,
                 std::string& pseudoRandBits);
};

#endif
