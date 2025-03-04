#include <iostream>
#include "dealpk.h"
int main()
{
   uint8_t *data;
   data=(uint8_t *)malloc(25*sizeof(uint8_t)); 
  uint8_t buffer[25] = { 0x03, 0x19, 0x00, 0x00, 0x00, 0x03, 0x00, 0x27, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x14, 0x00, 0x32, 0x00, 0x02, 0x18, 0x02, 0x09, 0x00, 0x20, 0x02};
  memcpy(data,buffer,9);
  AV_WL16(data+9,AV_RB16(buffer+9));
  AV_WL16(data+11,AV_RB16(buffer + 11));
  AV_WL16(data+13,AV_RB16(buffer + 13));
  AV_WL16(data+15,AV_RB16(buffer + 15));
  AV_WL16(data+17,AV_RB16(buffer + 17));
  AV_WL16(data+19,AV_RB16(buffer + 19));
  AV_WL16(data+21,AV_RB16(buffer + 21));
  AV_WL16(data+23,AV_RB16(buffer + 23));
  for(int i = 0;i<25;i++)
  {
    printf("%x ",data[i]);
  }
  printf("\n");
   free(data);
   return 0;
}