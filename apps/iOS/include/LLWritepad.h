// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include <recognizer.h>

@class LLWritepad;

@interface LLWritepadController : NSObject
{
    Recognizer recognizer;
}

- (void)strokeFinished:(LLWritepad*)pad;
- (void)touchDown:(LLWritepad*)pad event:(UIEvent*)event;
- (void)touchDragInside:(LLWritepad*)pad event:(UIEvent*)event;
- (void)touchUpInside:(LLWritepad*)pad event:(UIEvent*)event;

@end

@interface LLWritepad : UIControl {
    LLWritepadController* controller;
}

- (id)initWithFrame:(CGRect)aFrame delegate:(id)delegate;

@end