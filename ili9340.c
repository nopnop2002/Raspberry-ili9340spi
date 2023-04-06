// ili9340.c
// Used by bcm2835 library.(Mike McCauley)
//
// Original is http://imagewriteriij.blogspot.jp/2014/01/raspberry-pi-9-lcd-1.html
// LCD test program 20130103 by ImageWriter
//
// [Pin connection]
// ILI9340-SPI     Rpi(pin)
// ------------------------
// VCC-------------3.3V
// GND-------------GND
// CS--------------CS0 (24)
// RES-------------IO18(12)
// D/C-------------IO17(11) LOW = COMMAND / HIGH = DATA
// MOSI------------MOSI(19)
// SCK-------------SCLK(23)
// LED-------------3.3V
// MISO------------N/C
// ------------------------
//  
// [SPI settings]
// ORDER  : MSB First
// MODE   : Mode0
// CLOCK  : 31.25MHz on Rpi/Rpi2, 50MHz on RPI3 
// CS     : CS0
// CS_POL : LOW
//
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef WPI
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringShift.h>

#ifdef GPIO
#define D_C  17 // BCM IO17=Pin#11
#define RES  18 // BCM IO18=Pin#12
#define MOSI 10 // BCM IO10=Pin#19
#define SCLK 11 // BCM IO11=Pin#23
#define CS   24 // BCM IO24=Pin#24
#else
#define D_C   0 // wPi IO00=Pin#11
#define RES   1 // wPi IO01=Pin#12
#define MOSI 12 // wPi IO12=Pin#19
#define SCLK 14 // wPi IO14=Pin#23
#define CS   10 // wPi IO10=Pin#24
#endif
#endif // end WPI


#ifdef BCM
#include <bcm2835.h>
#define D_C 17  // BCM IO17=Pin#11
#define RES 18  // BCM IO18=Pin#12
#endif

#include "ili9340.h"

#define _DEBUG_   0

uint16_t _FONT_DIRECTION_;
uint16_t _FONT_FILL_;
uint16_t _FONT_FILL_COLOR_;
uint16_t _FONT_UNDER_LINE_;
uint16_t _FONT_UNDER_LINE_COLOR_;

int _width;
int _height;
int _offsetx;
int _offsety;

#ifdef WPI
// Write Command 8Bit
// D/C=LOW then,write command(8bit)
void lcdWriteCommandByte(uint8_t c){
  digitalWrite(D_C, LOW);
#ifdef SOFT_SPI
  digitalWrite(CS, LOW);
  shiftOut(MOSI, SCLK, MSBFIRST, c);
  digitalWrite(CS, HIGH);
#else
  wiringPiSPIDataRW(0, &c, 1);
#endif
}

// Write Data 8Bit
// D/C=HIGH then,write data(8bit)
void lcdWriteDataByte(uint8_t c){
  digitalWrite(D_C, HIGH);
#ifdef SOFT_SPI
  digitalWrite(CS, LOW);
  shiftOut(MOSI, SCLK, MSBFIRST, c);
  digitalWrite(CS, HIGH);
#else
  wiringPiSPIDataRW(0, &c, 1);
#endif
}

// Write Data 16Bit
void lcdWriteDataWord(uint16_t w){
  uint8_t hi,lo;
  hi = (w >> 8) & 0xFF;
  lo = w & 0xFF;
  lcdWriteDataByte(hi);
  lcdWriteDataByte(lo);
}

// Write Tow Data 8Bit
void lcdWriteAddr(uint8_t addr1, uint8_t addr2){
  uint8_t byte[4];
  byte[0] = (addr1 >> 8) & 0xFF;
  byte[1] = addr1 & 0xFF;
  byte[2] = (addr2 >> 8) & 0xFF;
  byte[3] = addr2 & 0xFF;
  digitalWrite(D_C, HIGH);
#ifdef SOFT_SPI
  digitalWrite(CS, LOW);
  for(int index=0;index<4;index++) {
    shiftOut(MOSI, SCLK, MSBFIRST, byte[index]);
  }
  digitalWrite(CS, HIGH);
#else
  wiringPiSPIDataRW(0, byte, 4);
#endif
}

// Write Data 16Bit
void lcdWriteColor(uint16_t color, uint16_t size) {
  uint8_t byte[1024];
  int index=0;
  int i;
  for(i=0;i<size;i++) {
    byte[index++] = (color >> 8) & 0xFF;
    byte[index++] = color & 0xFF;
  }
  digitalWrite(D_C, HIGH);
#ifdef SOFT_SPI
  digitalWrite(CS, LOW);
  for(int index=0;index<size*2;index++) {
    shiftOut(MOSI, SCLK, MSBFIRST, byte[index]);
  }
  digitalWrite(CS, HIGH);
#else
  wiringPiSPIDataRW(0, byte, size*2);
#endif
}
#endif // end WPI

#ifdef BCM
// Write Command
// D/C=LOW then,write command(8bit)
void lcdWriteCommandByte(uint8_t c){
  bcm2835_gpio_write(D_C, LOW);
  bcm2835_spi_transfer(c);
}

// Write Data 8Bit
// D/C=HIGH then,write data(8bit)
void lcdWriteDataByte(uint8_t c){
  bcm2835_gpio_write(D_C, HIGH);
  bcm2835_spi_transfer(c);
}

// Write Data 16Bit
void lcdWriteDataWord(uint16_t w){
  bcm2835_gpio_write(D_C, HIGH);
  bcm2835_spi_write(w);
}

// Write Tow Data 8Bit
void lcdWriteAddr(uint8_t addr1, uint8_t addr2){
  bcm2835_gpio_write(D_C, HIGH);
  bcm2835_spi_write(addr1);
  bcm2835_spi_write(addr2);
}

// Write Data 16Bit
void lcdWriteColor(uint16_t color, uint16_t size) {
  bcm2835_gpio_write(D_C, HIGH);
  int i;
  for(i=0;i<size;i++) bcm2835_spi_write(color);
}
#endif // end BCM


#ifdef WPI
// SPI interfase initialize
// MSB,mode0,clock=8,cs0=low
void lcdInit(int width, int height, int offsetx, int offsety){
  _width = width;
  _height = height;
  _offsetx = offsetx;
  _offsety = offsety;

#ifdef GPIO
  printf("Using wiringPiSetupGpio\n");
  if (wiringPiSetupGpio() == -1) {
    printf("wiringPiSetup Error\n");
    return;
  }
#else
  printf("Using wiringPiSetup\n");
  if (wiringPiSetup() == -1) {
    printf("wiringPiSetup Error\n");
    return;
  }
#endif // end GPIO


#ifdef SOFT_SPI
  pinMode(CS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCLK, OUTPUT);
  printf("Using SoftWare SPI\n");
#else

#if defined SPI_SPEED32
  int spi_speed = 32000000;
#elif defined SPI_SPEED16
  int spi_speed = 16000000;
#else
  int spi_speed = 8000000;
#endif
  printf("Using HardWare SPI. SPI SPEED %dMHz\n", spi_speed/1000000);

#ifdef SPI1
  printf("Using SPI Channel 1\n");
  //wiringPiSPISetup(1, 16000000);
  wiringPiSPISetup(1, spi_speed);
#else
  printf("Using SPI Channel 0\n");
  wiringPiSPISetup(0, 16000000);
  wiringPiSPISetup(0, spi_speed);
#endif
#endif // end SOFT_SPI
  //wiringPiSPISetup(0, 32000000);

  _FONT_DIRECTION_ = DIRECTION0;
  _FONT_FILL_ = false;
  _FONT_UNDER_LINE_ = false;

}
void lcdReset(void){
  pinMode(D_C, OUTPUT);
  pinMode(RES, OUTPUT);
  digitalWrite(D_C, HIGH);

  digitalWrite(RES, HIGH);
  delay(100);
  digitalWrite(RES, LOW);
  delay(100);
  digitalWrite(RES, HIGH);
  delay(100);
}
#endif

#ifdef BCM
// SPI interfase initialize
// MSB,mode0,clock=8,cs0=low
void lcdInit(int width, int height, int offsetx, int offsety){
  _width = width;
  _height = height;
  _offsetx = offsetx;
  _offsety = offsety;
  if (bcm2835_init() == -1) {
    printf("bmc2835_init Error\n");
    return;
  }

  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
#if defined SPI_SPEED32
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8); // 31.25MHz on Rpi2, 50MHz on RPI3
  printf("Using BCM. SPI SPEED 31.25MHz on Rpi2, 50MHz on RPI3\n");
#elif defined SPI_SPEED16
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16); // 15.625MHz on Rpi2, 25MHz on RPI3
  printf("Using BCM. SPI SPEED 15.625MHz on Rpi2, 25MHz on RPI3\n");
#else
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); // 7.8125MHz on Rpi2, 12.5MHz on RPI3
  printf("Using BCM. SPI SPEED 7.8125MHz on Rpi2, 12.5MHz on RPI3\n");
#endif

#ifdef SPI1
  printf("Using SPI Channel 1\n");
  bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
#else
  printf("Using SPI Channel 0\n");
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
#endif
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
  // Send a byte to the slave and simultaneously read a byte back from the slave
  // If you tie MISO to MOSI, you should read back what was sent

  _FONT_DIRECTION_ = DIRECTION0;
  _FONT_FILL_ = false;
  _FONT_UNDER_LINE_ = false;

}

// TFT Reset
void lcdReset(void){
  bcm2835_gpio_fsel(D_C,BCM2835_GPIO_FSEL_OUTP); // D/C
  bcm2835_gpio_fsel(RES,BCM2835_GPIO_FSEL_OUTP); // Reset
  bcm2835_gpio_write(D_C, HIGH);

  bcm2835_gpio_write(RES, HIGH);
  bcm2835_delay(100); 
  bcm2835_gpio_write(RES, LOW);
  bcm2835_delay(100);
  bcm2835_gpio_write(RES, HIGH);
  bcm2835_delay(100); 
}
#endif // end BCM

// TFT initialize
void lcdSetup(void){
  lcdWriteCommandByte(0xC0);		//Power Control 1
  lcdWriteDataByte(0x23); 

  lcdWriteCommandByte(0xC1);		//Power Control 2
  lcdWriteDataByte(0x10);

  lcdWriteCommandByte(0xC5);		//VCOM Control 1
  lcdWriteDataByte(0x3e);
  lcdWriteDataByte(0x28); 

  lcdWriteCommandByte(0xC7);		//VCOM Control 2 
  lcdWriteDataByte(0x86);

  lcdWriteCommandByte(0x36);		//Memory Access Control 
  lcdWriteDataByte(0x48);		//Left bottom start

  lcdWriteCommandByte(0x3A);		//Pixel Format Set
  lcdWriteDataByte(0x55);		//65K color: 16-bit/pixel

  lcdWriteCommandByte(0x20);		//Display Inversion OFF

  lcdWriteCommandByte(0xB1);		//Frame Rate Control
  lcdWriteDataByte(0x00);  
  lcdWriteDataByte(0x18); 

  lcdWriteCommandByte(0xB6);		//Display Function Control 
  lcdWriteDataByte(0x08); 
  lcdWriteDataByte(0xA2);
  lcdWriteDataByte(0x27);  
  lcdWriteDataByte(0x00);  

  lcdWriteCommandByte(0xF2);		//3Gamma Function Disable 
  lcdWriteDataByte(0x00); 

  lcdWriteCommandByte(0x26);		//Gamma Set 
  lcdWriteDataByte(0x01); 

  lcdWriteCommandByte(0xE0);		//Positive Gamma Correction
  lcdWriteDataByte(0x0F); 
  lcdWriteDataByte(0x31); 
  lcdWriteDataByte(0x2B); 
  lcdWriteDataByte(0x0C); 
  lcdWriteDataByte(0x0E); 
  lcdWriteDataByte(0x08); 
  lcdWriteDataByte(0x4E); 
  lcdWriteDataByte(0xF1); 
  lcdWriteDataByte(0x37); 
  lcdWriteDataByte(0x07); 
  lcdWriteDataByte(0x10); 
  lcdWriteDataByte(0x03); 
  lcdWriteDataByte(0x0E); 
  lcdWriteDataByte(0x09); 
  lcdWriteDataByte(0x00); 

  lcdWriteCommandByte(0XE1);		//Negative Gamma Correction
  lcdWriteDataByte(0x00); 
  lcdWriteDataByte(0x0E); 
  lcdWriteDataByte(0x14); 
  lcdWriteDataByte(0x03); 
  lcdWriteDataByte(0x11); 
  lcdWriteDataByte(0x07); 
  lcdWriteDataByte(0x31); 
  lcdWriteDataByte(0xC1); 
  lcdWriteDataByte(0x48); 
  lcdWriteDataByte(0x08); 
  lcdWriteDataByte(0x0F); 
  lcdWriteDataByte(0x0C); 
  lcdWriteDataByte(0x31); 
  lcdWriteDataByte(0x36); 
  lcdWriteDataByte(0x0F); 

  lcdWriteCommandByte(0x11);			//Sleep Out 
#ifdef BCM
  bcm2835_delay(200); 
#endif
#ifdef WPI
  delay(200);
#endif
  
  lcdWriteCommandByte(0x29);			//Display ON 
}

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color){
  if (x >= _width) return;
  if (y >= _height) return;

  uint16_t _x = x + _offsetx;
  uint16_t _y = y + _offsety;
  lcdWriteCommandByte(0x2A); // set column(x) address
  lcdWriteDataWord(_x);
  lcdWriteDataWord(_x);
  lcdWriteCommandByte(0x2B); // set Page(y) address
  lcdWriteDataWord(_y);
  lcdWriteDataWord(_y);
  lcdWriteCommandByte(0x2C); // Memory Write
  lcdWriteDataWord(color);
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  int i,j; 
  if (x1 >= _width) return;
  if (x2 >= _width) x2=_width-1;
  if (y1 >= _height) return;
  if (y2 >= _height) y2=_height-1;

  uint16_t _x1 = x1 + _offsetx;
  uint16_t _x2 = x2 + _offsetx;
  uint16_t _y1 = y1 + _offsety;
  uint16_t _y2 = y2 + _offsety;
  lcdWriteCommandByte(0x2A); // set column(x) address
  lcdWriteDataWord(_x1);
  lcdWriteDataWord(_x2);
  //lcdWriteAddr(_x1, _x2); // Don't work 
  lcdWriteCommandByte(0x2B); // set Page(y) address
  lcdWriteDataWord(_y1);
  lcdWriteDataWord(_y2);
  //lcdWriteAddr(_y1, _y2); // Don't work 
  lcdWriteCommandByte(0x2C); // Memory Write
  for(i=x1;i<=x2;i++){
    uint16_t size = y2-y1+1;
    lcdWriteColor(color, size);
#if 0
    for(j=y1;j<=y2;j++){
      lcdWriteDataWord(color);
    }
#endif
  }
}

// Display Off
void lcdDisplayOff(void) {
  lcdWriteCommandByte(0x28);    //Display OFF
}
 
// Display On
void lcdDisplayOn(void) {
  lcdWriteCommandByte(0x29);    //Display ON 
}

// Display Inversion On
void lcdInversionOn(void) {
  lcdWriteCommandByte(0x21);    //Display Inversion ON 
}

// Fill screen
// color:color
void lcdFillScreen(uint16_t color) {
  lcdDrawFillRect(0, 0, _width-1, _height-1, color);
}

// Draw line
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color 
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  int i;
  int dx,dy;
  int sx,sy;
  int E;

  /* distance between two points */
  dx = ( x2 > x1 ) ? x2 - x1 : x1 - x2;
  dy = ( y2 > y1 ) ? y2 - y1 : y1 - y2;

  /* direction of two point */
  sx = ( x2 > x1 ) ? 1 : -1;
  sy = ( y2 > y1 ) ? 1 : -1;

  /* inclination < 1 */
  if ( dx > dy ) {
    E = -dx;
    for ( i = 0 ; i <= dx ; i++ ) {
      lcdDrawPixel( x1, y1, color );
      x1 += sx;
      E += 2 * dy;
      if ( E >= 0 ) {
        y1 += sy;
        E -= 2 * dx;
      }
    }
  /* inclination >= 1 */
  } else {
    E = -dy;
    for ( i = 0 ; i <= dy ; i++ ) {
      lcdDrawPixel( x1, y1, color );
      y1 += sy;
      E += 2 * dx;
      if ( E >= 0 ) {
        x1 += sx;
        E -= 2 * dy;
      }
    }
  }
}

// Draw rectangule
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// color:color
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  lcdDrawLine(x1,y1,x2,y1,color);
  lcdDrawLine(x2,y1,x2,y2,color);
  lcdDrawLine(x2,y2,x1,y2,color);
  lcdDrawLine(x1,y2,x1,y1,color);
}

// Draw round
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
  int x;
  int y;
  int err;
  int old_err;
  
  x=0;
  y=-r;
  err=2-2*r;
  do{
    lcdDrawPixel(x0-x,y0+y,color); 
    lcdDrawPixel(x0-y,y0-x,color); 
    lcdDrawPixel(x0+x,y0-y,color); 
    lcdDrawPixel(x0+y,y0+x,color); 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
  
}

// Draw round of filling
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
  int x;
  int y;
  int err;
  int old_err;
  int ChangeX;
  
  x=0;
  y=-r;
  err=2-2*r;
  ChangeX=1;
  do{
    if(ChangeX) {
      lcdDrawLine(x0-x,y0-y,x0-x,y0+y,color);
      lcdDrawLine(x0+x,y0-y,x0+x,y0+y,color);
    } // if
    ChangeX=(old_err=err)<=x;
    if (ChangeX)            err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;
  } while(y<=0);
    
} 

// Draw rectangule with round corner
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// r:radius
// color:color
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color) {
  int x;
  int y;
  int err;
  int old_err;
  unsigned char temp;

  if(x1>x2) {
    temp=x1; x1=x2; x2=temp;
  }
  
  if(y1>y2) {
    temp=y1; y1=y2; y2=temp;
  }
  if (x2 - x1 < r) return; // Add 20190517
  if (y2 - y1 < r) return; // Add 20190517
  
  x=0;
  y=-r;
  err=2-2*r;

  do{
    if(x) {
      lcdDrawPixel(x1+r-x,y1+r+y,color); 
      lcdDrawPixel(x2-r+x,y1+r+y,color); 
      lcdDrawPixel(x1+r-x,y2-r-y,color); 
      lcdDrawPixel(x2-r+x,y2-r-y,color);
    } // if 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
   
  lcdDrawLine(x1+r,y1  ,x2-r,y1  ,color);
  lcdDrawLine(x1+r,y2  ,x2-r,y2  ,color);
  lcdDrawLine(x1  ,y1+r,x1  ,y2-r,color);
  lcdDrawLine(x2  ,y1+r,x2  ,y2-r,color);  
} 

// Draw arrow
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
// Thanks http://k-hiura.cocolog-nifty.com/blog/2010/11/post-2a62.html
void lcdDrawArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color) {

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//   printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//   printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

//   lcdDrawLine(x0,y0,x1,y1,color);
   lcdDrawLine(x1,y1,L[0],L[1],color);
   lcdDrawLine(x1,y1,R[0],R[1],color);
   lcdDrawLine(L[0],L[1],R[0],R[1],color);
}


// Draw arrow of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
void lcdDrawFillArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color) {

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//   printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//   printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

   lcdDrawLine(x0,y0,x1,y1,color);
   lcdDrawLine(x1,y1,L[0],L[1],color);
   lcdDrawLine(x1,y1,R[0],R[1],color);
   lcdDrawLine(L[0],L[1],R[0],R[1],color);

   int ww;
   for(ww=w-1;ww>0;ww--) {
     L[0]= x1 - Uy*ww - Ux*v;
     L[1]= y1 + Ux*ww - Uy*v;
     R[0]= x1 + Uy*ww - Ux*v;
     R[1]= y1 - Ux*ww - Uy*v;
//     printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
     lcdDrawLine(x1,y1,L[0],L[1],color);
     lcdDrawLine(x1,y1,R[0],R[1],color);
   }
}


// RGB565 conversion
// RGB565 is R(5)+G(6)+B(5)=16bit color format.
// Bit image "RRRRRGGGGGGBBBBB"
uint16_t rgb565_conv(uint16_t r,uint16_t g,uint16_t b) {
	return (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}


// Draw SJIS character
// x:X coordinate
// y:Y coordinate
// sjis: SJIS code
// color:color
int lcdDrawSJISChar(FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color) {
  uint16_t xx,yy,bit,ofs;
  unsigned char fonts[128]; // font pattern
  unsigned char pw, ph;
  int h,w;
  uint16_t mask;
  bool rc;
  
if(_DEBUG_)printf("_FONT_DIRECTION_=%d\n",_FONT_DIRECTION_);
//  sjis = UTF2SJIS(utf8);
//if(_DEBUG_)printf("sjis=%04x\n",sjis);

  rc = GetFontx(fx, sjis, fonts, &pw, &ph); // SJIS -> Font pattern
if(_DEBUG_)printf("GetFontx rc=%d pw=%d ph=%d\n",rc,pw,ph);
  if (!rc) return 0;

  uint16_t xd1, yd1;
  uint16_t xd2, yd2;
  uint16_t xss, yss;
  uint16_t xsd, ysd;
  int next;
  uint16_t x0 = 0;
  uint16_t x1 = 0;
  uint16_t y0 = 0;
  uint16_t y1 = 0;
  if (_FONT_DIRECTION_ == 0) {
    xd1 = +1;
    yd1 = -1;
    xd2 =  0;
    yd2 =  0;
    xss =  x;
    yss =  y + (ph - 1);
    xsd =  1;
    ysd =  0;
    next = x + pw;

    x0 = x;
    y0 = y;
    x1 = x + (pw-1);
    y1 = y + (ph-1);
  } else if (_FONT_DIRECTION_ == 2) {
    xd1 = -1;
    yd1 = +1;
    xd2 =  0;
    yd2 =  0;
    xss =  x;
    yss =  y - (ph + 1);
    xsd =  1;
    ysd =  0;
    next = x - pw;

    x0 = x - (pw-1);
    y0 = y - (ph-1);
    x1 = x;
    y1 = y;
  } else if (_FONT_DIRECTION_ == 1) {
    xd1 =  0;
    yd1 =  0;
    xd2 = -1;
    yd2 = -1;
    xss =  x + (ph - 1); // Bug Fix
    yss =  y;
    xsd =  0;
    ysd =  1;
    next = y - pw;

    x0 = x;
    y0 = y - (pw-1);
    x1 = x + (ph-1);
    y1 = y;
  } else if (_FONT_DIRECTION_ == 3) {
    xd1 =  0;
    yd1 =  0;
    xd2 = +1;
    yd2 = +1;
    xss =  x - (ph - 1); // Bug Fix
    yss =  y;
    xsd =  0;
    ysd =  1;
    next = y + pw;

    x0 = x - (ph-1);
    y0 = y;
    x1 = x;
    y1 = y + (pw-1);
  }
  if (_FONT_FILL_) lcdDrawFillRect(x0, y0, x1, y1, _FONT_FILL_COLOR_);

  int bits;
if(_DEBUG_)printf("xss=%d yss=%d\n",xss,yss);
  ofs = 0;
  yy = yss;
  xx = xss;
  for(h=0;h<ph;h++) {
    if(xsd) xx = xss;
    if(ysd) yy = yss;
//    for(w=0;w<(pw/8);w++) {
    bits = pw;
    for(w=0;w<((pw+4)/8);w++) {
      mask = 0x80;
      for(bit=0;bit<8;bit++) {
        bits--;
        if (bits < 0) continue;
//if(_DEBUG_)printf("xx=%d yy=%d mask=%02x fonts[%d]=%02x\n",xx,yy,mask,ofs,fonts[ofs]);
        if (fonts[ofs] & mask) {
          lcdDrawPixel(xx,yy,color);
        } else {
          //if (_FONT_FILL_) lcdDrawPixel(xx,yy,_FONT_FILL_COLOR_);
        }
        if (h == (ph-2) && _FONT_UNDER_LINE_)
          lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);
        if (h == (ph-1) && _FONT_UNDER_LINE_)
          lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);

        xx = xx + xd1;
        yy = yy + yd2;
        mask = mask >> 1;
      }
      ofs++;
    }
    yy = yy + yd1;
    xx = xx + xd2;

  }
  
  if (next < 0) next = 0;
  return next;
}

// Draw UTF8 character
// x:X coordinate
// y:Y coordinate
// utf8: UTF8 code
// color:color
int lcdDrawUTF8Char(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color) {
  uint16_t sjis[1];
  
  sjis[0] = UTF2SJIS(utf8);
if(_DEBUG_)printf("sjis=%04x\n",sjis[0]);
  return lcdDrawSJISChar(fx,x,y,sjis[0],color);
}

// Draw UTF8 string
// x:X coordinate
// y:Y coordinate
// utfs: UTF8 string
// color:color
int lcdDrawUTF8String(FontxFile *fx, uint16_t x,uint16_t y,unsigned char *utfs,uint16_t color) {

if(_DEBUG_)printf("lcdDrawUTF8String start x=%d y=%d\n",x,y);
  int i;
  int spos;
  uint16_t sjis[64];
  spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 64);
if(_DEBUG_)printf("spos=%d\n",spos);
  for(i=0;i<spos;i++) {
if(_DEBUG_)printf("sjis[%d]=%x y=%d\n",i,sjis[i],y);
    if (_FONT_DIRECTION_ == 0)
      x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 1)
      y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 2)
      x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 3)
      y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
  }
if(_DEBUG_)printf("lcdDrawUTF8String end x=%d y=%d\n",x,y);
  if (_FONT_DIRECTION_ == 0) return x;
  if (_FONT_DIRECTION_ == 2) return x;
  if (_FONT_DIRECTION_ == 1) return y;
  if (_FONT_DIRECTION_ == 3) return y;
  return 0;
}

// Set font direction
// dir:Direction
void lcdSetFontDirection(uint16_t dir) {
  _FONT_DIRECTION_ = dir;
}

// Set font filling
// color:fill color
void lcdSetFontFill(uint16_t color) {
  _FONT_FILL_ = true;
  _FONT_FILL_COLOR_ = color;
}

// UnSet font filling
void lcdUnsetFontFill(void) {
  _FONT_FILL_ = false;
}

// Set font underline
// color:frame color
void lcdSetFontUnderLine(uint16_t color) {
  _FONT_UNDER_LINE_ = true;
  _FONT_UNDER_LINE_COLOR_ = color;
}

// UnSet font filling
void lcdUnsetFontUnderLine(void) {
  _FONT_UNDER_LINE_ = false;
}

