/*
 XPT2046 Touch Screen Controller Library for Raspberry

 I ported from here.
 https://github.com/xofc/xpt2uinput
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>

#include "ili9340.h"
#include "xpt2046.h"

#define MAX_LEN 3

#define	XPT_START	0x80
#define XPT_XPOS 0x50
#define XPT_YPOS 0x10
#define XPT_8BIT 0x80
#define XPT_SER	0x04
#define XPT_DEF	0x03

#define _DEBUG_ 0

int xptGetit(int cmd){
	char rbuf[MAX_LEN];
	char wbuf[MAX_LEN];

	memset(wbuf, 0, sizeof(rbuf));
	memset(rbuf, 0, sizeof(rbuf));
	wbuf[0] = cmd;
	bcm2835_spi_transfernb(wbuf, rbuf, sizeof(wbuf));
if(_DEBUG_)printf("rbuf[0]=%02x rbuf[1]=%02x rbuf[2]=%02x\n", rbuf[0], rbuf[1], rbuf[2]);
	//return((rbuf[1]<<8)+rbuf[2]);
	return((rbuf[1]<<4) + (rbuf[2]>>4));
}

void xptGetxy(int chipSelect, int *xp, int *yp){
	bcm2835_spi_begin();

	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	//bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
	bcm2835_spi_chipSelect(chipSelect);
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	bcm2835_spi_setChipSelectPolarity(chipSelect, LOW);

#if 0
	*xp = xptGetit(XPT_START | XPT_XPOS);
	*yp = xptGetit(XPT_START | XPT_YPOS);
#endif
	*xp = xptGetit(XPT_START | XPT_XPOS | XPT_SER);
	*yp = xptGetit(XPT_START | XPT_YPOS | XPT_SER);

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
		hoge->tps[index].x1 = x2;
		hoge->tps[index].x2 = x1;
	} else {
		hoge->tps[index].x1 = x1;
		hoge->tps[index].x2 = x2;
	}
	if (y1 > y2) {
		hoge->tps[index].y1 = y2;
		hoge->tps[index].y2 = y1;
	} else {
		hoge->tps[index].y1 = y1;
		hoge->tps[index].y2 = y2;
	}
	hoge->tps[index].id = id;
	hoge->tpc++;
}

#define DTMAX 800000 // 800m sec

int xptGetPoint(uint8_t chipSelect, TouchInfo *hoge) {
	int i;
	struct timeval myTime;
	//struct tm *time_st;
	suseconds_t dt;

	float _xd = hoge->_max_xp - hoge->_min_xp;
	float _yd = hoge->_max_yp - hoge->_min_yp;
	float _xs = hoge->_max_xc - hoge->_min_xc;
	float _ys = hoge->_max_yc - hoge->_min_yc;

	int _xp;
	int _yp;
	xptGetxy(chipSelect, &_xp, &_yp);
if(_DEBUG_)printf("touch !! _xp=%5d  hoge->_min_xp=%5d hoge->_max_xp=%5d\n", _xp, hoge->_min_xp, hoge->_max_xp);
if(_DEBUG_)printf("touch !! _yp=%5d  hoge->_min_yp=%5d hoge->_max_yp=%5d\n", _yp, hoge->_min_yp, hoge->_max_yp);
	if (_xp < hoge->_min_xp && _xp > hoge->_max_xp) return -1;
	if (_yp < hoge->_min_yp && _yp > hoge->_max_yp) return -1;
	int _xpos = ( (float)(_xp - hoge->_min_xp) / _xd * _xs ) + hoge->_min_xc;
	int _ypos = ( (float)(_yp - hoge->_min_yp) / _yd * _ys ) + hoge->_min_yc;

	// Disable double touch
	gettimeofday(&myTime, NULL);
	//time_st = localtime(&myTime.tv_sec);
	localtime(&myTime.tv_sec);
	if (myTime.tv_sec == hoge->lsec) {
		dt = myTime.tv_usec - hoge->lusec;
		if (dt < DTMAX) return -1;
	}

	// Check range
	for(i=0;i<hoge->tpc;i++) {
		if(_DEBUG_) {
		printf("x1[%02d]=%d",i,hoge->tps[i].x1);
		printf(" x2[%02d]=%d",i,hoge->tps[i].x2);
		printf(" y1[%02d]=%d",i,hoge->tps[i].y1);
		printf(" y2[%02d]=%d",i,hoge->tps[i].y2);
		printf(" id[%02d]=%d\n",i,hoge->tps[i].id);
		}
		if (_xpos > hoge->tps[i].x1 && _xpos < hoge->tps[i].x2) {
			if (_ypos > hoge->tps[i].y1 && _ypos < hoge->tps[i].y2) {
				gettimeofday(&myTime, NULL);
				//time_st = localtime(&myTime.tv_sec);
				localtime(&myTime.tv_sec);
				hoge->lsec = myTime.tv_sec;
				hoge->lusec = myTime.tv_usec;
				return hoge->tps[i].id;
			}
		}
	}
	return -1;
}
