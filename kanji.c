#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <bcm2835.h>
#include "ili9340.h"

#define XMAX    240
#define YMAX    320

#define XMAX2   239
#define YMAX2   319

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

int main()
{
  int i,j;
  uint16_t Y1,Y2;
  uint16_t X1,X2;
  uint16_t xpos,ypos;
  uint16_t color;
  uint16_t size;
  uint8_t utf[64];

  if (bcm2835_init() == -1) {
    printf("bmc2835_init Error\n");
    return 1;
  }

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
  // 16Dot Mincho
  Fontx_init(fxM16,"./fontx/ILMH16XB.FNT","./fontx/ILMZ16XF.FNT");

  lcdInit();
  lcdReset();
  lcdSetup();

  //drawString
  lcdFillScreen(WHITE);
  xpos = 0;
  ypos = YMAX2-(32*1);
  color = BLACK;
  strcpy(utf,"漢字TEST");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = 0;
  ypos = YMAX2-(32*2);
  lcdSetFontFill(CYAN);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = 0;
  ypos = YMAX2-(32*3);
  lcdSetFontUnderLine(BLACK);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION180);
  xpos = XMAX2;
  ypos = 32*1;
  color = BLUE;
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2;
  ypos = 32*2;
  lcdSetFontFill(YELLOW);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2;
  ypos = 32*3;
  lcdSetFontUnderLine(BLUE);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION90);
  xpos = XMAX2-(32*1);
  ypos = YMAX2;
  color = RED;
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*2);
  ypos = YMAX2;
  lcdSetFontFill(GREEN);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2-(32*3);
  ypos = YMAX2;
  lcdSetFontUnderLine(RED);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION270);
  xpos = (32*1);
  ypos = 0;
  color = GRAY;
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = (32*2);
  ypos = 0;
  lcdSetFontFill(CYAN);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = (32*3);
  ypos = 0;
  lcdSetFontUnderLine(GRAY);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();
  sleep(1);
if(_DEBUG_)inputKey();

  //draw malti font
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);

  xpos = XMAX2-(32*1);
  ypos = YMAX2;
  color = BLACK;
  strcpy(utf,"32ドットゴシック");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*2);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*3);
  ypos = YMAX2;
  strcpy(utf,"32ドット明朝");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*4);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*0);
  ypos = YMAX2;
  strcpy(utf,"24ドットゴシック");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*1);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*2);
  ypos = YMAX2;
  strcpy(utf,"24ドット明朝");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*3);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  sleep(1);
if(_DEBUG_)inputKey();

  //draw malti font
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);

  xpos = XMAX2-(24*1);
  ypos = YMAX2;
  color = BLACK;
  strcpy(utf,"24ドットゴシック");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(24*2);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(24*3);
  ypos = YMAX2;
  strcpy(utf,"24ドット明朝");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  xpos = XMAX2-(24*4);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  xpos = XMAX2-(24*5)-(16*0);
  ypos = YMAX2;
  strcpy(utf,"16ドットゴシック");
  xpos = lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(24*5)-(16*1);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG16, xpos, ypos, utf, color);

  xpos = XMAX2-(24*5)-(16*2);
  ypos = YMAX2;
  strcpy(utf,"16ドット明朝");
  xpos = lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);

  xpos = XMAX2-(24*5)-(16*3);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM16, xpos, ypos, utf, color);

  sleep(1);
if(_DEBUG_)inputKey();

}
