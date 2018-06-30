#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <bcm2835.h>
#include <unistd.h>
#include "ili9340.h"
#include "xpt2046.h"

#define T_CS BCM2835_SPI_CS0
//#define T_CS BCM2835_SPI_CS1

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];
FontxFile fxG16[2];
FontxFile fxM16[2];

TouchInfo tinfo;

int main()
{
  int i,j;
  uint16_t xpos,ypos;
  uint16_t color;
  uint8_t utf[64];

  int XMAX,YMAX;
  int XMAX2,YMAX2;

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

  xptInit(&tinfo);

  XMAX = 240;
  YMAX = 320;
  XMAX2 = XMAX - 1;
  YMAX2 = YMAX - 1;
  lcdInit(XMAX,YMAX);
  lcdReset();
  lcdSetup();

  //drawString
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);
  xpos = 180;
  ypos = YMAX2-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i);
    xptDump(&tinfo);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 48;
    ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }

  xpos = 120;
  ypos = YMAX2-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i+5);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 53;
    ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }


  // read xpt2046
  int x, y;
  int pen_irq;
  int id;

  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22,BCM2835_GPIO_FSEL_INPT);

  xpos = 40;
  ypos = YMAX2-(32*1);
  color = RED;
  for (;;) {
    usleep(10000);      /* do it anyway ; settle time when pen goes up */
    pen_irq = bcm2835_gpio_lev(RPI_V2_GPIO_P1_22);
    if (pen_irq == LOW) { /* P1.22 == PenIRQ is LOW : touch! pen is down */
      id = xptGetPoint(T_CS, &tinfo);
      if (id != -1) {
//        printf("id=%d\n",id);
        lcdInit(XMAX,YMAX);
        lcdSetFontDirection(DIRECTION90);
        utf[0] = id + 48;
        ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
      }
    }
  }

}
