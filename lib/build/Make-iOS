# ----------------------------------------------------------------------------
# BYN SOFTWARE, COPYRIGHT 2012
# 
# Author: Jan-Willem Buurlage et al.
# Contact: j.buurlage@students.uu.nl
# 
# Part of the Lifeline project, handwriting recognition for scientific wr-
# iting. Tailored for touchscreens.
# ----------------------------------------------------------------------------

AR = lipo -create

CC = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/llvm-gcc-4.2
SDKDIR = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk

# CFLAGS = -std=c99 -Wall -c -arch armv7 -isysroot $(SDKDIR) 
CFLAGS = -std=c99 -Wall -c -arch i386

AFLAGS = -Wall

BUILD_DIR = ../build

SRC = src/LRecognizer.c src/DataStructures.c
HEAD = include/LRecognizer.h include/LListener.h
OBJ = $(BUILD_DIR)/LRecognizer.o $(BUILD_DIR)/DataStructures.o $(BUILD_DIR)/LFeatures.o

OUT = libLifeline.a

libLifeline.a : Lifeline.o	
	lipo -create $(BUILD_DIR)/Lifeline.o -output $(BUILD_DIR)/$(OUT)

Lifeline.o : $(OBJ)
	ld -r $(OBJ) -o $(BUILD_DIR)/Lifeline.o

$(BUILD_DIR)/LRecognizer.o : include/LRecognizer.h include/LListener.h src/LRecognizer.c include/LTestCharacters.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/LRecognizer.o src/LRecognizer.c
    
$(BUILD_DIR)/DataStructures.o : include/DataStructures.h src/DataStructures.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/DataStructures.o src/DataStructures.c
    
$(BUILD_DIR)/LFeatures.o : include/LFeatures.h src/LFeatures.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/LFeatures.o src/LFeatures.c
    
clean :
	rm $(BUILD_DIR)/*