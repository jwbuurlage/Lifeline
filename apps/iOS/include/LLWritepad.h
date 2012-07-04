// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include <LRecognizer.h>
#include <LMatch.h>
#include "LLListenerProtocol.h"

@class LLWritepadController;

@interface LLWritepad : UIControl {
    LLWritepadController* controller;
}

- (id)initWithFrame:(CGRect)aFrame delegate:(id)delegate;

@end

@interface LLWritepadController : NSObject
{
    LRecognizer recognizer;
    
    LPointData pointData;
    
    // paths for the view
    NSMutableArray* paths;
    UIBezierPath* bp;
    CGPoint previousPoint;
}

@property NSMutableArray* paths;

- (id)initWithDelegate:(id <LLListener>)delegate;

- (void)pushPointToData:(CGPoint)point;

- (void)touchDown:(LLWritepad*)pad event:(UIEvent*)event;
- (void)touchDragInside:(LLWritepad*)pad event:(UIEvent*)event;
- (void)touchUpInside:(LLWritepad*)pad event:(UIEvent*)event;

- (void)recognize:(LLWritepad*)pad;

@end