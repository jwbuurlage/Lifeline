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

///////////////////////////////////////////////////////////////////////////////

void image_histogram(LImage* image);
float image_moment(LImage* image);
float image_zernike_moment(LImage* image);

///////////////////////////////////////////////////////////////////////////////

#endif