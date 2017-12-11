#include <bitset>
#include <iostream>
#include <sstream>
#include <math.h>
#include "./head/SHAHash.hpp"
using namespace std;

SHAHash::SHAHash(uint reqBlockSize)
{
  if (reqBlockSize == blockChoice224)
  {
    h0 = 0xc1059ed8;
    h1 = 0x367cd507;
    h2 = 0x3070dd17;
    h3 = 0xf70e5939;
    h4 = 0xffc00b31;
    h5 = 0x68581511;
    h6 = 0x64f98fa7;
    h7 = 0xbefa4fa4;
  }
  else if (reqBlockSize == blockChoice256)
  {
    h0 = 0x6a09e667;
    h1 = 0xbb67ae85;
    h2 = 0x3c6ef372;
    h3 = 0xa54ff53a;
    h4 = 0x510e527f;
    h5 = 0x9b05688c;
    h6 = 0x1f83d9ab;
    h7 = 0x5be0cd19;
  }

  else
  {
    cout << "Input block size for SHA hash was invalid." << endl;
  }

}

vector<uint> SHAHash::padParseInput(string& inputHex)
{
  //Padding
  const uint bitsInHexChar = 4;
  const uint hexBase = 16;
  const uint finalPaddingBits = 64;
  const uint finalPaddingChars = finalPaddingBits / bitsInHexChar;
  const uint padFinder0 = 448 / bitsInHexChar;
  const uint hexBlockSize = blockSize / bitsInHexChar;

  uint l = inputHex.length();
  string first1 = "80";
  inputHex += "80";

  uint padFinder1 = (l + first1.size()) % hexBlockSize;
  uint k = padFinder0 - padFinder1;

  for (uint i = 0; i < k; i++)
  {
    inputHex += "0";
  }

  stringstream hexStream;
  hexStream << hex << l * bitsInHexChar;
  string finalPadString = hexStream.str();
  for (uint i = 0; i < finalPaddingChars - finalPadString.size(); i++)
  {
    inputHex += "0";
  }
  inputHex += finalPadString;

  //Parsing uinto blocks of words
  vector<uint> inputWords;
  uint wordCount = inputHex.length() / hexCharsInWord;
  for (uint i = 0; i < wordCount; i++)
  {
    uint startingInd = (i * hexCharsInWord);
    string tempHexString = inputHex.substr(startingInd, hexCharsInWord);
    inputWords.push_back((uint)stoul(tempHexString, nullptr, hexBase));
  }
  return inputWords;
}

uint SHAHash::rotr(uint x, uint n)
{
  return ((x >> n) | (x << (bitsInWord - n)));
}

uint SHAHash::bSigmaSub0(uint x)
{
  const uint rotVal0 = 2;
  const uint rotVal1 = 13;
  const uint rotVal2 = 22;
  uint rotrX0 = rotr(x, rotVal0);
  uint rotrX1 = rotr(x, rotVal1);
  uint rotrX2 = rotr(x, rotVal2);
  uint outputX = rotrX0^rotrX1^rotrX2;
  return outputX;
}
uint SHAHash::bSigmaSub1(uint x)
{
  const uint rotVal0 = 6;
  const uint rotVal1 = 11;
  const uint rotVal2 = 25;
  uint rotrX0 = rotr(x, rotVal0);
  uint rotrX1 = rotr(x, rotVal1);
  uint rotrX2 = rotr(x, rotVal2);
  uint outputX = rotrX0^rotrX1^rotrX2;
  return outputX;
}
uint SHAHash::sSigmaSub0(uint x)
{
  const uint rotVal0 = 7;
  const uint rotVal1 = 18;
  const uint shiftVal = 3;
  uint rotrX0 = rotr(x, rotVal0);
  uint rotrX1 = rotr(x, rotVal1);
  uint shiftX = x >> shiftVal;
  uint outputX = rotrX0^rotrX1^shiftX;
  return outputX;
}

uint SHAHash::sSigmaSub1(uint x)
{
  const uint rotVal0 = 17;
  const uint rotVal1 = 19;
  const uint shiftVal = 10;
  uint rotrX0 = rotr(x, rotVal0);
  uint rotrX1 = rotr(x, rotVal1);
  uint shiftX = x >> shiftVal;
  uint outputX = rotrX0^rotrX1^shiftX;
  return outputX;
}

uint SHAHash::chFunc(uint x, uint y, uint z)
{
  return (x & y)^(~x & z);
}
uint SHAHash::majFunc(uint x, uint y, uint z)
{
  return (x & y)^(x & z)^(y & z);
}

string SHAHash::hash(string inputHex)
{
  const uint hexCharsInWord = 8;
  const uint messageSchedFirst = 16;
  const uint messageSchedSecond = 64;
  const uint roundCount = 64;
  const uint workingVarCount = 8;
  const uint twoExp32 = pow(2, 32);

  uint hashVals[] = {h0, h1, h2, h3, h4, h5, h6, h7};
  vector<uint> inputWords = padParseInput(inputHex);

  uint a;
  uint b;
  uint c;
  uint d;
  uint e;
  uint f;
  uint g;
  uint h;

  uint blockCount = inputWords.size() / wordsInBlock;
  for (uint i = 0; i < blockCount; i++)
  {
    a = hashVals[0];
    b = hashVals[1];
    c = hashVals[2];
    d = hashVals[3];
    e = hashVals[4];
    f = hashVals[5];
    g = hashVals[6];
    h = hashVals[7];

    vector<uint> W;

    for (uint j = 0; j < messageSchedFirst; j++)
    {
      W.push_back(inputWords.at((i * wordsInBlock) + j));
    }
    for (uint j = messageSchedFirst; j < messageSchedSecond; j++)
    {
      uint sigma1WJMinus2 = sSigmaSub1(W.at(j - 2));
      uint wJMinus7 = W.at(j - 7);
      uint sigma0WJMinus15 = sSigmaSub0(W.at(j - 15));
      uint wJMinus16 = W.at(j - 16);
      uint newW = (sigma1WJMinus2 + wJMinus7 +
        sigma0WJMinus15 + wJMinus16) % twoExp32;
      W.push_back(newW);
    }

    for (uint j = 0; j < roundCount; j++)
    {
      uint t1 = h + bSigmaSub1(e) + chFunc(e, f, g) + K.at(j) + W.at(j);
      uint t2 = bSigmaSub0(a) + majFunc(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }

    hashVals[0] = (a + hashVals[0]) % twoExp32;
    hashVals[1] = (b + hashVals[1]) % twoExp32;
    hashVals[2] = (c + hashVals[2]) % twoExp32;
    hashVals[3] = (d + hashVals[3]) % twoExp32;
    hashVals[4] = (e + hashVals[4]) % twoExp32;
    hashVals[5] = (f + hashVals[5]) % twoExp32;
    hashVals[6] = (g + hashVals[6]) % twoExp32;
    hashVals[7] = (h + hashVals[7]) % twoExp32;
  }

  string hashHex = "";

  for (uint i = 0; i < workingVarCount - 1; i++)
  {
    stringstream hexStream;
    hexStream << hex << hashVals[i];
    string tempHexString = hexStream.str();
    for (uint j = 0; j < hexCharsInWord - tempHexString.size(); j++)
    {
      tempHexString = '0' + tempHexString;
    }
    hashHex += tempHexString;
  }
  return hashHex;
}
