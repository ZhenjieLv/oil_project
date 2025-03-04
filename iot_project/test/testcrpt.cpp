#include "cryptmsg.h"
#include <iostream>
using namespace std;
int main()
{ 
   string plaintext = "Hello, Crypto++";
   string str = msg_encrypt(plaintext);
   cout<<"加密后："<<str<<endl;
   string tmp = msg_decrypt(str);
   cout<<"解密后："<<tmp<<endl;
   return 0;
}