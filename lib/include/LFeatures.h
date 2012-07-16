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
float image_moment(LImage* image, int orderX, int orderY);
float image_zernike_moment(LImage* image);
List* image_branch_points(LImage* image);
List* image_end_points(LImage* image);
List* image_cross_points(LImage* image);

///////////////////////////////////////////////////////////////////////////////

#endif