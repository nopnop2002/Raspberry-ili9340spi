# ili9340spi_rpi
ILI9340 SPI TFT Library for Raspberry Pi/Orange Pi

You can show a chart to ILI9340/ILI9341/ILI9163C/ST7735 SPI TFT.   
You can choose bmc2835 library/WiringPi(WiringOp) library.   

I tested these TFT.   
1.44 inch 128x128 ST7735   
1.44 inch 128x128 ILI9163C   
1.8 inch 128x160 ST7735   
2.2 inch 240x320 ILI9340   
2.4 inch 240x320 ILI9341   
2.4 inch 240x320 ILI9341   

----

# Wirering   

|TFT||Rpi/Opi|
|:-:|:-:|:-:|
|MISO|--|N/C|
|LED|--|3.3V|
|SCK|--|Pin#23(SPI SCLK)|
|MOSI|--|Pin#19(SPI MOSI)|
|D/C|--|Pin#3(*)|
|RES|--|Pin#5(*)|
|CS|--|Pin#24(SPI CE0)|
|GND|--|GND|
|VCC|--|3.3V|

(*) You can change any pin.   

Note:   
Opi have only 1 SPI.   
OPi-PC have CE0 and GPIO8.  
OPi ZERO have CE1 and GPIO8.   

----

# Build using bcm2835 library, RPi Only, Very fast   

wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.42.tar.gz   
tar zxvf bcm2835-1.42.tar.gz   
cd bcm2835-1.42   
./configure   
make   
sudo make check   
sudo make install   
cd $HOME   
git clone https://github.com/nopnop2002/ili9340spi_rpi   
cd ili9340spi_rpi   
cc -o demo demo.c fontx.c ili9340.c -lbcm2835 -lm -DBCM   

----

# Build using WiringPi/WiringOp library, Both of RPi/OPi, Slow...   

git clone https://github.com/nopnop2002/ili9340spi_rpi   
cd ili9340spi_rpi   
cc -o demo demo.c fontx.c ili9340.c -lwiringPi -lm -DWPI   

----

![ili9340-1](https://cloud.githubusercontent.com/assets/6020549/25006015/3dd4f15c-2096-11e7-82db-d444ee689cb0.JPG)

![ili9340-2](https://cloud.githubusercontent.com/assets/6020549/25006026/4d36b842-2096-11e7-9030-1d7455a23a53.JPG)

![ili9340-3](https://cloud.githubusercontent.com/assets/6020549/25006042/5c2b1992-2096-11e7-9ee8-e1a4fe2b1b03.JPG)

![ili9340-4](https://cloud.githubusercontent.com/assets/6020549/25006061/6ccffa24-2096-11e7-90bf-caebc15cf942.JPG)

![ili9340-5](https://cloud.githubusercontent.com/assets/6020549/25006078/80c4b5d8-2096-11e7-8c88-650f8df1699f.JPG)

![ili9340-6](https://cloud.githubusercontent.com/assets/6020549/25006148/b546b11c-2096-11e7-83ad-7352264bae70.JPG)

![ili9340-7](https://cloud.githubusercontent.com/assets/6020549/25006105/91e96e4e-2096-11e7-917a-81ca38ad3d6b.JPG)

![ili9340-8](https://cloud.githubusercontent.com/assets/6020549/25006128/a2f493ee-2096-11e7-9d3c-246cb6da658e.JPG)

----

This library can use ILI9341 TFT.   

From left 2.8" ILI9341,2.4" ILI9341, 2.2" ILI9340.   

![ili9341-a](https://cloud.githubusercontent.com/assets/6020549/25058072/db0b0de2-21b0-11e7-8fe1-8dc0496c3fed.JPG)

![ili9341-b](https://cloud.githubusercontent.com/assets/6020549/25058088/f733f38a-21b0-11e7-9c71-b861f7da0c19.JPG)

![ili9341-c](https://cloud.githubusercontent.com/assets/6020549/25058093/02f7680a-21b1-11e7-8f7c-578e6127ca7e.JPG)

----

This library can use ILI9163C/ST7735 TFT.   

From left to right.   
2.2 inch 240x320 ILI9340   
1.44 inch 128x128 ST7735   
1.44 inch 128x128 ILI9163C   
1.8 inch 128x160 ST7735   

![ili9163-1](https://user-images.githubusercontent.com/6020549/28749424-d9c5af2e-7501-11e7-9e3c-a88376ac015f.JPG)

----

# A library of XPT2046 Touch Screen is included in this library   

Wirering   

|TFT||Rpi|
|:-:|:-:|:-:|
|T_IRQ|--|Pin#22(*)|
|T_OUT|--|Pin#19(SPI MOSI)|
|T_DIN|--|Pin#21(SPI MISO)|
|T_CS|--|Pin#26(SPI CE1)|
|T_CLK|--|Pin#23(SPI SCLK)|
|MISO|--|N/C|
|LED|--|3.3V|
|SCK|--|Pin#23(SPI SCLK)|
|MOSI|--|Pin#19(SPI MOSI)|
|D/C|--|Pin#3(*)|
|RES|--|Pin#5(*)|
|CS|--|Pin#24(SPI CE0)|
|GND|--|GND|
|VCC|--|3.3V|

(*) You can change any pin.   

----

cc -o xpt xpt.c xpt2046.c -lbcm2835   

If you touch screen, point will show.   

![touch-11](https://cloud.githubusercontent.com/assets/6020549/25060732/9b4ccd2e-21df-11e7-9f08-0b7377a07f10.jpg)

cc -o touch touch.c fontx.c ili9340.c xpt2046.c -lbcm2835 -lm

If you touch area, number will show.   

![touch-12](https://cloud.githubusercontent.com/assets/6020549/25060736/af89c170-21df-11e7-9789-1705e81e4692.JPG)

