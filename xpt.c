#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "bcm2835.h"
#include "xpt2046.h"

#define T_CS BCM2835_SPI_CS1
#define RES RPI_V2_GPIO_P1_11
#define T_IRQ RPI_V2_GPIO_P1_22

int main (int argc, char **argv){
	int	x, y;
	int	pen_irq;

	if (bcm2835_init() != 1) return 1;
	bcm2835_gpio_fsel(T_IRQ,BCM2835_GPIO_FSEL_INPT); // T_IRQ
	bcm2835_gpio_fsel(RES,BCM2835_GPIO_FSEL_OUTP); // RES
	bcm2835_gpio_write(RES, LOW);
	bcm2835_delay(100);
	bcm2835_gpio_write(RES, HIGH);
	bcm2835_delay(100);

	for (;;) {
		usleep(10000);	/* do it anyway ; settle time when pen goes up */
		pen_irq = bcm2835_gpio_lev(T_IRQ);
		if (pen_irq == LOW)	{ /* P1.22 == PenIRQ is LOW : touch! pen is down */
			xptGetxy(T_CS, &x, &y);
			printf("touch !! x=%5d y=%5d\n", x, y);
		} else { /* pen is up */

		}
	} // end for

	bcm2835_close();
}

