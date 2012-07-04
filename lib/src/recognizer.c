// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include "../include/recognizer.h"

void recognizer_add_stroke(Recognizer *recog, Stroke *stroke) 
{
    return;
}

void recognizer_recognize(Recognizer *recog)
{
    Listener listener = (*recog).listener;
    listener.char_found('c', listener.obj);
    return;
}
