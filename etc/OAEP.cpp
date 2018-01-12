// /*
// ///////////////////////////
// //        ENCRYPT        //
// ///////////////////////////
//
//
// const std::string sha224LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
//                                 "649b934ca495991b";
// const std::string sha256LHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
//                                 "649b934ca495991b7852b855";
//
//
// /*RSAES-OAEP encryption as specified in PKCS #1 version 2.2
//   Labels are not utilized, as they are not needed in this context.*/
// bool RSACipher::encryptOAEP(string plainTextString, string& cipherTextString)
// {
//   if (e == 0 || d == 0 || n == 0)
//   {
//     cout << "Key data not available! Please visit the key option menu to "
//       "generate RSA keys." << endl;
//     return false;
//   }
//
//   BigInt pt;
//   pt.set_str(plainTextString, hexBase);
//
//   if (mpz_sizeinbase(pt.get_mpz_t(), binBase) > nLen)
//   {
//     cout << "Input too large!" << endl;
//   }
//
//   BigInt ct;
//   uint mLen = mpz_sizeinbase(pt.get_mpz_t(), binBase);
//   const uint additPadbits = (2 * 8);
//   if (mLen > (nLen - (2 * shaOutLen) - additPadbits))
//   {
//     cout << "Input too large!" << endl;
//   }
//
//   string lHash;
//   if (shaOutLen == sha224OutLen)
//   {
//     lHash = sha224LHash;
//   }
//   else if (shaOutLen == sha256OutLen)
//   {
//     lHash = sha256LHash;
//   }
//   RandGen randGen(shaOutLen);
//   if(!randGen.instantiate())
//   {
//     cout << "Initializing RNG failed!" << endl;
//     return false;
//   }
//   string paddingString((nLen - mLen - (2 * shaOutLen) - additPadbits), '0');
//
//   const string hexZero = "00";
//   const string hexOne = "01";
//
//   string concatString = lHash + paddingString + hexOne + plainTextString;
//   BigInt dataBlock;
//   dataBlock.set_str(concatString, hexBase);
//
//   BigInt seed;
//   string seedString;
//   randGen.genRandom(shaOutLen, seedString);
//
//   seed.set_str(seedString, hexBase);
//
//   BigInt dbMask;
//   if(!maskGenFunc(seed, (nLen - shaOutLen - 1), dbMask))
//   {
//     cout << "Data block mask generation failed." << endl;
//     return false;
//   }
//   BigInt maskedDB = dataBlock^dbMask;
//   BigInt seedMask;
//   if(!maskGenFunc(maskedDB, shaOutLen, seedMask))
//   {
//     cout << "Seed mask generation failed." << endl;
//     return false;
//   }
//   BigInt maskedSeed = seed^seedMask;
//   string emString = hexZero + maskedSeed.get_str(hexBase) + maskedDB.get_str(hexBase);
//   BigInt em;
//   em.set_str(emString, hexBase);
//
//   cout << endl << emString << endl;
//
//   mpz_powm(ct.get_mpz_t(), em.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
//
//   cipherTextString = ct.get_str(hexBase);
//   if (cipherTextString.size() % 2 != 0)
//   {
//     cipherTextString = "0" + cipherTextString;
//   }
//   return true;
// }
//
// ///////////////////////////
// //        DECRYPT        //
// ///////////////////////////
// /*RSAES-OAEP decryption as specified in PKCS #1 version 2.2 with options for CRT
//   Labels are not utilized, as they are not needed in this context.
//   TODO finish pkcs 2.2*/
// bool RSACipher::decryptOAEP(string cipherTextString, string& plainTextString,
//                             bool crtFlag)
// {
//   const string hexZero = "00";
//   const string hexOne = "01";
//
//   if (e == 0 || d == 0 || n == 0)
//   {
//     cout << "Key data not available! Please visit the key option menu to "
//       "generate RSA keys." << endl;
//     return false;
//   }
//
//   BigInt ct;
//   ct.set_str(cipherTextString, hexBase);
//
//   if (mpz_sizeinbase(ct.get_mpz_t(), binBase) > nLen)
//   {
//     cout << "Input too large!" << endl;
//   }
//
//
//
//
//   if (mpz_sizeinbase(ct.get_mpz_t(), binBase) != nLen)
//   {
//     cout << mpz_sizeinbase(ct.get_mpz_t(), binBase) << endl;
//     cout << "Ciphertext is not of proper nLen." << endl;
//     return false;
//   }
//   if (nLen < ((2 * shaOutLen) + 2))
//   {
//     cout << "nLen too small." << endl;
//     return false;
//   }
//
//   BigInt encodedPt;
//
//   if (crtFlag)
//   {
//     BigInt dP = d % (p - 1);
//     if (dP < 0)
//     {
//       dP += (p - 1);
//     }
//     BigInt dQ = d % (q - 1);
//     if (dP < 0)
//     {
//       dQ += (q - 1);
//     }
//     BigInt qInv;
//     if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
//     {
//       cout << "Error getting qInv!" << endl;
//       return false;
//     }
//     BigInt m1;
//     BigInt m2;
//
//     mpz_powm(m1.get_mpz_t(), ct.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
//     mpz_powm(m2.get_mpz_t(), ct.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());
//
//     BigInt h = (qInv * (m1 - m2)) % p;
//     if (h < 0)
//     {
//       h += p;
//     }
//
//     encodedPt = m2 + (h * q);
//   }
//   else
//   {
//     mpz_powm(encodedPt.get_mpz_t(), ct.get_mpz_t(), d.get_mpz_t(),
//              n.get_mpz_t());
//   }
//
//   string encodedPtString = encodedPt.get_str(hexBase);
//
//   string lHash;
//   if (shaOutLen == sha224OutLen)
//   {
//     lHash = sha224LHash;
//   }
//   else if (shaOutLen == sha256OutLen)
//   {
//     lHash = sha256LHash;
//   }
//
//   const uint hexCharsInOctet = 2;
//
//   if (encodedPtString.substr(0, hexCharsInOctet).compare(hexZero) != 0)
//   {
//     cout << "Y is nonZero!" << endl;
//     cout << encodedPtString << endl;
//     return false;
//   }
//
//   BigInt maskedSeed;
//   maskedSeed.set_str(cipherTextString.substr(hexCharsInOctet, shaOutLen),
//                                              hexBase);
//   BigInt maskedDB;
//   maskedDB.set_str(cipherTextString.substr(hexCharsInOctet + shaOutLen,
//                                            (nLen - shaOutLen - 1)), hexBase);
//   BigInt seedMask;
//   if(!maskGenFunc(maskedDB, shaOutLen, seedMask))
//   {
//     cout << "Seed mask generation failed." << endl;
//     return false;
//   }
//   BigInt seed = maskedSeed^seedMask;
//   BigInt dbMask;
//   if(!maskGenFunc(seed, (nLen - shaOutLen - 1), dbMask))
//   {
//     cout << "Data block mask generation failed." << endl;
//     return false;
//   }
//
//   BigInt dataBlock = maskedDB^dbMask;
//   string dbString = dataBlock.get_str(hexBase);
//
//   string lHashPrime = dbString.substr(0, shaOutLen);
//   if (shaOutLen == sha224OutLen)
//   {
//     if (lHashPrime.compare(sha224LHash) != 0)
//     {
//       cout << "lHash' does not equal lHash!" << endl;
//       return false;
//     }
//   }
//   else if (shaOutLen == sha256OutLen)
//   {
//     if (lHashPrime.compare(sha256LHash) != 0)
//     {
//       cout << "lHash' does not equal lHash!" << endl;
//       return false;
//     }
//   }
//
//   uint oneIndex = dbString.find(hexOne, shaOutLen);
//   if (oneIndex == dbString.size())
//   {
//     cout << "Padding '01' not found!" << endl;
//     return false;
//   }
//
//   plainTextString = dbString.substr(oneIndex + hexCharsInOctet);
//
//   if (plainTextString.size() % 2 != 0)
//   {
//     plainTextString = "0" + plainTextString;
//   }
//
//   return true;
// }
//
// ///////////////////////////
// //         SIGN          //
// ///////////////////////////
//
// /*RSASSA-PSS signing as specified in PKCS #1 version 2.2 with options for CRT
//   Labels are not utilized, as they are not needed in this context.
//   TODO finish pkcs 2.2*/
// bool RSACipher::signPSS(string plainTextString, string& cipherTextString,
//                         bool crtFlag)
// {
//  if (e == 0 || d == 0 || n == 0)
//  {
//    cout << "Key data not available! Please visit the key option menu to "
//      "generate RSA keys." << endl;
//    return false;
//  }
//
//  BigInt pt;
//  pt.set_str(plainTextString, hexBase);
//
//  if (mpz_sizeinbase(pt.get_mpz_t(), binBase) > nLen)
//  {
//    cout << "Input too large!" << endl;
//  }
//
//  BigInt ct;
//
//  if (crtFlag)
//  {
//    BigInt dP = d % (p - 1);
//    if (dP < 0)
//    {
//      dP += (p - 1);
//    }
//    BigInt dQ = d % (q - 1);
//    if (dP < 0)
//    {
//      dQ += (q - 1);
//    }
//    BigInt qInv;
//    if (!mpz_invert(qInv.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t()))
//    {
//      cout << "Error getting qInv!" << endl;
//      return false;
//    }
//    BigInt m1;
//    BigInt m2;
//    mpz_powm(m1.get_mpz_t(), pt.get_mpz_t(), dP.get_mpz_t(), p.get_mpz_t());
//    mpz_powm(m2.get_mpz_t(), pt.get_mpz_t(), dQ.get_mpz_t(), q.get_mpz_t());
//
//    BigInt h = (qInv * (m1 - m2)) % p;
//    if (h < 0)
//    {
//      h += p;
//    }
//    ct = m2 + (h * q);
//  }
//  else
//  {
//    mpz_powm(ct.get_mpz_t(), pt.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
//  }
//  cipherTextString = ct.get_str(hexBase);
//  if (cipherTextString.size() % 2 != 0)
//  {
//    cipherTextString = "0" + cipherTextString;
//  }
//  return true;
// }
//
// ///////////////////////////
// //         AUTH          //
// ///////////////////////////
//
// /*RSASSA-PSS authentication as specified in PKCS #1 version 2.2 with options
//   for CRT.
//   Labels are not utilized, as they are not needed in this context.
//   TODO finish pkcs 2.2*/
// bool RSACipher::authPSS(string cipherTextString, string& plainTextString)
// {
//   if (e == 0 || d == 0 || n == 0)
//   {
//     cout << "Key data not available! Please visit the key option menu to "
//       "generate RSA keys." << endl;
//     return false;
//   }
//
//   BigInt ct;
//   ct.set_str(cipherTextString, hexBase);
//
//   if (mpz_sizeinbase(ct.get_mpz_t(), binBase) > nLen)
//   {
//     cout << "Input too large!" << endl;
//   }
//
//   BigInt pt;
//   mpz_powm(pt.get_mpz_t(), ct.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
//   plainTextString = pt.get_str(hexBase);
//   if (plainTextString.size() % 2 != 0)
//   {
//     plainTextString = "0" + plainTextString;
//   }
//   return true;
// }
//
// */
