/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
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

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/f2/rng.h>
#include <libopencm3/stm32/f2/rcc.h>
#include <libopencm3/stm32/flash.h>

#include "rng.h"
#include "layout.h"
#include "setup.h"

uint32_t __stack_chk_guard;

void __attribute__((noreturn)) __stack_chk_fail(void)
{
	layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Stack smashing", "detected.", NULL, "Please unplug", "the device.", NULL);
	for (;;) {} // loop forever
}

void nmi_handler(void)
{
	// Clock Security System triggered NMI
	if ((RCC_CIR & RCC_CIR_CSSF) != 0) {
		layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Clock instability", "detected.", NULL, "Please unplug", "the device.", NULL);
		for (;;) {} // loop forever
	}
}

#if 1 // for debug, resovled sys hang issue.
const struct rcc_clock_scale rcc_hse_8mhz_3v3_ext[3] = {
    { /* 48MHz */
        .pllm = 8,
        .plln = 96,
        .pllp = 2,
        .pllq = 2,
        .hpre = RCC_CFGR_HPRE_DIV_NONE,
        .ppre1 = RCC_CFGR_PPRE_DIV_4,
        .ppre2 = RCC_CFGR_PPRE_DIV_2,
        .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
            FLASH_ACR_LATENCY_3WS,
        .apb1_frequency = 12000000,
        .apb2_frequency = 24000000,
    },
    { /* 84MHz */
        .pllm = 8,
        .plln = 336,
        .pllp = 4,
        .pllq = 7,
        .hpre = RCC_CFGR_HPRE_DIV_NONE,
        .ppre1 = RCC_CFGR_PPRE_DIV_2,
        .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
        .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
            FLASH_ACR_LATENCY_2WS,
        .apb1_frequency = 42000000,
        .apb2_frequency = 84000000,
    },
    { /* 120MHz */
        .pllm = 8,
        .plln = 240,
        .pllp = 2,
        .pllq = 5,
        .hpre = RCC_CFGR_HPRE_DIV_NONE,
        .ppre1 = RCC_CFGR_PPRE_DIV_4,
        .ppre2 = RCC_CFGR_PPRE_DIV_2,
        .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
            FLASH_ACR_LATENCY_3WS,
        .apb1_frequency = 30000000,
        .apb2_frequency = 60000000,
    },
};
#endif

void setup(void)
{
	// set SCB_CCR STKALIGN bit to make sure 8-byte stack alignment on exception entry is in effect.
	// This is not strictly necessary for the current TREZOR system.
	// This is here to comply with guidance from section 3.3.3 "Binary compatibility with other Cortex processors"
	// of the ARM Cortex-M3 Processor Technical Reference Manual.
	// According to section 4.4.2 and 4.4.7 of the "STM32F10xxx/20xxx/21xxx/L1xxxx Cortex-M3 programming manual",
	// STM32F2 series MCUs are r2p0 and always have this bit set on reset already.
	SCB_CCR |= SCB_CCR_STKALIGN;

	// setup clock
	/** struct rcc_clock_scale clock = rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_120MHZ]; */
	struct rcc_clock_scale clock = rcc_hse_8mhz_3v3_ext[1];
	rcc_clock_setup_hse_3v3(&clock);

	// enable GPIO clock - A (oled), B(oled), C (buttons)
    rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	// enable SPI clock
	rcc_periph_clock_enable(RCC_SPI2);

	// enable OTG FS clock
	rcc_periph_clock_enable(RCC_OTGFS);

	// enable RNG
	rcc_periph_clock_enable(RCC_RNG);
	RNG_CR |= RNG_CR_RNGEN;
	// to be extra careful and heed the STM32F205xx Reference manual, Section 20.3.1
	// we don't use the first random number generated after setting the RNGEN bit in setup
	random32();

	// enable CSS (Clock Security System)
	RCC_CR |= RCC_CR_CSSON;

	// set GPIO for buttons
	gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO9 | GPIO8);

	// set GPIO for OLED display
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1);
  
	// enable SPI 1 for OLED display
   /**  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO7); */
	/** gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO7); */
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
    gpio_set_af(GPIOC, GPIO_AF5, GPIO3);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13);
    gpio_set_af(GPIOB, GPIO_AF5, GPIO13);

//	spi_disable_crc(SPI1);
	spi_init_master(SPI2, 
        SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
	spi_enable_ss_output(SPI2);
//	spi_enable_software_slave_management(SPI1);
//	spi_set_nss_high(SPI1);
//	spi_clear_mode_fault(SPI1);
	spi_enable(SPI2);

	// enable OTG_FS
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12);
}

void setupApp(void)
{
	// for completeness, disable RNG peripheral interrupts for old bootloaders that had
	// enabled them in RNG control register (the RNG interrupt was never enabled in the NVIC)
	RNG_CR &= ~RNG_CR_IE;
	// the static variables in random32 are separate between the bootloader and firmware.
	// therefore, they need to be initialized here so that we can be sure to avoid dupes.
	// this is to try to comply with STM32F205xx Reference manual - Section 20.3.1:
	// "Each subsequent generated random number has to be compared with the previously generated
	// number. The test fails if any two compared numbers are equal (continuous random number generator test)."
	random32();

	// enable CSS (Clock Security System)
	RCC_CR |= RCC_CR_CSSON;

	// hotfix for old bootloader
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO9);
	spi_init_master(SPI1, 
        SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
}
