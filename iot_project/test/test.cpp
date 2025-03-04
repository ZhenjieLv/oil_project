#include <iostream>
#include <string.h>
#include <cryptopp/aes.h>      // 包含AES加密算法的头文件
#include <cryptopp/filters.h>  // 包含加解密过程中使用的filters头文件
#include <cryptopp/modes.h>    // 包含加解密过程中使用的modes头文件
#include <cryptopp/hex.h>      // 包含将二进制转换为十六进制的头文件
using namespace std;
using namespace CryptoPP;
 
int main()
{
    // 明文和密钥
    string plaintext = "Hello, Crypto++";
    string key = "1234567890123456";  // AES-128密钥长度为16字节
    cout<<"test!"<<endl;
    //打印明文
    cout << "明文：" << plaintext << endl;
 
    // 加密过程
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE);
    CBC_Mode<AES>::Encryption encryption((byte *)key.c_str(), key.length(), iv);
    string ciphertext;
    StringSource(plaintext, true, new StreamTransformationFilter(encryption, new StringSink(ciphertext)));
 
    // 打印加密后的密文
    cout << "密文：" << ciphertext << endl;
 
    // 解密过程
    CBC_Mode<AES>::Decryption decryption((byte *)key.c_str(), key.length(), iv);
    string decryptedtext;
    StringSource(ciphertext, true, new StreamTransformationFilter(decryption, new StringSink(decryptedtext)));
 
   // 打印解密后的明文
    cout << "解密后的明文：" << decryptedtext << endl;
 
    return 0;
}