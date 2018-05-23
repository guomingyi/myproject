/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

int _write(int file, char *ptr, int len);

static void clock_setup(void)
{
	/* Enable clocks on all the peripherals we are going to use. */
	rcc_periph_clock_enable(RCC_USART1);
	/** rcc_periph_clock_enable(RCC_GPIOC); */
	rcc_periph_clock_enable(RCC_GPIOA);
}

static void usart_setup(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 9600);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

/*
* stdio libc will call this func when use printf.
*/

#if 0
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}
#endif

static inline void delay(uint32_t wait)
{
    while (--wait > 0) __asm__("nop");
}

int usart_init(void)
{
    clock_setup();
    usart_setup();
    return 0;
}


void uart_val(int a)
{ 
    char buf[64] = {"val:"};
    char *p = NULL; 
    int t = 0;
    int i = 4;

    t = a;
    while (t > 0) {
        buf[i++] = (t % 10) + '0';
        t = t/10;
    }

    if (i > 0)
        buf[i] = ' ';

    p = buf;
    while(p != NULL && *p != 0) { 
        usart_send_blocking(USART1, *p++); 
    }  
}

void uart_str(char *s) 
{
    char *p = s;
    while(p != NULL && *p != 0) { 
        usart_send_blocking(USART1, *p++); 
    }
}

