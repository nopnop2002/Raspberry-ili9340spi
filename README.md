# Raspberry-ili9340spi
ILI9340 SPI TFT Library for Raspberry Pi.   
This may works with other boards like OrangePi/NanoPi.   

You can show a chart to ILI9340/ILI9341/ILI9163C/ST7735 SPI TFT.   
You can choose bmc2835 library/WiringPi library.   

I tested these TFT.   
1.44 inch 128x128 ST7735   
1.44 inch 128x128 ILI9163C   
1.8 inch 128x160 ST7735   
2.2 inch 240x320 ILI9340   
2.4 inch 240x320 ILI9341   
2.4 inch 240x320 ILI9341   

This project can be built with either:
- Build using bcm2835 library   
- Build using Hardware SPI of the WiringPi library   
- Build using Software SPI of the WiringPi library   

---

# Wirering   

|TFT||GPIO Header||
|:-:|:-:|:-:|:-:|
|VCC|--|3.3V|*4|
|GND|--|GND||
|CS|--|Pin#24(SPI CS0)|*2 *3|
|RES|--|Pin#12|*1|
|D/C|--|Pin#11|*1|
|MOSI|--|Pin#19(SPI MOSI)|*3|
|SCK|--|Pin#23(SPI SCLK)|*3|
|LED|--|3.3V|*4|
|MISO|--|N/C||

(*1) You can change it to any pin by changing source.   

(*2) You can use CS1 by specifying compilation flags.   

(*3) For Software SPI, you can change it to any pin by changing source.   

(*4) SPI TFTs require a lot of current.   
If it is supplied from the Raspberry Pi's 3.3V pin, it may run out of current.   
Use the 5V pin and the regulator to power it and it will be stable.   

---

# Build using bcm2835 library   
RPi Only, Very fast   

### Install bcm2835 library

```
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
tar zxvf bcm2835-1.56.tar.gz
cd bcm2835-1.56
./configure
make
sudo make check
sudo make install
```

__\* This tool require 1.56 or later.__   
__\* Because this tool uses bcm2835_spi_write.__   

### Using other GPIO
You can change GPIO to any pin by changing here.   
```
#ifdef BCM
#include <bcm2835.h>
#define D_C 17  // BCM IO17=Pin#11
#define RES 18  // BCM IO18=Pin#12
#endif
```

### Using SPI0
Use Pin#24 as ChipSelect.   
```
cd $HOME   
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
make lib
cc -o demo demo.c fontx.c ili9340.c jpeg.a png.a -lbcm2835 -lm -DBCM
sudo ./demo
```

### Using SPI1
Use Pin#26 as ChipSelect.   
```
cd $HOME   
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
make lib
cc -o demo demo.c fontx.c ili9340.c jpeg.a png.a -lbcm2835 -lm -DBCM -DSPI1
sudo ./demo
```

### SPI bus speed for bcm2835   
By default it uses 7.8125MHz on Rpi2, 12.5MHz on RPI3.   
Can be changed at compile time.   
- -DSPI_SPEED16 : 15.625MHz on Rpi2, 25MHz on RPI3.   
- -DSPI_SPEED32 : 31.25MHz on Rpi2, 50MHz on RPI3.   

___50MHz is an overclock.___   



---

# Build using Hardware SPI of the WiringPi library   
WiringPi library initializes GPIO in one of the following ways:
- int wiringPiSetup (void);   
- int wiringPiSetupGpio (void);   
- int wiringPiSetupPhys (void);   
- int wiringPiSetupSys (void);   

This project by default uses the ```wiringPiSetup()``` function to initialize GPIOs.   
Then use the wiringPiSPISetup() function to initialize the SPI.   
If you use it on a board other than the RPI board, you may need to change the WiringPi number.
```
#define D_C   0 // wPi IO00=Pin#11
#define RES   1 // wPi IO01=Pin#12
```

As far as I know, there are these WiringPi libraries.   
- WiringPi for OrangePi   
- WiringPi for BananaPi   
- WiringPi for NanoPi   
- WiringPi for Pine-64   

If you want to initialize GPIO with ```wiringPiSetupGpio()```, Use the -DGPIO compilation flag.   
In this case, use the following GPIOs.   
```
#define D_C  17 // BCM IO17=Pin#11
#define RES  18 // BCM IO18=Pin#12
```



### Using SPI0
Use Pin#24 as ChipSelect.   
```
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
make lib
cc -o demo demo.c fontx.c ili9340.c jpeg.a png.a -lwiringPi -lm -pthread -DWPI
sudo ./demo
```

### Using SPI1
Use Pin#26 as ChipSelect.   
```
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
make lib
cc -o demo demo.c fontx.c ili9340.c jpeg.a png.a -lwiringPi -lm -pthread -DWPI -DSPI1
sudo ./demo
```

___Note for OrangePi___   
Opi have only 1 SPI.   
OPi-PC has SPI0 on pin 24.   
OPi-ZERO has SPI1 on pin 24.   

### SPI bus speed for WiringPi   
By default it uses 8MHz on all Rpi.   
Can be changed at compile time.   
- -DSPI_SPEED16 : 16MHz on all Rpi.   
- -DSPI_SPEED32 : 32MHz on all Rpi.   


---

# Build using Software SPI of the WiringPi library   
WiringPi library initializes GPIO in one of the following ways:
- int wiringPiSetup (void);   
- int wiringPiSetupGpio (void);   
- int wiringPiSetupPhys (void);   
- int wiringPiSetupSys (void);   

This project by default uses the ```wiringPiSetup()``` function to initialize GPIOs.   
Then use the wiringPiSPISetup() function to initialize the SPI.   
If you use it on a board other than the RPI board, you may need to change the WiringPi number.
```
#define D_C   0 // wPi IO00=Pin#11
#define RES   1 // wPi IO01=Pin#12
#define MOSI 12 // wPi IO12=Pin#19
#define SCLK 14 // wPi IO14=Pin#23
#define CS   10 // wPi IO10=Pin#24
```

If you want to initialize GPIO with ```wiringPiSetupGpio()```, Use the -DGPIO compilation flag.   
In this case, use the following GPIOs.   
```
#define D_C  17 // BCM IO17=Pin#11
#define RES  18 // BCM IO18=Pin#12
#define MOSI 10 // BCM IO10=Pin#19
#define SCLK 11 // BCM IO11=Pin#23
#define CS   24 // BCM IO24=Pin#24
```
```
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
make lib
cc -o demo demo.c fontx.c ili9340.c jpeg.a png.a -lwiringPi -lm -pthread -DWPI -DSOFT_SPI
sudo ./demo
```


---

# TFT resolution and GRAM offset   
TFT resolution is set to tft.conf.   

If your TFT doesn't use a memory from 0th address in GRAM,
It use GRAM offset which set to tft.conf.   

```
#width=128 height=128
width=240 height=320
#width=240 height=400

#If TFT have GRAM offset
#offsetx=2
#offsety=1
```


----

![ili9340-11](https://user-images.githubusercontent.com/6020549/58363270-668e0880-7edc-11e9-8f5a-ad00c60c5d4d.JPG)
![ili9340-12](https://user-images.githubusercontent.com/6020549/58363271-668e0880-7edc-11e9-80f9-4019c53c334d.JPG)
![ili9340-13](https://user-images.githubusercontent.com/6020549/58363272-668e0880-7edc-11e9-8ced-64367179c509.JPG)
![ili9340-14](https://user-images.githubusercontent.com/6020549/58363273-668e0880-7edc-11e9-84c1-779bd70a9ac4.JPG)
![ili9340-15](https://user-images.githubusercontent.com/6020549/58363274-67269f00-7edc-11e9-874e-b96165374809.JPG)
![ili9340-16](https://user-images.githubusercontent.com/6020549/58363275-67269f00-7edc-11e9-9664-2e7a2fe6d6bf.JPG)
![ili9340-17](https://user-images.githubusercontent.com/6020549/58363276-67269f00-7edc-11e9-9fc4-579a03e6bfd2.JPG)
![ili9340-18](https://user-images.githubusercontent.com/6020549/58363277-67269f00-7edc-11e9-9d77-2ebacc8666c5.JPG)
![ili9340-19](https://user-images.githubusercontent.com/6020549/58363278-67bf3580-7edc-11e9-9e95-c9daaa85c4b1.JPG)
![ili9340-20](https://user-images.githubusercontent.com/6020549/58363268-65f57200-7edc-11e9-8cc8-af25397d5e24.JPG)
![ili9340-21](https://user-images.githubusercontent.com/6020549/58363269-65f57200-7edc-11e9-89f9-8ad644e0b279.JPG)

JPEG File   
![ili9340-JPEG](https://user-images.githubusercontent.com/6020549/232970130-3241a691-48d6-4b48-b34f-ce7a2e42f6c3.JPG)   
PNG File   
![ili9340-PNG](https://user-images.githubusercontent.com/6020549/232970139-5b23c012-f678-45fb-bd76-fcb63f6a9c26.JPG)   

---

From left to right:   
2.8" 240x320 ILI9341   
2.4" 240x320 ILI9341   
2.2" 240x320 ILI9340   

![ILI9341-A](https://user-images.githubusercontent.com/6020549/232970787-2374f04f-def0-4498-b585-c287b9e31663.JPG)
![ILI9341-B](https://user-images.githubusercontent.com/6020549/232970793-dc0c753a-6da9-4ac9-bab8-c5ebd0752221.JPG)
![ILI9341-C](https://user-images.githubusercontent.com/6020549/232970799-f87800de-a6f4-435e-b9e9-a2a8a6da6b95.JPG)

---

From left to right:   
2.2" 240x320 ILI9340   
1.44" 128x128 ST7735   
1.44" 128x128 ILI9163C   
1.8" 128x160 ST7735   

![ili9163-1](https://user-images.githubusercontent.com/6020549/28749424-d9c5af2e-7501-11e7-9e3c-a88376ac015f.JPG)

---

# XPT2046 Touch Screen   
There is a TFT equipped with XPT2046.   
![XPT2046-3](https://user-images.githubusercontent.com/6020549/144333924-5236bff3-3f4d-4be4-8e99-b6e31878e4f3.jpg)

A library of XPT2046 Touch Screen is included in this library.   
I ported from [here](https://github.com/xofc/xpt2uinput).

There is a TFT equipped with HR2046.   
XPT2046 and HR2046 are very similar. But HR2046 does not work properly.   
![XPT2046-2](https://user-images.githubusercontent.com/6020549/144332571-717f33b1-df03-4a0a-9a23-c7c99b9d4d32.JPG)

Wirering   

|TFT||Rpi||
|:-:|:-:|:-:|:-:|
|T_IRQ|--|Pin#22|(*1)|
|T_OUT|--|Pin#21(SPI MISO)|(*2)|
|T_DIN|--|Pin#19(SPI MOSI)|(*2)|
|T_CS|--|Pin#26(SPI CE1)||
|T_CLK|--|Pin#23(SPI SCLK)|(*2)|
|MISO|--|N/C||
|LED|--|3.3V||
|SCK|--|Pin#23(SPI SCLK)|(*2)|
|MOSI|--|Pin#19(SPI MOSI)|(*2)|
|D/C|--|Pin#3|(*1)|
|RES|--|Pin#5|(*1)|
|CS|--|Pin#24(SPI CS0)||
|GND|--|GND||
|VCC|--|3.3V||

(*1) You can change any pin.   

(*2) SPI is shared by TFT and XPT2046.   

---

```
cc -o xpt xpt.c xpt2046.c -lbcm2835   
sudo ./xpt
```

If you touch screen, point will show.   

![Touch-11](https://user-images.githubusercontent.com/6020549/232971208-6904db4f-15a8-492b-8e66-89c34c85d8e0.jpg)

---
```
cc -o touch touch.c fontx.c ili9340.c xpt2046.c -lbcm2835 -lm -DBCM
sudo ./touch
```

If you touch area, number will show.   

![Touch-12](https://user-images.githubusercontent.com/6020549/232971233-a03cd5e8-f7e9-4f30-957e-b69fa2474b4d.JPG)
