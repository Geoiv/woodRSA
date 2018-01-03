#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include "../src/head/RSAConsts.hpp"
#include "../src/head/RSABigTypes.hpp"
#include "../src/head/RandGen.hpp"
using namespace std;

int main()
{
  RandGen rander(256);
  rander.instantiate(256);
  string randNum;
  rander.genRandom(10, randNum);
  cout << randNum << endl;
  rander.genRandom(10, randNum);
  cout << randNum << endl;
  rander.genRandom(10, randNum);
  cout << randNum << endl;
  rander.genRandom(10, randNum);
  cout << randNum << endl;
}
