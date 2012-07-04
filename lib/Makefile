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

BUILD_DIR = ../build

SRC = src/LRecognizer.c src/DataStructures.c
HEAD = include/LRecognizer.h include/LListener.h
OBJ = $(BUILD_DIR)/LRecognizer.o

OUT = libLifeline.a

libLifeline.a : $(OBJ)
	$(AR) $(OBJ) -output $(BUILD_DIR)/$(OUT)

$(BUILD_DIR)/recognizer.o : include/recognizer.h include/listener.h src/recognizer.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/recognizer.o src/recognizer.c