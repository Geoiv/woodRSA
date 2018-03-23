/*
RandGen.hpp - George Wood
*/

#ifndef __RANDGEN_HPP__
#define __RANDGEN_HPP__

#include <string>
#include "RSAConsts.hpp"

class RandGen
{
private:
  //Maximum security strength this generator can provide
  const uint maxSecStrength = 256;

  //Preimage sec strength of sha 224 is 224
  //Security strengths supported w/ sha 224 are 112, 128, and 192
  //Preimage sec strength of sha 256 is 256
  //Security strengths supported w/ sha 256 are 112, 128, 192, and 256
  //Available security strengths for this generator
  const uint secStrength0 = 112;
  const uint secStrength1 = 125;
  const uint secStrength2 = 192;
  const uint secStrength3 = 256;

  //Must be less than stated in FIPS 800 90A, section 10
  const uint maxReqBits = 5000;
  //Must be less than stated in FIPS 800 90A, section 10
  const uint entropyBitLen = 1000;
  //Determined by type of SHA used, for 224 or 256 it will always be 440
  const uint seedBitLen = 440;

  //Type of SHA algorithm used by this generator
  uint shaType;
  //Security strength of this generator
  uint securityStrength;
  //Nonce used as part of the seed
  uint instantiationNonce;
  //Vars for holding seed generation values
  std::string V;
  std::string C;

  //Gets the entropy used to instantiate the generator
  bool getEntropyInput(std::string& entropyInput, uint entropySize, uint iters);
  //Hash function used for instantiation of the generator
  bool hashDf(std::string inputHex, uint bitsToReturn, std::string& outputHex);

public:
  //Constructor with parameter for requested SHA algorithm type
  RandGen(uint shaOutLen);
  //Instantiates the generator
  bool instantiate();
  //Generates a random number with this generator
  bool genRandom(uint requestedBitCount, std::string& pseudoRandBits);
};

#endif
