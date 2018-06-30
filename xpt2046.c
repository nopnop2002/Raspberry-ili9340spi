/*
 XPT2046 Touch Screen Controller Library for Raspberry

 Original 
 https://github.com/xofc/xpt2uinput
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "bcm2835.h"
#include "xpt2046.h"

#define MAX_LEN 4

#define	START	0x80
#define XPOS	0x50
#define YPOS	0x10

#define _DEBUG_ 0

int xptGetit(int cmd){
  char rbuf[MAX_LEN];
  char wbuf[MAX_LEN];

  memset(wbuf, 0, sizeof(rbuf));
  memset(rbuf, 0, sizeof(rbuf));
  wbuf[0] = cmd;
  bcm2835_spi_transfernb(wbuf, rbuf, sizeof(wbuf));
  return((rbuf[1]<<8)+rbuf[2]);
}

void xptGetxy(uint8_t chipSelect, int *xp, int *yp){
  bcm2835_spi_begin();

  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
//  bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
  bcm2835_spi_chipSelect(chipSelect);
//  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
  bcm2835_spi_setChipSelectPolarity(chipSelect, LOW);

  *xp = xptGetit(START |  XPOS);
  *yp = xptGetit(START |  YPOS);

  bcm2835_spi_end();
}


void xptInit(TouchInfo *hoge){
  hoge->tpc = 0;
  hoge->tpx = MAXTP;
  hoge->lsec = 0;
  hoge->lusec = 0;
}

void xptDump(TouchInfo *hoge){
  int i;
  if (_DEBUG_ == 0) return;
  printf("tpc=%d\n",hoge->tpc);
  printf("tpx=%d\n",hoge->tpx);
  for(i=0;i<hoge->tpc;i++) {
    printf("x1[%02d]=%d",i,hoge->tps[i].x1);
    printf(" x2[%02d]=%d",i,hoge->tps[i].x2);
    printf(" y1[%02d]=%d",i,hoge->tps[i].y1);
    printf(" y2[%02d]=%d",i,hoge->tps[i].y2);
    printf(" id[%02d]=%d\n",i,hoge->tps[i].id);
  }
}

void xptSetPoint(TouchInfo *hoge,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id){
  if (hoge->tpc == hoge->tpx) return;
  int index = hoge->tpc;
  if (x1 > x2) {
    hoge->tps[index].x1 = x2 * XRATE;
    hoge->tps[index].x2 = x1 * XRATE;
  } else {
    hoge->tps[index].x1 = x1 * XRATE;
    hoge->tps[index].x2 = x2 * XRATE;
  }
  if (y1 > y2) {
    hoge->tps[index].y1 = y2 * YRATE;
    hoge->tps[index].y2 = y1 * YRATE;
  } else {
    hoge->tps[index].y1 = y1 * YRATE;
    hoge->tps[index].y2 = y2 * YRATE;
  }
  hoge->tps[index].id = id;
  hoge->tpc++;
}

int xptGetPoint(uint8_t chipSelect, TouchInfo *hoge) {
  int x, y;
  int i;
  struct timeval myTime;
  struct tm *time_st;
  suseconds_t dt;

  xptGetxy(chipSelect, &x, &y);
if(_DEBUG_)printf("touch !! x=%5d y=%5d\n", x, y);
  gettimeofday(&myTime, NULL);
  time_st = localtime(&myTime.tv_sec);
  if (myTime.tv_sec == hoge->lsec) {
    dt = myTime.tv_usec - hoge->lusec;
if(_DEBUG_)printf("dt=%06d\n",dt);
    if (dt < DTMAX) return -1;
  }
  for(i=0;i<hoge->tpc;i++) {
    if(_DEBUG_) {
    printf("x1[%02d]=%d",i,hoge->tps[i].x1);
    printf(" x2[%02d]=%d",i,hoge->tps[i].x2);
    printf(" y1[%02d]=%d",i,hoge->tps[i].y1);
    printf(" y2[%02d]=%d",i,hoge->tps[i].y2);
    printf(" id[%02d]=%d\n",i,hoge->tps[i].id);
    }
    if (x > hoge->tps[i].x1 && x < hoge->tps[i].x2) {
      if (y > hoge->tps[i].y1 && y < hoge->tps[i].y2) {
        gettimeofday(&myTime, NULL);
        time_st = localtime(&myTime.tv_sec);
if(_DEBUG_)printf("myTime.tv_sec=%06d tv_usec=%06d\n",myTime.tv_sec,myTime.tv_usec);
        hoge->lsec = myTime.tv_sec;
        hoge->lusec = myTime.tv_usec;
if(_DEBUG_)printf("usec=%06d-%06d\n",hoge->lsec,hoge->lusec);
        return hoge->tps[i].id;
      }
    }
  }
  return -1;
}
