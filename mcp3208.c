/*
 * Controlling MCP3208 ADC by SPI (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * 
 * Modifications 2012 by Claus Kﾃｼhnel <info@ckuehnel.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Compile on Raspberry Pi by gcc mcp3208.c -o mcp3208
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define DEBUG 0

// Configuration bits for single-ended channel selection
#define CHANNEL0 0x060000   // Channel 0
#define CHANNEL1 0x064000   // Channel 1
#define CHANNEL2 0x068000   // Channel 2
#define CHANNEL3 0x06C000   // Channel 3
#define CHANNEL4 0x070000   // Channel 4
#define CHANNEL5 0x074000   // Channel 5
#define CHANNEL6 0x078000   // Channel 6
#define CHANNEL7 0x07C000   // Channel 7

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

uint32_t channel;

static void pabort(const char *s)
{
   perror(s);
   abort();
}

static void transfer(int fd)
{
   uint16_t adc;
   uint8_t ret,i;

   uint8_t tx[3]; 		// 3 bytes will be sent to MPC3208
   uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	
   for (i = 0; i < ARRAY_SIZE(tx); i++)
   {
      tx[ARRAY_SIZE(tx)-i-1] = channel%256;
      channel = channel >> 8;
   }

   printf("Sent bytes    : ");
   for (i=0; i < ARRAY_SIZE(tx); i++)
      printf("%02X ", tx[i]);
   printf("\n");

   struct spi_ioc_transfer tr = 
   {
      .tx_buf = (unsigned long)tx,
      .rx_buf = (unsigned long)rx,
      .len = ARRAY_SIZE(tx),
      .delay_usecs = delay,
      .speed_hz = speed,
      .bits_per_word = bits,
   };

   ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
   if (ret < 1)
      pabort("can't send spi message");

   printf("Received bytes: ");
   for (i = 0; i < ARRAY_SIZE(tx); i++)
      printf("%02X ", rx[i]);
   printf("\n");

   adc = (rx[1] << 8) + rx[2];
   adc &= 0xFFF;
   printf("ADC = %5X (hex)\n", adc);
   printf("ADC = %5d (dec)\n", adc);
   printf("ADC Voltage = %g mV\n", (float) adc *3300/4096); 
}

int main(int argc, char *argv[])
{
   int ret = 0;
   int fd;
	 
   fd = open(device, O_RDWR);
   if (fd < 0)
      pabort("can't open device");
   
   // max speed hz
   ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
   if (ret == -1)
      pabort("can't set max speed hz");

   ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
   if (ret == -1)
      pabort("can't get max speed hz");

#if DEBUG
   printf("spi device: %s\n", device);
   printf("spi mode: %d\n", mode);
   printf("bits per word: %d\n", bits);
   printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
#endif

   channel = CHANNEL7;  // we use channel 7 here
   transfer(fd);
   close(fd);
   return ret;
}
