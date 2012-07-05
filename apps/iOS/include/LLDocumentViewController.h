// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import <UIKit/UIKit.h>

#import "LLListenerProtocol.h"
#include "LLGridView_Debug.h"

@interface LLDocumentViewController : UIViewController <LLListener>
{
    UITextView* textView;
    
    // DEBUG
    _LLGridView* gridView;
    UITextView* results;
}

@end
