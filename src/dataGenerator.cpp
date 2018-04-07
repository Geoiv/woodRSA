#include "./head/RSAConsts.hpp"
#include <fstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <gmpxx.h>
using namespace std;

  typedef mpz_class BigInt;

//Key sizes of 512b, 1024b, 2048b, 4096b
const vector<uint> keySizes = {512, 1024, 2048, 4096};
//Data sizes of 64B, 1kB, 32kB, 64kB, 128kB, 1MB
const vector<uint> fileSizes = {64, 1024, 32768, 65536, 131072, 1048576};
int main()
{
  srand((unsigned) time(0));
  int randomInt;
  fstream dataWriter(dataFileName, fstream::out);
  for (uint size : fileSizes)
  {
    for (uint i = 0; i < size; i++)
    {
      randomInt = (rand() % 16);
      dataWriter << hex << randomInt;
    }
    dataWriter << "\n";
  }
  dataWriter.close();

}
