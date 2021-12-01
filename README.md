# Raspberry-ili9340spi
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

---

# Wirering   

|TFT||Rpi/Opi|
|:-:|:-:|:-:|
|VCC|--|3.3V|
|GND|--|GND|
|CS|--|Pin#24(SPI CE0)|
|RES|--|Pin#5(*)|
|D/C|--|Pin#3(*)|
|MOSI|--|Pin#19(SPI MOSI)|
|SCK|--|Pin#23(SPI SCLK)|
|LED|--|3.3V|
|MISO|--|N/C|

(*) You can change any pin.   

Note:   
Opi have only 1 SPI.   
OPi-PC have CE0 and GPIO8.  
OPi ZERO have CE1 and GPIO8.   

---

# Build using bcm2835 library   
RPi Only, Very fast   

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


```
cd $HOME   
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
cc -o demo demo.c fontx.c ili9340.c -lbcm2835 -lm -DBCM
```

---

# Build using WiringPi/WiringOp library   
Both of RPi/OPi, But slow   

```
git clone https://github.com/nopnop2002/Raspberry-ili9340spi
cd Raspberry-ili9340spi
cc -o demo demo.c fontx.c ili9340.c -lwiringPi -lm -DWPI
```

---

# TFT resolution and GRAM offset   
TFT resolution is set to tft.conf.   

If your TFT doesn't use a memory from 0th address in GRAM,
It use GRAM offset which set to tft.conf.   

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

---

This library can use ILI9341 TFT.   

From left 2.8" ILI9341,2.4" ILI9341, 2.2" ILI9340.   

![ili9341-a](https://cloud.githubusercontent.com/assets/6020549/25058072/db0b0de2-21b0-11e7-8fe1-8dc0496c3fed.JPG)

![ili9341-b](https://cloud.githubusercontent.com/assets/6020549/25058088/f733f38a-21b0-11e7-9c71-b861f7da0c19.JPG)

![ili9341-c](https://cloud.githubusercontent.com/assets/6020549/25058093/02f7680a-21b1-11e7-8f7c-578e6127ca7e.JPG)

---

This library can use ILI9163C/ST7735 TFT.   

From left to right.   
2.2 inch 240x320 ILI9340   
1.44 inch 128x128 ST7735   
1.44 inch 128x128 ILI9163C   
1.8 inch 128x160 ST7735   

![ili9163-1](https://user-images.githubusercontent.com/6020549/28749424-d9c5af2e-7501-11e7-9e3c-a88376ac015f.JPG)

---

# XPT2046 Touch Screen   
A library of XPT2046 Touch Screen is included in this library.   
I ported from [here](https://github.com/xofc/xpt2uinput).

Wirering   

|TFT||Rpi|
|:-:|:-:|:-:|
|T_IRQ|--|Pin#22(*)|
|T_OUT|--|Pin#21(SPI MISO)|
|T_DIN|--|Pin#19(SPI MOSI)|
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

---

```
cc -o xpt xpt.c xpt2046.c -lbcm2835   
sudo ./xpt
```

If you touch screen, point will show.   

![touch-11](https://cloud.githubusercontent.com/assets/6020549/25060732/9b4ccd2e-21df-11e7-9f08-0b7377a07f10.jpg)

---
```
cc -o touch touch.c fontx.c ili9340.c xpt2046.c -lbcm2835 -lm -DBCM
sudo ./touch
```

If you touch area, number will show.   

![touch-12](https://cloud.githubusercontent.com/assets/6020549/25060736/af89c170-21df-11e7-9789-1705e81e4692.JPG)

