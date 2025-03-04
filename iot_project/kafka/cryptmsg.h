#ifndef _CRYPTMSG_H
#define _CRYPTMSG_H
#include <iostream>
#include <string.h>
#include <cryptopp/aes.h>      // 包含AES加密算法的头文件
#include <cryptopp/filters.h>  // 包含加解密过程中使用的filters头文件
#include <cryptopp/modes.h>    // 包含加解密过程中使用的modes头文件
#include <cryptopp/hex.h>      // 包含将二进制转换为十六进制的头文件
using namespace std;
using namespace CryptoPP;
string msg_encrypt(const string& msg);
string msg_decrypt(const string& msg);
#endif