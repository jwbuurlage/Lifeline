// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LLISTENER_H 
#define LLISTENER_H

#include "LMatch.h"

/* Function typedefs */

typedef void (*callback_best_match)(char character, void* obj);
typedef void (*callback_result_set)(LResultSet result, void* obj);
typedef void (*callback_source_image)(LImage src, void* obj);

/* The Listener struct itself */

typedef struct
{
    // any data required 'on the other side' like Obj-C instances
    void* obj; 
    
    // callback functions
    callback_best_match char_found;
    callback_result_set result_set;
    callback_source_image source_image;
} LListener;

#endif