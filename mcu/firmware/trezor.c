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

#include "trezor.h"
#include "oled.h"
#include "bitmaps.h"
#include "util.h"
#include "usb.h"
#include "setup.h"
#include "storage.h"
#include "layout.h"
#include "layout2.h"
#include "rng.h"
#include "timer.h"
#include "buttons.h"
#include "gettext.h"
#include "fastflash.h"

void check_lock_screen(void)
{
	buttonUpdate();

	// wake from screensaver on any button
	if (layoutLast == layoutScreensaver && (button.NoUp || button.YesUp)) {
		layoutHome();
		return;
	}

	// button held for long enough (2 seconds)
	if (layoutLast == layoutHome && button.NoDown >= 285000 * 2) {

		layoutDialog(&bmp_icon_question, _("Cancel"), _("Lock Device"), NULL, _("Do you really want to"), _("lock your TREZOR?"), NULL, NULL, NULL, NULL);

		// wait until NoButton is released
		usbTiny(1);
		do {
			usbSleep(5);
			buttonUpdate();
		} while (!button.NoUp);

		// wait for confirmation/cancellation of the dialog
		do {
			usbSleep(5);
			buttonUpdate();
		} while (!button.YesUp && !button.NoUp);
		usbTiny(0);

		if (button.YesUp) {
			// lock the screen
			session_clear(true);
			layoutScreensaver();
		} else {
			// resume homescreen
			layoutHome();
		}
	}

	// if homescreen is shown for longer than 10 minutes, lock too
	if (layoutLast == layoutHome) {
		if ((system_millis - system_millis_lock_start) >= 600000) {
			// lock the screen
			session_clear(true);
			layoutScreensaver();
		}
	}
}

int main(void)
{
    uart_init("application-main():\n");
#ifndef APPVER
	setup();
	__stack_chk_guard = random32(); // this supports compiler provided unpredictable stack protection checks
	oledInit();
#else
    uart_printf("application-setupApp():\n");
	setupApp();
    #ifndef UART_DEBUG
    ssss
	__stack_chk_guard = random32(); // this supports compiler provided unpredictable stack protection checks
    #endif
#endif

#if FASTFLASH
aaa
	uint16_t state = gpio_port_read(BTN_PORT);
	if ((state & BTN_PIN_NO) == 0) {
		run_bootloader();
	}
#endif

    uart_printf("application-timer_init():\n");
	timer_init();

#if DEBUG_LINK
fdf
	oledSetDebugLink(1);
	storage_reset(); // wipe storage if debug link
	storage_reset_uuid();
	storage_commit();
	storage_clearPinArea(); // reset PIN failures if debug link
#endif

    uart_printf("application-oledDrawBitmap:bmp_logo64:\n");
	oledDrawBitmap(40, 0, &bmp_logo64);
	oledRefresh();

    uart_printf("application-storage_init():\n");
	storage_init();

    uart_printf("application-layoutHome():\n");
	layoutHome();

    uart_printf("application-usbInit():\n");
	usbInit();
	for (;;) {
        uart_printf("application-usbPoll():\n");
		usbPoll();
		check_lock_screen();
	}

	return 0;
}
