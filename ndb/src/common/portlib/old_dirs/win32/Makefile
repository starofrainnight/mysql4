include .defs.mk

TYPE := util

PIC_ARCHIVE    := Y
ARCHIVE_TARGET := portlib

SOURCES.c =  NdbCondition.c \
	     NdbMutex.c     \
	     NdbSleep.c     \
             NdbTick.c      \
             NdbEnv.c       \
             NdbThread.c    \
             NdbHost.c      \
	     NdbTCP.c	    \
	     NdbDaemon.c

ifeq ($(NDB_OS), SOFTOSE)
	SOURCES += NdbMem_SoftOse.cpp
else
	SOURCES.c += NdbMem.c
endif

include $(NDB_TOP)/Epilogue.mk






