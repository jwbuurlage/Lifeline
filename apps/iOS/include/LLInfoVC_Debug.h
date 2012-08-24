// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include "LLGridView_Debug.h"

@interface LLInfoVC : UIViewController
{
    id delegate;
    LLGridView* gridView;  
}

@property (readwrite) id delegate;

- (void)setImage:(LImage*)image;

@end