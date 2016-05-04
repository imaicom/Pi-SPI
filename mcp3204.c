// cc -o mcp3204 mcp3204.c -lwiringPi -lm
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <math.h>

int main () {
  int value;
  float vIn;
  unsigned char data[3];
  
  wiringPiSetup();
  wiringPiSPISetup(0,1000000);
  
  printf("Raspberry Pi - SPI Analog Test\n");
  
  while(1) {
    data[0]=0x06;
    data[1]=0x3F;
    data[2]=0xFF;
    
    wiringPiSPIDataRW(0,data,3);
    value=((int)data[1]<<8)+(int)data[2];
    vIn=value * 3.3 / 4095.0;
    printf("Voltage = %2.2f\n",vIn);
    delay(500);
  }
}
