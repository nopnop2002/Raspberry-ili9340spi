#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <bcm2835.h>
#include <unistd.h>

#include "ili9340.h"
#include "xpt2046.h"

#define T_CS BCM2835_SPI_CS1
#define T_IRQ RPI_V2_GPIO_P1_22
#define _DEBUG_ 0

TouchInfo tinfo;

int ReadTFTConfig(char *path, int *width, int *height, int *offsetx, int *offsety) {
	FILE *fp;
	char buff[128];

	//printf("path=%s\n",path);
	fp = fopen(path,"r");
	if(fp == NULL) return 0;
	while (fgets(buff,128,fp) != NULL) {
		//printf("buf=%s\n",buff);
		//printf("buff[0]=%x\n",buff[0]);
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
	uint16_t xpos;
	uint16_t ypos;
	uint16_t color;
	uint8_t utf[64];
	char dir[128];
	char cpath[128];

	if(_DEBUG_)printf("argv[0]=%s\n",argv[0]);
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
	printf("Your TFT offsetx		is %d.\n",offsetx);
	printf("Your TFT offsety		is %d.\n",offsety);

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

	bcm2835_gpio_fsel(T_IRQ,BCM2835_GPIO_FSEL_INPT);

	// Get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx32G, 0, buffer, &fontWidth, &fontHeight);
	printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

	uint8_t ascii[24];
	int pen_irq;

	// Calibration
	lcdFillScreen(BLACK);
	tinfo._min_xc = 15;
	tinfo._min_yc = 15;
	//lcdDrawFillCircle(10, 10, 10, CYAN);
	lcdDrawFillCircle(tinfo._min_xc, tinfo._min_yc, 10, CYAN);
	strcpy((char *)ascii, "Calibration");
	ypos = ((screenHeight - fontHeight) / 2) - 1;
	xpos = (screenWidth + (strlen((char *)ascii) * fontWidth)) / 2;
	lcdSetFontDirection(DIRECTION180);
	lcdDrawUTF8String(fx32G, xpos, ypos, ascii, WHITE);
	ypos = ypos + fontHeight;
	int _xpos = xpos;
	for(int i=0;i<10;i++) {
		lcdDrawFillCircle(_xpos, ypos, fontWidth/2, RED);
		_xpos = _xpos - fontWidth - 5;
	}

	int16_t xp = INT16_MIN;
	int16_t yp = INT16_MAX;
	int counter = 0;
	while(1) {
		usleep(10000);
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == HIGH) continue;
		int _xp;
		int _yp;
		xptGetxy(T_CS, &_xp, &_yp);
		//printf("counter=%d _xp=%d _yp=%d xp=%d yp=%d\n", counter, _xp, _yp, xp, yp);
		if (_xp > xp) xp = _xp;
		if (_yp < yp) yp = _yp;
		counter++;
		if (counter == 100) break;
		if ((counter % 10) == 0) {
			lcdInit(screenWidth, screenHeight, offsetx, offsety);
			lcdDrawFillCircle(xpos, ypos, fontWidth/2, GREEN);
			xpos = xpos - fontWidth - 5;
		}
	} // end while
	printf("_min_xp=%d _min_yp=%d\n", xp, yp);
	tinfo._min_xp = xp;
	tinfo._min_yp = yp;

	// Clear IRQ
	lcdInit(screenWidth, screenHeight, offsetx, offsety);
	lcdFillScreen(BLACK);
	while(1) {
		usleep(10000);
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == HIGH) break;
	} // end while

	lcdFillScreen(BLACK);
	tinfo._max_xc = screenWidth-10;
	tinfo._max_yc = screenHeight-10;
	//lcdDrawFillCircle(screenWidth-10, screenHeight-10, 10, CYAN);
	lcdDrawFillCircle(tinfo._max_xc, tinfo._max_yc, 10, CYAN);
	strcpy((char *)ascii, "Calibration");
	ypos = ((screenHeight - fontHeight) / 2) - 1;
	xpos = (screenWidth + (strlen((char *)ascii) * fontWidth)) / 2;
	lcdSetFontDirection(DIRECTION180);
	lcdDrawUTF8String(fx32G, xpos, ypos, ascii, WHITE);
	ypos = ypos + fontHeight;
	_xpos = xpos;
	for(int i=0;i<10;i++) {
		lcdDrawFillCircle(_xpos, ypos, fontWidth/2, RED);
		_xpos = _xpos - fontWidth - 5;
	}

	xp = INT16_MAX;
	yp = INT16_MIN;
	counter = 0;
	while(1) {
		usleep(10000);
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == HIGH) continue;
		int _xp;
		int _yp;
		xptGetxy(T_CS, &_xp, &_yp);
		//printf("counter=%d _xp=%d _yp=%d xp=%d yp=%d\n", counter, _xp, _yp, xp, yp);
		if (_xp < xp) xp = _xp;
		if (_yp > yp) yp = _yp;
		counter++;
		if (counter == 100) break;
		if ((counter % 10) == 0) {
			lcdInit(screenWidth, screenHeight, offsetx, offsety);
			lcdDrawFillCircle(xpos, ypos, fontWidth/2, GREEN);
			xpos = xpos - fontWidth - 5;
		}
	} // end while
	printf("_max_xp=%d _max_yp=%d\n", xp, yp);
	tinfo._max_xp = xp;
	tinfo._max_yp = yp;

	// Clear IRQ
	lcdInit(screenWidth, screenHeight, offsetx, offsety);
	lcdFillScreen(BLACK);
	while(1) {
		usleep(10000);
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == HIGH) break;
	} // end while
	tinfo._calibration = false;

	// Draw a button
	lcdFillScreen(WHITE);
	lcdSetFontDirection(DIRECTION90);
	xpos = 180;
	ypos = (screenHeight-1)-(32*1);
	for (i=0;i<5;i++) {
		lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
		xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i);
		xptDump(&tinfo);
		color = BLACK;
		//strcpy(utf,"1");
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
		//strcpy(utf,"1");
		utf[0] = i + 53;
		utf[1] = 0;
		ypos = lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);
		ypos = ypos - 32;
	}


	// Read xpt2046
	xpos = 40;
	ypos = (screenHeight-1)-(32*1);
	color = RED;
	while(1) {
		usleep(10000);			/* do it anyway ; settle time when pen goes up */
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == LOW) { /* P1.22 == PenIRQ is LOW : touch! pen is down */
			int id = xptGetPoint(T_CS, &tinfo);
			if (id != -1) {
			if(_DEBUG_)printf("id=%d\n",id);
				lcdInit(screenWidth, screenHeight, offsetx, offsety);
				lcdSetFontDirection(DIRECTION90);
				utf[0] = id + 48;
				ypos = lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);
			}
		}
	} // end while

}
