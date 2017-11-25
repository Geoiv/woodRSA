#include <gmpxx.h>
#include <bitset>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include "SHA256.cpp"
using namespace std;

  typedef mpz_class BigInt;
  typedef mpf_class BigFloat;

vector<bool> sieveProcedure(uint limitVal)
{
  const int minPrime = 2;
  vector<bool> primes(limitVal, true);
  primes.at(0) = false;
  primes.at(1) = false;
  for (uint i = minPrime; i < limitVal; i++)
  {
    if (primes.at(i))
    {
      for (uint j = i * 2; j < limitVal; j += i)
      {
        primes.at(j) = false;
      }
    }
  }

  return primes;
}

int main()
{
  BigInt test;
  test.set_str("127116100615364637055465439569564589172575006384519403543447397246721325262242541956253275883001305239638803001056104711468027753114194405309674037852774753620835005633143962957867452186578274031334454492653910512679395584886071938530148988866140574837446054931200493962311533606308484321222679968356008113771", 10);
  cout << test << endl;
  cout <<endl << sqrt(test) << endl;

  BigInt rootTest = sqrt(test);
  vector<bool> primes = sieveProcedure(rootTest.get_ui());

  for (BigInt itr = 0; itr <= primes.size(); itr++)
  {
    if (primes.at(itr.get_ui()))
    {
      BigInt testPrime = itr;
      if ((mpz_divisible_ui_p(test.get_mpz_t(), testPrime.get_mpz_t())) != 0)
      {
        //cout << "  " << c << " is divisible by " << testPrime << endl;
        cout << "AH" << endl;
      }
    }
  }
  cout << "done" << endl;
}