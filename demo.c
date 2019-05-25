#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "ili9340.h"

#define JAPANESE 0
#define _DEBUG_ 0

//When you'd like to wait by a keyboard entries, enable this line.
#define WAIT inputKey()

//When you'd like to wait in the waiting time, enable this line.
//#define WAIT sleep(10)

void inputKey() {
  char ch;
  printf("Hit any key");
  scanf("%c",&ch);
}

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

time_t elapsedTime(struct timeval startTime, struct timeval endTime) {
    time_t diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
    suseconds_t diffsub = endTime.tv_usec - startTime.tv_usec;
//printf("diffsec=%ld diffsub=%ld\n",diffsec, diffsub);
    if(diffsub < 0) {
        diffsec--;
        diffsub = (endTime.tv_usec+1000000) - startTime.tv_usec;
    }
    uint16_t diffmsec = diffsub / 1000;
    time_t diff = (diffsec * 1000) + diffmsec;
    return diff;
}

time_t ColorBarTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t y1 = height/3;
    uint16_t y2 = (height/3)*2;
    lcdDrawFillRect(0, 0, width-1, y1-1, RED);
    lcdDrawFillRect(0, y1-1, width-1, y2-1, GREEN);
    lcdDrawFillRect(0, y2-1, width-1, height-1, BLUE);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t ArrowTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t xpos;
    uint16_t ypos;
    int stlen;

    uint16_t color;
    //lcdFillScreen(WHITE);
    lcdFillScreen(BLACK);
    lcdSetFontDirection(0);
    color = RED;
    uint8_t ascii[10];
    lcdDrawFillArrow(10, 10, 0, 0, 5, color);
    strcpy((char *)ascii, "0,0");
    lcdDrawUTF8String(fx, 0, 20, ascii, color);

    color = GREEN;
    lcdDrawFillArrow(width-11, 10, width-1, 0, 5, color);
    sprintf((char *)ascii, "%d,0",width-1);
    stlen = strlen((char *)ascii);
    xpos = (width-1) - (fontWidth*stlen);
    lcdDrawUTF8String(fx, xpos, 20, ascii, color);

    color = GRAY;
    lcdDrawFillArrow(10, height-11, 0, height-1, 5, color);
    sprintf((char *)ascii, "0,%d",height-1);
    ypos = (height-11) - (fontHeight) - 5;
    lcdDrawUTF8String(fx, 0, ypos, ascii, color);

    color = CYAN;
    lcdDrawFillArrow(width-11, height-11, width-1, height-1, 5, color);
    sprintf((char *)ascii, "%d,%d",width-1, height-1);
    stlen = strlen((char *)ascii);
    xpos = (width-1) - (fontWidth*stlen);
    lcdDrawUTF8String(fx, xpos, ypos, ascii, color);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t DirectionTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    uint8_t ascii[20];

    color = RED;
    strcpy((char *)ascii, "Direction=0");
    lcdSetFontDirection(DIRECTION0);
    lcdDrawUTF8String(fx, 0, height-fontHeight-1, ascii, color);

    color = BLUE;
    strcpy((char *)ascii, "Direction=180");
    lcdSetFontDirection(DIRECTION180);
    lcdDrawUTF8String(fx, width-1, fontHeight-1, ascii, color);

    color = CYAN;
    strcpy((char *)ascii, "Direction=90");
    lcdSetFontDirection(DIRECTION90);
    lcdDrawUTF8String(fx, width-fontHeight-1, height-1, ascii, color);

    color = GREEN;
    strcpy((char *)ascii, "Direction=270");
    lcdSetFontDirection(DIRECTION270);
    lcdDrawUTF8String(fx, fontHeight-1, 0, ascii, color);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}



time_t HorizontalTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    uint8_t ascii[20];

    color = RED;
    strcpy((char *)ascii, "Direction=0");
    lcdSetFontDirection(DIRECTION0);
    uint16_t ypos = height - fontHeight - 1;
    lcdDrawUTF8String(fx, 0, ypos, ascii, color);

    lcdSetFontUnderLine(RED);
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, ascii, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(GREEN);
    ypos = ypos - fontHeight;
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, ascii, color);

    lcdSetFontUnderLine(RED);
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, ascii, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    color = BLUE;
    strcpy((char *)ascii, "Direction=180");
    lcdSetFontDirection(DIRECTION180);
    ypos = fontHeight - 1;
    lcdDrawUTF8String(fx, width-1, ypos, ascii, color);

    lcdSetFontUnderLine(BLUE);
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, ascii, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(YELLOW);
    ypos = ypos + fontHeight;
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, ascii, color);

    lcdSetFontUnderLine(BLUE);
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, ascii, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t VerticalTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    uint8_t ascii[20];

    color = RED;
    strcpy((char *)ascii, "Direction=90");
    lcdSetFontDirection(DIRECTION90);
    uint16_t xpos = width - fontHeight - 1;
    lcdDrawUTF8String(fx, xpos, height-1, ascii, color);

    lcdSetFontUnderLine(RED);
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, ascii, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(GREEN);
    xpos = xpos - fontHeight;
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, ascii, color);

    lcdSetFontUnderLine(RED);
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, ascii, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    color = BLUE;
    strcpy((char *)ascii, "Direction=270");
    lcdSetFontDirection(DIRECTION270);
    xpos = fontHeight -1;
    lcdDrawUTF8String(fx, xpos, 0, ascii, color);

    lcdSetFontUnderLine(BLUE);
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, ascii, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(YELLOW);
    xpos = xpos + fontHeight;
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, ascii, color);

    lcdSetFontUnderLine(BLUE);
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, ascii, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t LineTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(BLACK);
    color = RED;
    uint16_t xpos;
    uint16_t ypos;
    for(ypos=0;ypos<height;ypos=ypos+10) {
        lcdDrawLine(0, ypos, width, ypos, color);
    }

    for(xpos=0;xpos<width;xpos=xpos+10) {
        lcdDrawLine(xpos, 0, xpos, height, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t CircleTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(BLACK);
    color = GRAY;
    uint16_t xpos = width/2;
    uint16_t ypos = height/2;
    int i;
    for(i=5;i<height;i=i+5) {
        lcdDrawCircle(xpos, ypos, i, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t RoundRectTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    uint16_t limit = width;
    if (width > height) limit = height;
    lcdFillScreen(BLACK);
    color = BLUE;
    int i;
    for(i=5;i<limit;i=i+5) {
        if (i > (limit-i-1) ) break;
        lcdDrawRoundRect(i, i, (width-i-1), (height-i-1), 10, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t FillRectTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(CYAN);

    uint16_t red;
    uint16_t green;
    uint16_t blue;
    srand( (unsigned int)time( NULL ) );
    int i;
    for(i=1;i<100;i++) {
        red=rand()%255;
        green=rand()%255;
        blue=rand()%255;
        color=rgb565_conv(red, green, blue);
        uint16_t xpos=rand()%width;
        uint16_t ypos=rand()%height;
        uint16_t size=rand()%(width/5);
        lcdDrawFillRect(xpos, ypos, xpos+size, ypos+size, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t ColorTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(WHITE);
    color = RED;
    uint16_t delta = height/16;
    uint16_t ypos = 0;
    int i;
    for(i=0;i<16;i++) {
        lcdDrawFillRect(0, ypos, width-1, ypos+delta, color);
        color = color >> 1;
        ypos = ypos + delta;
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}



int main(int argc, char **argv)
{
    int i;
    int screenWidth = 0;
    int screenHeight = 0;
    int offsetx = 0;
    int offsety = 0;
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

    lcdInit(screenWidth, screenHeight, offsetx, offsety);
    lcdReset();
    lcdSetup();


    ColorBarTest(screenWidth, screenHeight);
    WAIT;

    if (screenWidth >= 240) { 
        ArrowTest(fx24G, screenWidth, screenHeight);
    } else {
        ArrowTest(fx16G, screenWidth, screenHeight);
    }
    WAIT;

    LineTest(screenWidth, screenHeight);
    WAIT;

    CircleTest(screenWidth, screenHeight);
    WAIT;

    RoundRectTest(screenWidth, screenHeight);
    WAIT;

    if (screenWidth >= 240) {
        DirectionTest(fx24G, screenWidth, screenHeight);
    } else {
        DirectionTest(fx16G, screenWidth, screenHeight);
    }
    WAIT;

    if (screenHeight >= 320) {
        HorizontalTest(fx24G, screenWidth, screenHeight);
    } else {
        HorizontalTest(fx16G, screenWidth, screenHeight);
    }
    WAIT;

    if (screenWidth >= 240) {
        VerticalTest(fx24G, screenWidth, screenHeight);
    } else {
        VerticalTest(fx16G, screenWidth, screenHeight);
    }
    WAIT;

    FillRectTest(screenWidth, screenHeight);
    WAIT;

    ColorTest(screenWidth, screenHeight);
    WAIT;

    // Multi Font Test
    uint16_t color;
    unsigned char utf8[64];;
    uint16_t xpos = 0;
    uint16_t ypos = screenHeight-16-1;
    int margin = 10;
    lcdFillScreen( BLACK);
    color = WHITE;
    lcdSetFontDirection(DIRECTION0);
    int xd = 0;
    int yd = 1;
    if(screenWidth < screenHeight) {
        lcdSetFontDirection(DIRECTION90);
        xpos = screenWidth-16-1;
        xd = 1;
        yd = 0;
    }

    strncpy((char *)utf8, "16Dot Gothic Font", sizeof(utf8));
    if(JAPANESE)strncpy((char *)utf8, "16ドットゴシック", sizeof(utf8));
    lcdDrawUTF8String(fx16G, xpos, ypos, utf8, color);

    xpos = xpos - (24 * xd) - (margin * xd);
    ypos = ypos - (16 * yd) - (margin * yd);
    strncpy((char *)utf8, "24Dot Gothic Font", sizeof(utf8));
    if(JAPANESE)strncpy((char *)utf8, "24ドットゴシック", sizeof(utf8));
    lcdDrawUTF8String(fx24G, xpos, ypos, utf8, color);

    xpos = xpos - (32 * xd) - (margin * xd);
    ypos = ypos - (24 * yd) - (margin * yd);
    if (screenWidth >= 240) {
        strncpy((char *)utf8, "32Dot Gothic Font", sizeof(utf8));
        if(JAPANESE)strncpy((char *)utf8, "32ドットゴシック", sizeof(utf8));
        lcdDrawUTF8String(fx32G, xpos, ypos, utf8, color);
        xpos = xpos - (32 * xd) - (margin * xd);
        ypos = ypos - (32 * yd) - (margin * yd);
    }

    //xpos = xpos - (10 * xd);
    ypos = ypos - (10 * yd);
    strncpy((char *)utf8, "16Dot Mincyo Font", sizeof(utf8));
    if(JAPANESE)strncpy((char *)utf8, "16ドット明朝", sizeof(utf8));
    lcdDrawUTF8String(fx16M, xpos, ypos, utf8, color);

    xpos = xpos - (24 * xd) - (margin * xd);
    ypos = ypos - (16 * yd) - (margin * yd);
    strncpy((char *)utf8, "24Dot Mincyo Font", sizeof(utf8));
    if(JAPANESE)strncpy((char *)utf8, "24ドット明朝", sizeof(utf8));
    lcdDrawUTF8String(fx24M, xpos, ypos, utf8, color);

    if (screenWidth >= 240) {
        xpos = xpos - (32 * xd) - (margin * xd);
        ypos = ypos - (24 * yd) - (margin * yd);
        strncpy((char *)utf8, "32Dot Mincyo Font", sizeof(utf8));
        if(JAPANESE)strncpy((char *)utf8, "32ドット明朝", sizeof(utf8));
        lcdDrawUTF8String(fx32M, xpos, ypos, utf8, color);
    }
    lcdSetFontDirection(0);
    WAIT;

    //lcdDisplayOff();

    return 0;
}

