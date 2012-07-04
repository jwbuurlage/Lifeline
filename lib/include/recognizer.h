// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef RECOGNIZER_H 
#define RECOGNIZER_H

#include "listener.h"

typedef enum
{
    CharacterSetNumeric,
    CharacterSetAlphabet,
    CharacterSetAlphanumeric
} CharacterSet;

typedef struct {
    float* points;
    int length;
} Stroke;

typedef struct Recognizer_ {
    Stroke* current_strokes;
	Listener listener;
    CharacterSet charSet;
} Recognizer;

void recognizer_add_stroke(Recognizer *recog, Stroke *stroke);
void recognizer_recognize(Recognizer *recog);

#endif