# ----------------------------------------------------------------------------
# BYN SOFTWARE, COPYRIGHT 2012
# 
# Author: Jan-Willem Buurlage et al.
# Contact: j.buurlage@students.uu.nl
# 
# Part of the Lifeline project, handwriting recognition for scientific wr-
# iting. Tailored for touchscreens.
# ----------------------------------------------------------------------------

CC = gcc
AR = lipo -create

CFLAGS = -Wall -c -arch i386
AFLAGS = -Wall

SRC = src/lifeline.c
HEAD = include/lifeline.c
OBJ = build/lifeline.o

TABLET_TARGET = build-iPhoneSimulator

OUT = build/${TABLET_TARGET}/libLifeline.a

libLifeline.a : $(OBJ)
	$(AR) $(OBJ) -output $(OUT)

build/lifeline.o : include/lifeline.h src/lifeline.c
	$(CC) $(CFLAGS) -o build/lifeline.o src/lifeline.c