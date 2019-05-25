#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <bcm2835.h>
#include <unistd.h>

#include "ili9340.h"
#include "xpt2046.h"

//#define T_CS BCM2835_SPI_CS0
#define T_CS BCM2835_SPI_CS1
#define _DEBUG_ 0

TouchInfo tinfo;


int ReadTFTConfig(char *path, int *width, int *height, int *offsetx, int *offsety) {
  FILE *fp;
  char buff[128];

//  printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
//    printf("buf=%s\n",buff);
//    printf("buff[0]=%x\n",buff[0]);
    if (buff[0] == '#') continue;
    if (buff[0] == 0x0a) continue;
    if (strncmp(buff, "width=", 6) == 0) {
      sscanf(buff, "width=%d height=%d",width,height);
if(_DEBUG_)printf("width=%d height=%d\n",*width,*height);
    } else if (strncmp(buff, "offsetx=", 8) == 0) {
      sscanf(buff, "offsetx=%d",offsetx);
if(_DEBUG_)printf("offsetx=%d\n",*offsetx);
    } else if (strncmp(buff, "offsety=", 8) == 0) {
      sscanf(buff, "offsety=%d",offsety);
if(_DEBUG_)printf("offsety=%d\n",*offsety);
    }
  }
  fclose(fp);
  return 1;
}

int main(int argc, char **argv)
{
  int i;
  int screenWidth = 0;
  int screenHeight = 0;
  int offsetx = 0;
  int offsety = 0;
  uint16_t xpos,ypos;
  uint16_t color;
  uint8_t utf[64];
  char dir[128];
  char cpath[128];

if(_DEBUG_)  printf("argv[0]=%s\n",argv[0]);
  strcpy(dir, argv[0]);
  for(i=strlen(dir);i>0;i--) {
    if (dir[i-1] == '/') {
      dir[i] = 0;
      break;
    } // end if
  } // end for
if(_DEBUG_)printf("dir=%s\n",dir);
  strcpy(cpath,dir);
  strcat(cpath,"tft.conf");
if(_DEBUG_)printf("cpath=%s\n",cpath);
  if (ReadTFTConfig(cpath, &screenWidth, &screenHeight, &offsetx, &offsety) == 0) {
    printf("%s Not found\n",cpath);
    return 0;
  }
if(_DEBUG_)printf("ReadTFTConfig:screenWidth=%d height=%d\n",screenWidth, screenHeight);
  printf("Your TFT resolution is %d x %d.\n",screenWidth, screenHeight);
  printf("Your TFT offsetx    is %d.\n",offsetx);
  printf("Your TFT offsety    is %d.\n",offsety);

#if 0
  if (bcm2835_init() == -1) {
    printf("bmc2835_init Error\n");
    return 1;
  }
#endif

  // You can change font file
  FontxFile fx32G[2];
  FontxFile fx24G[2];
  FontxFile fx16G[2];
  Fontx_init(fx32G,"./fontx/ILGH32XB.FNT","./fontx/ILGZ32XB.FNT"); // 16x32Dot Gothic
  Fontx_init(fx24G,"./fontx/ILGH24XB.FNT","./fontx/ILGZ24XB.FNT"); // 12x24Dot Gothic
  Fontx_init(fx16G,"./fontx/ILGH16XB.FNT","./fontx/ILGZ16XB.FNT"); // 8x16Dot Gothic

  FontxFile fx32M[2];
  FontxFile fx24M[2];
  FontxFile fx16M[2];
  Fontx_init(fx32M,"./fontx/ILMH32XF.FNT","./fontx/ILMZ32XF.FNT"); // 16x32Dot Mincyo
  Fontx_init(fx24M,"./fontx/ILMH24XF.FNT","./fontx/ILMZ24XF.FNT"); // 12x24Dot Mincyo
  Fontx_init(fx16M,"./fontx/ILMH16XB.FNT","./fontx/ILMZ16XF.FNT"); // 8x16Dot Mincyo

  xptInit(&tinfo);

  lcdInit(screenWidth, screenHeight, offsetx, offsety);
  lcdReset();
  lcdSetup();

  //drawString
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);
  xpos = 180;
  ypos = (screenHeight-1)-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i);
    xptDump(&tinfo);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 48;
    utf[1] = 0;
    ypos = lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }

  xpos = 120;
  ypos = (screenHeight-1)-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i+5);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 53;
    utf[1] = 0;
    ypos = lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }


  // read xpt2046
  int pen_irq;
  int id;

  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22,BCM2835_GPIO_FSEL_INPT);

  xpos = 40;
  ypos = (screenHeight-1)-(32*1);
  color = RED;
  for (;;) {
    usleep(10000);      /* do it anyway ; settle time when pen goes up */
    pen_irq = bcm2835_gpio_lev(RPI_V2_GPIO_P1_22);
    if (pen_irq == LOW) { /* P1.22 == PenIRQ is LOW : touch! pen is down */
      id = xptGetPoint(T_CS, &tinfo);
      if (id != -1) {
if(_DEBUG_)printf("id=%d\n",id);
  	lcdInit(screenWidth, screenHeight, offsetx, offsety);
        lcdSetFontDirection(DIRECTION90);
        utf[0] = id + 48;
        ypos = lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);
      }
    }
  }

}
