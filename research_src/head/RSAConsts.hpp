/*
RSAConsts.hpp - George Wood
*/

#ifndef __RSACONSTS_HPP__
#define __RSACONSTS_HPP__

#include <string>

  typedef unsigned int uint;

//Constants for conversion between binary, decimal, hex, and ASCII codes
const uint asciiBase = 0;
const uint binBase = 2;
const uint decBase = 10;
const uint hexBase = 16;
const uint bitsInHexChar = 4;

//Constant identifiers for types of SHA that can be used
const uint sha224OutLen = 224;
const uint sha256OutLen = 256;

//Files for key/data storage
const std::string keyFileName = "RSAKeys.txt";
const std::string dataFileName = "RSAData.txt";

#endif
