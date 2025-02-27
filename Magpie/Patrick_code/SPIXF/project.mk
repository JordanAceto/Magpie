# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analogdevicesinc.github.io/msdk/USERGUIDE/#build-system

# **********************************************************

# Add your config here!

# If you have secure version of MCU (MAX32666), set SBT=1 to generate signed binary
# For more information on how sing process works, see
# https://www.analog.com/en/education/education-library/videos/6313214207112.html
SBT=0

#ifneq ($(BOARD),EvKit_V1)
#$(error ERR_NOTSUPPORTED: This example requires an external flash IC that is only available for the MAX32665EVKIT)
#endif

#PROJ_CFLAGS+=-DEXT_FLASH_W25

VPATH += ExtMemory
IPATH += ExtMemory