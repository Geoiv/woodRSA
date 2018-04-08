/*
SHA256.hpp - George Wood
*/

#ifndef __SHAHASH_HPP__
#define __SHAHASH_HPP__

#include <vector>
#include <string>
#include "RSAConsts.hpp"

class SHAHash
{
private:


  //Hash values used in hashing algorithm (Values depend on SHA type)
  uint h0;
  uint h1;
  uint h2;
  uint h3;
  uint h4;
  uint h5;
  uint h6;
  uint h7;
  //Number of working variables used in output hash value (depends on SHA type)
  uint usedWorkingVarCount;
  //Size of the output block (Depends on SHA type)
  uint outputBlockSize;

  //K values used in hashing algorithm (Independent of SHA type)
  const std::vector<uint> K = {
                            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                            0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                            0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                            0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                            0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                            0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

  //Pads and parses the hex value input to the hasher
  std::vector<uint> padParseInput(std::string& inputHex);
  //Right circular rotation of n bits in input value x
  uint rotr(uint x, uint n);
  //Intermediate functions for hashing
  uint bSigmaSub0(uint x);
  uint bSigmaSub1(uint x);
  uint sSigmaSub0(uint x);
  uint sSigmaSub1(uint x);
  uint chFunc(uint x, uint y, uint z);
  uint majFunc(uint x, uint y, uint z);

public:
  //Number of bits in each word
  static const uint bitsInWord = 32;
  //Number of words in each block
  static const uint wordsInBlock = 16;
  //Number of hex chars in each word
  static const uint hexCharsInWord = 8;
  //Constructor with parameter for requested output block size in bits
  SHAHash(uint reqBlockSize);
  //Gets the output block size of the hasher
  uint getBlockSize();
  //Hashes the input hex value
  std::string hash(std::string inputHex);
};

#endif
