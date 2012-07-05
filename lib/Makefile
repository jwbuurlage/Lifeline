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

CFLAGS = -std=c99 -Wall -c -arch i386
AFLAGS = -Wall

BUILD_DIR = ../build

SRC = src/LRecognizer.c src/DataStructures.c
HEAD = include/LRecognizer.h include/LListener.h
OBJ = $(BUILD_DIR)/LRecognizer.o $(BUILD_DIR)/DataStructures.o

OUT = libLifeline.a

libLifeline.a : $(OBJ) Lifeline.o	
	$(AR) $(BUILD_DIR)/Lifeline.o -output $(BUILD_DIR)/$(OUT)

Lifeline.o : $(OBJ)
	ld -r $(OBJ) -o $(BUILD_DIR)/Lifeline.o

$(BUILD_DIR)/LRecognizer.o : include/LRecognizer.h include/LListener.h src/LRecognizer.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/LRecognizer.o src/LRecognizer.c
    
$(BUILD_DIR)/DataStructures.o : include/DataStructures.h src/DataStructures.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/DataStructures.o src/DataStructures.c