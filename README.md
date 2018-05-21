# myproject

1. Souce code：
> mcu
> stlink

2. how to build project ?
> install docker
> cd docker-image && ./unpkg
> cd mcu && make.sh

3. Use gdb online debug
> download arm-none-eabi-gdb
wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
tar -xvf gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2 -d /usr/

> install stlink tools.

> $ st-util
..
2018-05-19T15:09:10 INFO common.c: Device connected is: F2 device, id 0x20076411
2018-05-19T15:09:10 INFO common.c: SRAM size: 0x20000 bytes (128 KiB), Flash: 0x100000 bytes (1024 KiB) in pages of 131072 bytes
2018-05-19T15:09:10 INFO gdb-server.c: Chip ID is 00000411, Core ID is  2ba01477.
2018-05-19T15:09:10 INFO gdb-server.c: Listening at *:4242...

> new window:
$ arm-none-eabi-gdb usart_printf.elf 
GNU gdb (GNU Tools for ARM Embedded Processors) 7.10.1.20160923-cvs
Copyright (C) 2015 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-linux-gnu --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from usart_printf.elf...done.
(gdb) target remote :4242
Remote debugging using :4242
gpio_mode_setup (gpioport=0, mode=0 '\000', pull_up_down=0 '\000', gpios=0)
    at ../common/gpio_common_f0234.c:107
    107         moder &= ~GPIO_MODE_MASK(i);
(gdb) b main
    Breakpoint 1 at 0x80001c0: file usart_printf.c, line 76.
(gdb)



