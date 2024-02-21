BOOTLOADER=my-install

PROGS  = 0-user-sp-lr.c

#PROGS += 2-simple-single-step.c
#PROGS += 4-reg-restore.c
#PROGS = 5-single-step-one.c
#PROGS = 6-single-step-proc.c

PROGS += 1-rfe-example.c
PROGS += 1-rfe-blk.c
PROGS += 1-rfe-switchto-user.c

PROGS += 2-reg-save.c
PROGS += 2-reg-save-full.c

START_ASM = staff-start.S
# START_ASM = staff-start.S
# OUR_START = staff-start.S

# have a common library for the tests
# LIBNAME = libasm.a
# these are the files used in the library.
COMMON_SRC += 0-user-sp-lr-asm.S  
COMMON_SRC += 1-rfe-asm.S  
COMMON_SRC += 2-reg-save-asm.S
# single-step.c
# COMMON_SRC += 2-simple-single-step-asm.S

#COMMON_SRC += 4-reg-restore-asm.S
#COMMON_SRC += 5-single-step-one-asm.S
# STAFF_OBJS = ../../../

# automatically run.
RUN=1

CAN_EMIT=1

EXCLUDE ?= grep -v simple_boot
# include $(CS140E_2023_PATH)/libpi/mk/Makefile.template-v2
# include $(CS140E_2024_PATH)/libpi/mk/Makefile.robust
include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
