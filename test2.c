//cc -o test2_3204 test2.c -lwiringPi -DMCP3204
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define DEBUG 0
#define SPI_CHANNEL 0

int main(int argc, char **argv){
    int retCode;
    int i;
    int a2dChannel = 0;  // analog channel
    int a2dVal = 0;
    float a2dVol = 0;
    float Vref = 3.3;
#ifdef MCP3002
    unsigned char data[2];
#endif
#ifdef MCP3204
    unsigned char data[3];
#endif

// SPI channel 0 を 1MHz で開始。
    if (wiringPiSPISetup(SPI_CHANNEL, 1000000) < 0)
    {
        printf("SPISetup failed:\n");
    }

    for(i=0;i<20;i++) {
#ifdef MCP3002
        data[0] = 0b01100000 |( ((a2dChannel & 0x03) << 4));  //  first byte transmitted -> start bit
        data[1] = 0; // third byte transmitted....don't care
if(DEBUG)printf("[MCP3002]data[0][1]=%02x-%02x\n",data[0],data[1]);
        retCode=wiringPiSPIDataRW (SPI_CHANNEL,data,sizeof(data));
if(DEBUG)printf("[MCP3002]wiringPiSPIDataRW=%d\n",retCode);
        a2dVal = (data[0]<< 8) & 0b1100000000; //first 2 bit
        a2dVal |=  (data[1] & 0xff);
        a2dVol = (float)a2dVal/1023 * Vref;
#endif
#ifdef MCP3204
        data[0] = 0b00000110 |( ((a2dChannel & 0x04) >> 2));  //  first byte transmitted -> start bit -> (SGL/DIF = 1, D2=0)
        data[1] = 0b00000000 |( ((a2dChannel & 0x03) << 6)); // second byte transmitted -> (D1=D0=0)
if(DEBUG)printf("[MCP3204]data[0][1]=%02x-%02x\n",data[0],data[1]);
        data[2] = 0; // third byte transmitted....don't care
        retCode=wiringPiSPIDataRW (SPI_CHANNEL,data,sizeof(data));
if(DEBUG)printf("[MCP3004]wiringPiSPIDataRW=%d\n",retCode);
if(DEBUG)printf("[MCP3004]data[1][2]=%02x-%02x\n",data[1],data[2]);
        a2dVal = (data[1]<< 8) & 0b111100000000; //first 4 bit
        a2dVal |=  (data[2] & 0xff);
        a2dVol = (float)a2dVal/4095 * Vref;
#endif

        printf("a2dVal=%d\n",a2dVal);
        printf("a2dVol=%f[V]\n",a2dVol);
        sleep(1);
    }
}
