#
# Makefile for the LibGTop linux sysctl interface.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definition is now in the main makefile...

O_TARGET := kernel.o
ifeq ($(CONFIG_LIBGTOP),y)
O_OJBS   := main.o libgtop.o
else
O_OBJS   := main.o
endif
OX_OBJS  := libgtop_syms.o

ifeq ($(CONFIG_LIBGTOP),m)
M_OBJS   := libgtop.o
endif

include $(TOPDIR)/Rules.make
