///////////////////////////
//        ENCRYPT        //
///////////////////////////

/*
const std::string sha224LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                "649b934ca495991b";
const std::string sha256LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
                                "649b934ca495991b7852b855";
*/


// uint mLen = mpz_sizeinbase(pt.get_mpz_t(), binBase);
// const uint additPadbits = (2 * 8);
// if (mLen > (nLen - (2 * shaOutLen) - additPadbits))
// {
//   cout << "Input too large!" << endl;
// }
//
// string lHash;
// if (shaOutLen == sha0 || shaOutLen == sha1)
// {
//   lHash = sha224LHash;
// }
// else if (shaOutLen == sha2)
// {
//   lHash = sha256LHash;
// }
//
// gmp_randclass randGen(gmp_randinit_mt);
// randGen.seed(clock());
// string paddingString((nLen - mLen - (2 * shaOutLen) - additPadbits), '0');
//
// const string hexZero = "00";
// const string hexOne = "01";
//
// string concatString = lHash + paddingString + hexOne + plainTextString;
// BigInt dataBlock;
// dataBlock.set_str(concatString, hexBase);
//
// BigInt seed = randGen.get_z_bits(shaOutLen);
//
// BigInt dbMask;
// if(!maskGenFunc(seed, (nLen - shaOutLen - 1), dbMask))
// {
//   cout << "Data block mask generation failed." << endl;
//   return false;
// }
// BigInt maskedDB = dataBlock^dbMask;
// BigInt seedMask;
// if(!maskGenFunc(maskedDB, shaOutLen, seedMask))
// {
//   cout << "Seed mask generation failed." << endl;
//   return false;
// }
// BigInt maskedSeed = seed^seedMask;
// string emString = hexZero + maskedSeed.get_str(hexBase) + maskedDB.get_str(hexBase);
// BigInt em;
// em.set_str(emString, hexBase);

// mpz_powm(ct.get_mpz_t(), em.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

///////////////////////////
//        DECRYPT        //
///////////////////////////

/*
if (mpz_sizeinbase(ct.get_mpz_t(), binBase) != nLen)
{
  cout << mpz_sizeinbase(ct.get_mpz_t(), binBase) << endl;
  cout << "Ciphertext is not of proper nLen." << endl;
  return false;
}
if (nLen < ((2 * shaOutLen) + 2))
{
  cout << "nLen too small." << endl;
  return false;
}

string lHash;
if (shaOutLen == sha0 || shaOutLen == sha1)
{
  lHash = sha224LHash;
}
else if (shaOutLen == sha2)
{
  lHash = sha256LHash;
}

const uint hexCharsInOctet = 2;

if (cipherTextString.substr(0, hexCharsInOctet).compare(hexZero) != 0)
{
  cout << "Y is nonZero!" << endl;
  return false;
}

BigInt maskedSeed;
maskedSeed.set_str(cipherTextString.substr(hexCharsInOctet, shaOutLen),
                                           hexBase);
BigInt maskedDB;
maskedDB.set_str(cipherTextString.substr(hexCharsInOctet + shaOutLen,
                                         (nLen - shaOutLen - 1)), hexBase);
BigInt seedMask;
if(!maskGenFunc(maskedDB, shaOutLen, seedMask))
{
  cout << "Seed mask generation failed." << endl;
  return false;
}
BigInt seed = maskedSeed^seedMask;
BigInt dbMask;
if(!maskGenFunc(seed, (nLen - shaOutLen - 1), dbMask))
{
  cout << "Data block mask generation failed." << endl;
  return false;
}



BigInt dataBlock = maskedDB^dbMask;
string dbString = dataBlock.get_str(hexBase);

string lHashPrime = dbString.substr(0, shaOutLen);
if (shaOutLen == sha0 || shaOutLen == sha1)
{
  if (lHashPrime.compare(sha224LHash) != 0)
  {
    cout << "lHash' does not equal lHash!" << endl;
    return false;
  }
}
else if (shaOutLen == sha2)
{
  if (lHashPrime.compare(sha256LHash) != 0)
  {
    cout << "lHash' does not equal lHash!" << endl;
    return false;
  }
}

uint oneIndex = dbString.find(hexOne, shaOutLen);
if (oneIndex == dbString.size())
{
  cout << "Padding '01' not found!" << endl;
  return false;
}

string m = dbString.substr(oneIndex + hexCharsInOctet);
BigInt mInt;
mInt.set_str(m, hexBase);
*/

// mpz_powm(m1.get_mpz_t(), mInt.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
// mpz_powm(m2.get_mpz_t(), mInt.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());
