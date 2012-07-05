// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include <LListener.h>

@protocol LLListener <NSObject>

void bestMatch(char best_char, void* obj);
void sourceImage(LImage* src, void*obj);
void resultSet(LResultSet* result, void*obj);

-(LListener)C_LListener;

@end