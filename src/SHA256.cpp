#include <bitset>
#include "SHA256.hpp"
using namespace std;


vector<vector<char*>> SHA256::padParseInput(char* inputBinary, int l)
{
  //Padding
  if (l % blockSize != 0)
  {
    const int finalPaddingBits = 64;
    const int padFinder = 448;
    int lPlus1PadFinder = (l + 1) % blockSize;
    int k = padFinder - lPlus1PadFinder;
    inputBinary += '1';
    for (int i = 0; i < k; i++)
    {
      inputBinary += '0';
    }

    bitset<finalPaddingBits> finalPad(l);
    for (int i = finalPaddingBits - 1; i >=0; i--)
    {
      inputBinary += (char)finalPad[i];
    }
  }

  //Parsing into blocks of words
  vector<vector<char*>> inputBlocks;
  int blockCount = l / blockSize;
  const int bitsInByte = 8;
  const int wordsInBlock = 16 / bitsInByte;
  const int bytesInWord = 32 / bitsInByte;

  for (int i = 0; i < blockCount; i++)
  {
    vector<char*> tempVect;
    for (int j = 0; j < wordsInBlock; j++)
    {
      tempVect.push_back(inputBinary + (i * blockSize) + j * bytesInWord);
    }
    inputBlocks.push_back(tempVect);
  }
  return inputBlocks;
}

void SHA256::hash(char* inputBinary, int l)
{
  vector<vector<char*>> inputBlocks = padParseInput(inputBinary, l);
}
