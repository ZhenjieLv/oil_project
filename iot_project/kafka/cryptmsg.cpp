#include "cryptmsg.h"
string msg_encrypt(const string& msg)
{
  string key = "1234567890123456";  // AES-128密钥长度为16字节
  byte iv[AES::BLOCKSIZE];
  memset(iv, 0x00, AES::BLOCKSIZE);
  CBC_Mode<AES>::Encryption encryption((byte *)key.c_str(), key.length(), iv);
  string ciphertext;
  StringSource(msg, true, new StreamTransformationFilter(encryption, new StringSink(ciphertext)));
  return ciphertext;
}
string msg_decrypt(const string& msg)
{
    string key = "1234567890123456";  // AES-128密钥长度为16字节
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE);
    CBC_Mode<AES>::Decryption decryption((byte *)key.c_str(), key.length(), iv);
    string decryptedtext;
    StringSource(msg, true, new StreamTransformationFilter(decryption, new StringSink(decryptedtext)));
    return decryptedtext;
}
