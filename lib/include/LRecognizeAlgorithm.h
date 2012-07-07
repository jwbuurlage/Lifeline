// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef L_RECOGNIZE_ALGORITHM_H
#define L_RECOGNIZE_ALGORITHM_H

#include "LMatch.h"

typedef void (*alg_set_data)(LRecognizeAlgorithm* alg, PointData* data);

typedef struct
{
    alg_set_data set_data;
    
    PointData* data;
} LRecognizeAlgorithm;

#endif