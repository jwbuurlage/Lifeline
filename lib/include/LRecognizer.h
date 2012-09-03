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

#include "LListener.h"
#include "LMatch.h"
#include "LFeatures.h"

typedef struct {
    int image_size; // has to be odd

    LPointData* source_points;
    LImage* source_image;
    LResultSet* results;

    LListener listener;
    LCharacterSet charSet;

    LFeatureSet currentFeatures; //Calculated features of the current symbol
} LRecognizer;

///////////////////////////////////////////////////////////////////////////////

void insert_char_into_list(List* list, char character, unsigned int* grid);
LPoint* points_center(LPointData* pointData);

///////////////////////////////////////////////////////////////////////////////

/*!
 * Initialize character data from file, and sets up the recognizer to use it.
 */
void recognizer_load_data(LRecognizer *recog);

/*!
 * Set the client data set to test against. The recognizer will proceed to
 * process the data and match the character against the database.
 */
void recognizer_set_data(LRecognizer *recog, LPointData* pointData);

/*!
 * Normalizes the data by sending the smallest square that fits the data to the
 * unit square [-1, 1] x [-1, 1].
 */
void recognizer_normalize_data(LRecognizer *recog);

/*!
 * Connects the data, such that there are no gaps in a single stroke. The time
 * offset for a new stroke is 0.1 seconds.
 */
void recognizer_connect_data(LRecognizer *recog);

/*!
 * Creates an LImage from the source_data and saves it in recog.source_image
 */
void recognizer_create_image(LRecognizer *recog);

///////////////////////////////////////////////////////////////////////////////

void recognizer_clear_samples(LRecognizer *recog);
void recognizer_take_sample(LRecognizer *recog);
void recognizer_save_samples(LRecognizer *recog, char* symbol);

#endif
