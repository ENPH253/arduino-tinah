## $Id: Wiring.mk 1153 2011-06-07 00:51:12Z bhagman $
#####################################################
#
# Wiring boards
#
# Written by Brett Hagman <bhagman@wiring.org.co>
# 2010, 2011
#
# http://wiring.org.co/
#
#####################################################

## Wiring V1 Mega
TINAH: HARDWARE = TINAH
TINAH: MCU = atmega128
TINAH: BOOTLOADER_ADDRESS = 1F000
TINAH: F_CPU = 16000000
TINAH: HW_DEFS_H = defs-wiring-v1-mega.h
TINAH: EXTRACFLAGS = -DBOOT_USART=0 -DBOOT_BAUD_RATE=-117647
TINAH: all

