
BOARD_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


SRCS += board.c
SRCS += stdio.c

SRCS += bsp_i2c.c
SRCS += bsp_pushbutton.c
SRCS += bsp_status_led.c

MISC_DRIVERS_DIR ?= $(MAXIM_PATH)/Libraries/MiscDrivers

VPATH += $(BOARD_DIR)/Source
IPATH += $(BOARD_DIR)/Include

VPATH += $(MISC_DRIVERS_DIR)
IPATH += $(MISC_DRIVERS_DIR)
