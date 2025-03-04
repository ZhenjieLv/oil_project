#include<iostream>
#include<string.h>
#include<stdio.h>
typedef unsigned char uint8_t;

using namespace std;

int main()
{
	// uint8_t code_roi[] = { 0X00, 0X00,0Xbc, 0X41 };
	// float f = *(float*)code_roi;
  //   printf("%f\n",f);
  //   return 0;
  uint8_t data[4];
  memset(data,0,sizeof(data));
  uint8_t code_roi[] = { 0x00, 0x00,0x41, 0xbc };
  data[0] = code_roi[1];
  data[1] = code_roi[0];
  data[2] = code_roi[3];
  data[3] = code_roi[2];
  float f = *(float*)data;
  printf("%f\n",f);
  return 0;

}