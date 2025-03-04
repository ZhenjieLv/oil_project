//#include "stdafx.h"
#include<iostream>
#include<string>
typedef unsigned char uint8_t;

using namespace std;

int main()
{
	uint8_t code_roi[5] = { 0XAA, 0X01, 0XE3, 0X23, 0X34 };
	string code_str;

	for (int i = 0; i < 5;i++)
	{
        //分离16进制数的"十位"和“个位”
		char s1 = char(code_roi[i] >> 4);
		char s2 = char(code_roi[i] & 0xf);
        //将分离得到的数字转换成对应的ASCII码，数字和字母分开，统一按照小写处理
		s1 > 9 ? s1 += 87 : s1 += 48;
		s2 > 9 ? s2 += 87 : s2 += 48;
        //将处理好的字符放入到string中
		code_str.append(1,s1);
		code_str.append(1, s2);
	}
	cout << "code_str=" << code_str << endl;

	system("pause");
    return 0;
}
