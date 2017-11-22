#include <bitset>
#include "SHA256.hpp"
using namespace std;


vector<unsigned int> SHA256::padParseInput(string& inputBinary)
{
  //Padding
  const int finalPaddingBits = 64;
  const int padFinder = 448;
  int l = inputBinary.length();
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

  //Parsing into blocks of words
  vector<unsigned int> inputWords;
  int wordCount = inputBinary.length() / bitsInWord;

  for (int i = 0; i < wordCount; i++)
  {
    int startingInd = (i * bitsInWord);
    bitset<SHA256::bitsInWord> curWord(inputBinary.substr(startingInd,
      bitsInWord));
    inputWords.push_back((unsigned int)curWord.to_ulong());
  }
  return inputWords;
}

int SHA256::rotr(int x, int n)
{
  return ((x >> n) | (x << (bitsInWord - n)));
}

int SHA256::bSigmaSub0(int x)
{
  const int rotVal0 = 2;
  const int rotVal1 = 13;
  const int rotVal2 = 22;
  int rotrX0 = rotr(x, rotVal0);
  int rotrX1 = rotr(x, rotVal1);
  int rotrX2 = rotr(x, rotVal2);
  int outputX = rotrX0^rotrX1^rotrX2;
  return outputX;
}
int SHA256::bSigmaSub1(int x)
{
  const int rotVal0 = 6;
  const int rotVal1 = 11;
  const int rotVal2 = 25;
  int rotrX0 = rotr(x, rotVal0);
  int rotrX1 = rotr(x, rotVal1);
  int rotrX2 = rotr(x, rotVal2);
  int outputX = rotrX0^rotrX1^rotrX2;
  return outputX;
}
int SHA256::sSigmaSub0(int x)
{
  const int rotVal0 = 7;
  const int rotVal1 = 18;
  const int shiftVal = 3;
  int rotrX0 = rotr(x, rotVal0);
  int rotrX1 = rotr(x, rotVal1);
  int shiftX = x >> shiftVal;
  int outputX = rotrX0^rotrX1^shiftX;
  return outputX;
}

int SHA256::sSigmaSub1(int x)
{
  const int rotVal0 = 17;
  const int rotVal1 = 19;
  const int shiftVal = 10;
  int rotrX0 = rotr(x, rotVal0);
  int rotrX1 = rotr(x, rotVal1);
  int shiftX = x >> shiftVal;
  int outputX = rotrX0^rotrX1^shiftX;
  return outputX;
}

int SHA256::chFunc(int x, int y, int z)
{
  return (x & y)^(~x & z);
}
int SHA256::majFunc(int x, int y, int z)
{
  return (x & y)^(x & z)^(y & z);
}

string SHA256::hash(string& inputBinary)
{
  const int messageSchedFirst = 16;
  const int messageSchedSecond = 64;
  const int workingVarCount = 8;

  unsigned int workingVars[] = {h0, h1, h2, h3, h4, h5, h6, h7};
  vector<unsigned int> inputWords = padParseInput(inputBinary);

  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  unsigned int e;
  unsigned int f;
  unsigned int g;
  unsigned int h;

  int blockCount = inputWords.size() / wordsInBlock;
  for (int i = 0; i < blockCount; i++)
  {
    const int msgSchedVal0 = 2;
    const int msgSchedVal1 = 7;
    const int msgSchedVal2 = 15;
    const int msgSchedVal3 = 16;

    a = workingVars[0];
    b = workingVars[1];
    c = workingVars[2];
    d = workingVars[3];
    e = workingVars[4];
    f = workingVars[5];
    g = workingVars[6];
    h = workingVars[7];

    vector<unsigned int> W;

    for (int j = 0; j < messageSchedFirst; j++)
    {
      W.push_back(inputWords.at((i * wordsInBlock) + j));
    }
    for (int j = messageSchedFirst; j < messageSchedSecond; j++)
    {
      unsigned int newWVal0 = sSigmaSub1(W.at(j - msgSchedVal0));
      unsigned int newWVal1 = W.at(j - msgSchedVal1);
      unsigned int newWVal2 = sSigmaSub0(W.at(j - msgSchedVal2));
      unsigned int newWVal3 = W.at(j - msgSchedVal3);
      W.push_back(newWVal0 + newWVal1 + newWVal2 + newWVal3);
    }

    for (int j = 0; j < messageSchedSecond; j++)
    {
      unsigned int t1 = h + bSigmaSub1(e) + chFunc(e, f, g) + K[j] + W.at(j);
      unsigned int t2 = bSigmaSub0(a) + majFunc(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }

    workingVars[0] += a;
    workingVars[1] += b;
    workingVars[2] += c;
    workingVars[3] += d;
    workingVars[4] += e;
    workingVars[5] += f;
    workingVars[6] += g;
    workingVars[7] += h;
  }

  string hashBinary = "";

  for (int i = 0; i < workingVarCount; i++)
  {
    bitset<SHA256::bitsInWord> curWord(workingVars[i]);
    hashBinary += curWord.to_string();
  }
  return hashBinary;
}
