// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LFEATURES_H
#define LFEATURES_H

#include "DataStructures.h"
#include "LMatch.h"

///////////////////////////////////////////////////////////////////////////////

void image_histogram(LImage* image);
void image_moments(LImage* image, LFeatureSet* output);
int image_branch_points(LImage* image);
int image_end_points(LImage* image);
int image_cross_points(LImage* image);
int image_thin(LImage* image);

///////////////////////////////////////////////////////////////////////////////

#endif
