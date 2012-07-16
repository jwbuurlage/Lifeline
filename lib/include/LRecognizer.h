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

#include <stdlib.h>
#include <string.h>

// Includes Listener, for callback support
#include "LListener.h"
#include "LMatch.h"

// The Recognizer struct itself
typedef struct {
    LPointData* source_points;
    LImage* source_image;
    LResultSet* results;
    
	LListener listener;
    LCharacterSet charSet;
} LRecognizer;

///////////////////////////////////////////////////////////////////////////////

// Incoming data
void recognizer_set_data(LRecognizer *recog, LPointData* pointData);

// Preprocessing
void recognizer_normalize_data(LRecognizer *recog);
void recognizer_create_image(LRecognizer *recog);

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet);
float recognizer_compare(LRecognizer *recog, LImage* source, LImage* test);

// Postprocessing & reporting
void recognizer_gather_results(LRecognizer *recog);
void recognizer_report(LRecognizer *recog);

///////////////////////////////////////////////////////////////////////////////

#endif