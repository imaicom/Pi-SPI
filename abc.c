#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

const int PI_CE  = 0;                  　 // CE0使用(CE0orCE1)
const int ADC_CH = 0;                   　// CH0使用
const int CLOCK_SPEED = 3200000;          // クロック
const int INTERVAL = 3;                   // インターバル秒

unsigned int mcp_read(int, int);

int main(void)
{
   unsigned int val;
   float volt_value;

   // SPI初期化(SPIチャンネル, 通信速度)
   if (wiringPiSPISetup(PI_CE, CLOCK_SPEED) < 0)
   {
      printf("wiringPiSPISetup failed\n");
      return EXIT_FAILURE;
   }

   // GPIO初期化
   if (wiringPiSetupGpio() == -1)
   {                  
      printf("wiringPiSetupGpio failed\n");
      return EXIT_FAILURE;
   }

   while (1)
   {
      val = mcp_read(PI_CE, ADC_CH);
      volt_value = 3.3 * (float)val / 4095.0;
      printf("volt_value = %f, val = %d\n", volt_value, val);
      sleep(INTERVAL);
   }

   return EXIT_SUCCESS;
}

unsigned int mcp_read(int pi_channel, int adc_channel)
{
   unsigned int val = 0;
   unsigned char data[3] = {};

   data[0] = 0b00000110 | (((pi_channel & 0x04) >> 2));
   data[1] = 0b00000000 | (((pi_channel & 0x03) << 6));
   data[2] = 0;
   wiringPiSPIDataRW(pi_channel, data, sizeof(data));
   // 12ビット
   val  = (data[1] << 8) & 0b111100000000;
   val += (data[2] & 0xff);

   return val;
}
