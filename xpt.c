#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "bcm2835.h"
#include "xpt2046.h"

#define T_CS BCM2835_SPI_CS0
//#define T_CS BCM2835_SPI_CS1

int main (int argc, char **argv){
  int	x, y;
  int	pen_irq;

  if (bcm2835_init() != 1) return 1;
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22,BCM2835_GPIO_FSEL_INPT);

  for (;;) {
    usleep(10000);	/* do it anyway ; settle time when pen goes up */
    pen_irq = bcm2835_gpio_lev(RPI_V2_GPIO_P1_22);
    if (pen_irq == LOW)	{ /* P1.22 == PenIRQ is LOW : touch! pen is down */
      xptGetxy(T_CS, &x, &y);
      printf("touch !! x=%5d y=%5d\n", x, y);
    } else { /* pen is up */
    }
  }

  bcm2835_close();
}

