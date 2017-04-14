# ili9340spi_rpi
ILI9340 SPI TFT Library for Raspberry


This is library for RaspberryPi.   
This library can show a chart to ILI9340/ILI9341 SPI TFT.   

----

Wirering   

|TFT||Rpi/Opi|
|:-:|:-:|:-:|
|MISO|--|N/C|
|LED|--|3.3V|
|SCK|--|Pin#23|
|MOSI|--|Pin#19|
|D/C|--|Pin#3|
|RES|--|Pin#5|
|CS|--|Pin#24|
|GND|--|GND|
|5V|--|5V|

build   
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
cc -o demo demo.c fontx.c ili9340.c -lbcm2835 -lm   

----

![ili9340-1](https://cloud.githubusercontent.com/assets/6020549/25006015/3dd4f15c-2096-11e7-82db-d444ee689cb0.JPG)

![ili9340-2](https://cloud.githubusercontent.com/assets/6020549/25006026/4d36b842-2096-11e7-9030-1d7455a23a53.JPG)

![ili9340-3](https://cloud.githubusercontent.com/assets/6020549/25006042/5c2b1992-2096-11e7-9ee8-e1a4fe2b1b03.JPG)

![ili9340-4](https://cloud.githubusercontent.com/assets/6020549/25006061/6ccffa24-2096-11e7-90bf-caebc15cf942.JPG)

![ili9340-5](https://cloud.githubusercontent.com/assets/6020549/25006078/80c4b5d8-2096-11e7-8c88-650f8df1699f.JPG)

![ili9340-6](https://cloud.githubusercontent.com/assets/6020549/25006148/b546b11c-2096-11e7-83ad-7352264bae70.JPG)

![ili9340-7](https://cloud.githubusercontent.com/assets/6020549/25006105/91e96e4e-2096-11e7-917a-81ca38ad3d6b.JPG)

![ili9340-8](https://cloud.githubusercontent.com/assets/6020549/25006128/a2f493ee-2096-11e7-9d3c-246cb6da658e.JPG)

