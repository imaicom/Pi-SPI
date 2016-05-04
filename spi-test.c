/*
 * Save as spi-test.c
 * Compile with: gcc -o spi-test spi-test.c -lwiringPi
 *
 * If no value: 
 * rmmod spi_bcm2708
 * modprobe spi_bcm2708
 * 
 * http://www.icbanq.com/pbloger/board_View.aspx?number=269
 * http://www.raspberrypi.org/forums/viewtopic.php?f=93&t=78551 (baart)
 * http://www.mikroe.com/add-on-boards/measurement/adc-proto/ (boards)
 * 
 */ 

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS_MCP3208  8       // BCM_GPIO8

#define SPI_CHANNEL 0
#define SPI_SPEED   100000  // !! Start low here and if all works try to increase if needed on a breadboard I could go upto about 750000 


int read_mcp3208_adc(unsigned char adcChannel)
{
  unsigned char buff[3];
  int adcValue = 0;

  buff[0] = 0x06 | ((adcChannel & 0x07) >> 7);
  buff[1] = ((adcChannel & 0x07) << 6);
  buff[2] = 0x00;

  digitalWrite(CS_MCP3208, 0);  // Low : CS Active

  wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

  buff[1] = 0x0F & buff[1];
  adcValue = ( buff[1] << 8) | buff[2];

  digitalWrite(CS_MCP3208, 1);  // High : CS Inactive

  return adcValue;
}


int main (void)
{
  int adc1Channel = 0;
  int adc1Value   = 0;
  int adc2Channel = 1;
  int adc2Value   = 0;
  int adc3Channel = 2;
  int adc3Value   = 0;
  int adc4Channel = 3;
  int adc4Value   = 0;
  int adc5Channel = 4;
  int adc5Value   = 0;
  int adc6Channel = 5;
  int adc6Value   = 0;
  int adc7Channel = 6;
  int adc7Value   = 0;
  int adc8Channel = 7;
  int adc8Value   = 0;
   
  if(wiringPiSetup() == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1 ;
  }

  if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
  {
    fprintf (stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno));
    return 1 ;
  }

  pinMode(CS_MCP3208, OUTPUT);

  while(1)
  {
    system("clear");
    printf("\n\nMCP3208 channel output.\n\n");
    adc1Value = read_mcp3208_adc(adc1Channel);
    printf("adc0 Value = %04u", adc1Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc1Value));
    adc2Value = read_mcp3208_adc(adc2Channel);
    printf("adc1 Value = %04u", adc2Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc2Value));
    adc3Value = read_mcp3208_adc(adc3Channel);
    printf("adc2 Value = %04u", adc3Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc3Value));
    adc4Value = read_mcp3208_adc(adc4Channel);
    printf("adc3 Value = %04u", adc4Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc4Value));
    adc5Value = read_mcp3208_adc(adc5Channel);
    printf("adc4 Value = %04u", adc5Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc5Value));
    adc6Value = read_mcp3208_adc(adc6Channel);
    printf("adc5 Value = %04u", adc6Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc6Value));
    adc7Value = read_mcp3208_adc(adc7Channel);
    printf("adc6 Value = %04u", adc7Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc7Value));
    adc8Value = read_mcp3208_adc(adc8Channel);
    printf("adc7 Value = %04u", adc8Value);
    printf("\tVoltage = %.3f\n", ((3.3/4096) * adc8Value));
    usleep(1000000);
  }
  return 0;
}
