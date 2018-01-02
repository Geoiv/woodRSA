/*
RSAConsts.hpp - George Wood
*/

#ifndef __RSACONSTS_HPP__
#define __RSACONSTS_HPP__

  typedef unsigned int uint;
  //TODO is this really needed?
  typedef unsigned long long ullong;

//Constants for conversion between binary, decimal, hex, and ASCII codes
const uint asciiBase = 0;
const uint binBase = 2;
const uint decBase = 10;
const uint hexBase = 16;
const uint bitsInHexChar = 4;

//Constant identifiers for types of SHA that can be used
const uint sha224OutLen = 224;
const uint sha256OutLen = 256;
#endif
