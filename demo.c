#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "ili9340.h"

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];
FontxFile fxG16[2];
FontxFile fxM16[2];

#define _DEBUG_ 1

void inputKey() {
  char ch;
  printf("Hit any key");
  scanf("%c",&ch);
}

int main(int argc, char **argv)
{
  int i,j;
  uint16_t Y1,Y2;
  uint16_t xpos,ypos;
  uint16_t color;
  uint16_t size;
  unsigned char utf[20];
  int model;
  int XMAX = 0;
  int YMAX = 0;
  int XMAX2,YMAX2;
  int ARROW;
  int FSIZE;

  if (argc == 1) {
    printf("Usage : %s TFT_SIZE\n",argv[0]);
    printf(" 1:240 x 320\n");
    printf(" 2:128 x 160\n");
    printf(" 3:128 x 128\n");
    return 1;
  }
  model = atoi(argv[1]);
  if (model == 1) {
    XMAX = 240;
    YMAX = 320;
    ARROW = 1;
    FSIZE = 32;
  } else if (model == 2) {
    XMAX = 128;
    YMAX = 160;
    ARROW = 0;
    FSIZE = 16;
  } else if (model == 3) {
    XMAX = 128;
    YMAX = 128;
    ARROW = 0;
    FSIZE = 16;
  }
  XMAX2 = XMAX - 1;
  YMAX2 = YMAX - 1;
  printf("TFT size %d x %d\n",XMAX,YMAX);

#if 0
  if (bcm2835_init() == -1) {
    printf("bmc2835_init Error\n");
    return 1;
  }
#endif

  // You can change font file
  // 32Dot Gothic
  Fontx_init(fxG32,"./fontx/ILGH32XB.FNT","./fontx/ILGZ32XB.FNT");
  // 32Dot Mincho
  Fontx_init(fxM32,"./fontx/ILMH32XF.FNT","./fontx/ILMZ32XF.FNT");
  // 24Dot Gothic
  Fontx_init(fxG24,"./fontx/ILGH24XB.FNT","./fontx/ILGZ24XB.FNT");
  // 24Dot Mincho
  Fontx_init(fxM24,"./fontx/ILMH24XF.FNT","./fontx/ILMZ24XF.FNT");
  // 16Dot Gothic
  Fontx_init(fxG16,"./fontx/ILGH16XB.FNT","./fontx/ILGZ16XB.FNT");
  // 16Dot Mincyo
  Fontx_init(fxM16,"./fontx/ILMH16XB.FNT","./fontx/ILMZ16XF.FNT");

  lcdInit(XMAX, YMAX);
  lcdReset();
  lcdSetup();

  //color bar
  Y1=YMAX/3;
  Y2=(YMAX/3)*2;
  for(i=0;i<XMAX;i++){
    for(j=0;j<YMAX;j++){
    if(j<Y1){
      lcdDrawPixel(i,j,RED);
    } else if(j<Y2) {
      lcdDrawPixel(i,j,GREEN);
    } else {
      lcdDrawPixel(i,j,BLUE);
    }
   }
  }
if(_DEBUG_)inputKey();

  //coner angle
  lcdFillScreen(WHITE);
  color = RED;
  lcdDrawFillRect(0,0,20,20,color);
  if(ARROW) {
    lcdDrawFillArrow(30,30,20,20,10,color);
    xpos = 30;
    ypos = 30;
    strcpy((char *)utf,"0,0");
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  } else {
    xpos = 00;
    ypos = 25;
    strcpy((char *)utf,"0,0");
    lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);
  }

  color = GRAY;
  lcdDrawFillRect(XMAX-20,0,XMAX,20,color);
  if(ARROW) {
    lcdDrawFillArrow(XMAX-30,30,XMAX-20,20,10,color);
    xpos = 130;
    ypos = 30;
    strcpy((char *)utf,"239,0");
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  } else {
    xpos = 80;
    ypos = 25;
    sprintf((char *)utf,"%d,0",XMAX);
    lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);
  }

  color = BLUE;
  lcdDrawFillRect(0,YMAX-20,20,YMAX,color);
  if(ARROW) {
    lcdDrawFillArrow(30,YMAX-30,20,YMAX-20,10,color);
    xpos = 30;
    ypos = 250;
    strcpy((char *)utf,"0,319");
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  } else {
    xpos = 00;
    ypos = YMAX -  40;
    sprintf((char *)utf,"0,%d",YMAX);
    lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);
  }


  color = PURPLE;
  lcdDrawFillRect(XMAX-20,YMAX-20,XMAX,YMAX,color);
  if(ARROW) {
    lcdDrawFillArrow(XMAX-30,YMAX-30,XMAX-20,YMAX-20,10,color);
    xpos = 130;
    ypos = 250;
    strcpy((char *)utf,"239,319");
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  } else {
    xpos = 70;
    ypos = YMAX -  40;
    sprintf((char *)utf,"%d,%d",XMAX,YMAX);
    lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);
  }
if(_DEBUG_)inputKey();

  //drawLine
  lcdFillScreen(WHITE);
  color=RED;
  for(ypos=0;ypos<YMAX;ypos=ypos+10) {
    lcdDrawLine(0,ypos,XMAX,ypos,color);
  }

  for(xpos=0;xpos<XMAX;xpos=xpos+10) {
    lcdDrawLine(xpos,0,xpos,YMAX,color);
  }
if(_DEBUG_)inputKey();

  //drawRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRect(i,i,XMAX-i,YMAX-i,color);
  }
if(_DEBUG_)inputKey();

  //drawCircle
  lcdFillScreen(WHITE);
  color=GRAY;
  xpos=XMAX/2;
  ypos=YMAX/2;
  for(i=10;i<220;i=i+10) {
    lcdDrawCircle(xpos,ypos,i,color);
  }
if(_DEBUG_)inputKey();

  //drawRoundRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRoundRect(i,i,XMAX-i,YMAX-i,10,color);
  }
if(_DEBUG_)inputKey();

  //drawString
  lcdFillScreen(WHITE);
  xpos = 0;
  ypos = YMAX2-(FSIZE*1);
  color = BLACK;
  sprintf((char *)utf,"%dx%d",XMAX,YMAX);
//  strcpy((char *)utf,"ILI9340");
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = 0;
  ypos = YMAX2-(FSIZE*2);
  lcdSetFontFill(CYAN);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = 0;
  ypos = YMAX2-(FSIZE*3);
  lcdSetFontUnderLine(BLACK);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION180);
  xpos = XMAX2;
  ypos = FSIZE*1;
  color = BLUE;
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2;
  ypos = FSIZE*2;
  lcdSetFontFill(YELLOW);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2;
  ypos = FSIZE*3;
  lcdSetFontUnderLine(BLUE);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION90);
  xpos = XMAX2-(FSIZE*1);
  ypos = YMAX2;
  color = RED;
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(FSIZE*2);
  ypos = YMAX2;
  lcdSetFontFill(GREEN);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2-(FSIZE*3);
  ypos = YMAX2;
  lcdSetFontUnderLine(RED);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION270);
  xpos = (FSIZE*1);
  ypos = 0;
  color = GRAY;
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  xpos = (FSIZE*2);
  ypos = 0;
  lcdSetFontFill(CYAN);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();
  lcdUnsetFontFill();

  xpos = (FSIZE*3);
  ypos = 0;
  lcdSetFontUnderLine(GRAY);
  if (FSIZE == 32) 
    lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  if (FSIZE == 24) 
    lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);
  if (FSIZE == 16) 
    lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();
  lcdUnsetFontUnderLine();
if(_DEBUG_)inputKey();

  //draw malti font
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);

  if (model == 1) {
  xpos = XMAX2-(32*1);
  ypos = YMAX2;
  color = BLACK;
  strcpy((char *)utf,"32Dot Gothic");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*2);
  strcpy((char *)utf,"32ドットゴシック");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*3);
  ypos = YMAX2;
  strcpy((char *)utf,"32Dot Mincho");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*4);
  strcpy((char *)utf,"32ドット明朝");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*0);
  ypos = YMAX2;
  strcpy((char *)utf,"24Dot Gothic");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*1);
  strcpy((char *)utf,"24ドットゴシック");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*2);
  ypos = YMAX2;
  strcpy((char *)utf,"24Dot Mincho");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*3);
  strcpy((char *)utf,"24ドット明朝");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);
if(_DEBUG_)inputKey();
  }

  if (model == 3) {
  xpos = XMAX2-(16*1);
  ypos = YMAX2;
  color = BLACK;
  strcpy((char *)utf,"16Dot Gothic");
  lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(16*2);
  strcpy((char *)utf,"16ドットゴシック");
  lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(16*3);
  strcpy((char *)utf,"16ﾄﾞｯﾄｺﾞｼｯｸ");
  lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(16*5);
  strcpy((char *)utf,"16Dot Mincho");
  lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);

  xpos = XMAX2-(16*6);
  strcpy((char *)utf,"16ドット明朝");
  lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);

  xpos = XMAX2-(16*7);
  strcpy((char *)utf,"16ﾄﾞｯﾄﾐﾝﾁｮｳ");
  lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);
if(_DEBUG_)inputKey();
  }



  //drawFillRect
  lcdFillScreen(WHITE);
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  srand( (unsigned int)time( NULL ) );
  for(i=1;i<100;i++) {
    red=rand()%255;
    green=rand()%255;
    blue=rand()%255;
    color=rgb565_conv(red,green,blue);
    xpos=rand()%XMAX;
    ypos=rand()%YMAX;
    size=rand()%(XMAX/10);
    lcdDrawFillRect(xpos,ypos,xpos+size,ypos+size,color);
  }
  return 0;
}
